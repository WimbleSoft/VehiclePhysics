#pragma once
#include "ACVehiclePhysics.h"
#include "Vehicle.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetTextLibrary.h>
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UACVehiclePhysics::UACVehiclePhysics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//MechanicalData.AxisData = TArray<FSAxis>();
	//MechanicalData.BrakeData = FSBrake();
	//MechanicalData.ClutchData = FSClutch();
	//MechanicalData.EngineData = FSEngine();
	//MechanicalData.FuelData = FSFuel();
	//MechanicalData.GearBoxData = FSGearBox();
	//MechanicalData.NitroData = FSNitro();
	//MechanicalData.TransmissionData = FSTransmission();
	//MechanicalData.TurboData = FSTurbo();

}

// Called when the game starts
void UACVehiclePhysics::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UACVehiclePhysics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	TArray<USCWheel*> Suspensions = GetSuspensions();

	for(USCWheel* AxisObj : Suspensions)
	{
		if (!AxisObj) continue;
		AxisObj->UpdatePhysics(DeltaTime);
	}

	if (IsActive()) {
		CalcTurbo(1.0f);
		CalcNitrous(1.0f);
		CalcFuelConsumption(1.0f);
		SetLoad();
		double EngineTorqueL = CalcEngine();
		double ClutchTorqueL = CalcClutch(EngineTorqueL);
		CalcTransmissionTorque(ClutchTorqueL);
		CalcBrakeTorque();
		SetWheelFeedback();
		PrintDebug();
	}
}

void UACVehiclePhysics::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

namespace
{
	static constexpr double RPM_TO_RAD_PER_SEC = PI / 30.0;          // ω = RPM * 2π / 60 = π / 30
}

void UACVehiclePhysics::ConstructVehiclePhysics()
{
	ClearArrays();
	if (IsAxisCountEqualToConfig()) {
		CreateAxis();
		MechanicalData.EngineData.CurrentTemperature = GetWorldTemperature();

		SplitAxisReferences();
		InitTransmissionRatios();
		InitBrakeRatios();
		CreateWheelMeshes();
	}
}

TArray<FName> UACVehiclePhysics::GetAxisSockets()
{
	TArray<FName> AxisSocketsL = TArray<FName>();
	AVehicle* Vehicle = GetVehicle();

	USkeletalMeshComponent* BodyMesh = Vehicle->VehicleBody;
	if (!IsValid(BodyMesh))
	{
		return AxisSocketsL;
	}

	// Get all socket names on the body
	TArray<FName> AllSocketNames = BodyMesh->GetAllSocketNames();

	// For each axis, if the axis name exists as a socket, add it
	for (const FSAxis& Axis : MechanicalData.AxisData)
	{
		if (Axis.AxisName != NAME_None && AllSocketNames.Contains(Axis.AxisName))
		{
			AxisSocketsL.Add(Axis.AxisName);
		}
	}

	return AxisSocketsL;
}

AVehicle* UACVehiclePhysics::GetVehicle() const
{
	return Cast<AVehicle>(GetOwner());
}

void UACVehiclePhysics::SetSteeringInput(double Steering)
{
	// ------------------------------------------------------------
	// Smooth / raw steering selection (SelectFloat + FInterpTo)
	// ------------------------------------------------------------
	float Dt = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
	float Smoothed = FMath::FInterpTo(SteeringValue, Steering, Dt, 7.5f);
	SteeringValue = SmoothSteering ? (double)Smoothed : Steering;

	// ------------------------------------------------------------
	// ForEach Axis in AxisArray
	// ------------------------------------------------------------
	for (USCAxis* AxisObj : AxisArray)
	{
		if (!AxisObj) continue;

		const FSAxis& Axis = AxisObj->AxisSetup;       // Break SAxis
		if (!Axis.IsSteerAxis)                         // Branch(IsSteerAxis)
		{
			continue;
		}

		// --------------------------------------------------------
		// Build rotation target: SteeringValue * MaxRotationAngle
		// Clamp to [-MaxRotationAngle, +MaxRotationAngle]
		// (Multiply_DoubleDouble + FClamp with -MaxRot and +MaxRot)
		// --------------------------------------------------------
		const float MaxRot = Axis.MaxRotationAngle;
		double RotationAngle = FMath::Clamp(SteeringValue * MaxRot, -MaxRot, MaxRot);

		FName AxisName = Axis.AxisName;
		float AxisWidth = Axis.AxisWidth;

		// --------------------------------------------------------
		// ForEach Suspension (SCWheel) on this axis
		// --------------------------------------------------------
		for (USCWheel* Wheel : AxisObj->Suspensions)
		{
			if (!Wheel) continue;

			const FSAxis WheelAxisSetup = Wheel->GetAxisSetup();
			const FName WheelAxisName = WheelAxisSetup.AxisName; // GetAxisSetup → Break SAxis.AxisName
			if (WheelAxisName != AxisName)// EqualEqual_NameName
				continue;

			if (AckermannAccuracy)
			{
				SetSteeringValueByAckermannAccuracy(Wheel, AxisName, RotationAngle, AxisWidth);
			}
			else
			{
				Wheel->SteeringAngle = GetSteeringAngle(RotationAngle, Wheel);
			}
		}
	}
}

void UACVehiclePhysics::SetThrottleInput(double Throttle)
{
	// --- Break SMechanicalData / Break SEngine ---
	bool bEngineStarted = MechanicalData.EngineData.IsEngineStarted;

	// --- If engine not started → Start Engine ---
	if (!bEngineStarted)
	{
		if (Throttle > 0)
			StartEngine();
	}

	// --- Speed in km/h (Get Forward Speed * 0.036) + ABS node ---
	double ThrottleOnGearchange = Throttle * static_cast<double>(!GearChange);

	if (MechanicalData.GearBoxData.GearBoxType == EGearBoxType::FullAuto)
	{
		bool bReverse = (GetCurrentGear() < 0);
		if (bReverse)
		{
			float SpeedCmS = GetForwardSpeed();               // your function returning cm/s
			float SpeedKmh = SpeedCmS * 0.036f;   // ABS + * 0.036
			if (SpeedKmh > 1)
			{
				SetBrakeInput(ThrottleOnGearchange);
			}
			else
			{
				if (GetCurrentGear() != 1 and ThrottleOnGearchange > 0) {
					SetGear(1);
				}
				else {
					if (GearChange) {
						ThrottleValue = SmoothThrottle ?
							FMath::FInterpTo(ThrottleValue, ThrottleOnGearchange, GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f, 10.f)
							:
							ThrottleOnGearchange;
					}
				}
			}
		}
		else
		{
			float SpeedCmS = GetForwardSpeed();               // your function returning cm/s
			float SpeedKmh = FMath::Abs(SpeedCmS * 0.036f);   // ABS + * 0.036
			if (SpeedKmh < 0.001) {
				if (GetCurrentGear() != 1 and ThrottleOnGearchange > 0) {
					SetGear(1);
				}
				else {
					if (GearChange) {
						ThrottleValue = SmoothThrottle ?
							FMath::FInterpTo(ThrottleValue, ThrottleOnGearchange, GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f, 10.f)
							:
							ThrottleOnGearchange;
					}
				}
			}
			else {
				if (GearChange) {
					ThrottleValue = SmoothThrottle ?
						FMath::FInterpTo(ThrottleValue, ThrottleOnGearchange, GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f, 10.f)
						:
						ThrottleOnGearchange;
				}
			}
		}
	}
	else
	{
		if (GearChange)
		{
			ThrottleValue = 0;
		}
		else
		{
			ThrottleValue = SmoothThrottle ? 
			FMath::FInterpTo(ThrottleValue, ThrottleOnGearchange, GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f, 50.f)
			:
			ThrottleOnGearchange;
		}
	}

}

void UACVehiclePhysics::StartEngine()
{
	if (bStartEngineDoOnceGate) {
		bStartEngineDoOnceGate = false;

		//Block ignition if no juice. But play ignition sound.
		CanIgnite = MechanicalData.FuelData.FuelLeft > 0;
		if (CanIgnite) {
			if (MechanicalData.EngineData.EngineHealth > 0) {
				if (!MechanicalData.EngineData.IsEngineStarted) {
					EngineStarted.Broadcast();
					//TODO: Delay as much as ignition sound length (Maybe)
					MechanicalData.EngineData.IsEngineStarted = true;
					bStopEngineDoOnceGate = true;
				}
			}
			else {
				//Can't start car if engine health is not more than 0. But play ignition sound. And set CanIgnite true for next attempt.
				PlayEngineIgniteSound.Broadcast();
				CanIgnite = true;
				bStartEngineDoOnceGate = true;
			}
		}
		else {
			//Can't start car if engine fuel left is not more than 0. But play ignition sound. And set CanIgnite true for next attempt.
			PlayEngineIgniteSound.Broadcast();
			CanIgnite = true;
			bStartEngineDoOnceGate = true;
		}
	}
}

void UACVehiclePhysics::StopEngine()
{
	if (bStopEngineDoOnceGate) {
		bStopEngineDoOnceGate = false;
		if (MechanicalData.EngineData.IsEngineStarted) {
			MechanicalData.EngineData.IsEngineStarted = false;
			ThrottleValue = 0.0;
			BrakeValue = 0.0;
			SetNitrous(false);
			EngineStopped.Broadcast();
			CanIgnite = true;
			bStartEngineDoOnceGate = true;
		}
	}
}

void UACVehiclePhysics::FixEngine()
{
	bKillEngineDoOnceGate = true;
	MechanicalData.EngineData.CurrentTemperature = MechanicalData.EngineData.AvgTemp;
	MechanicalData.EngineData.EngineHealth = 100.0;
}

void UACVehiclePhysics::KillEngine()
{
	if (bKillEngineDoOnceGate) {
		bKillEngineDoOnceGate = false;
		//Engine is dead.
		EngineExploded.Broadcast();
		StopEngine();
	}
}

void UACVehiclePhysics::SetBrakeInput(double Brake)
{
	// UE velocities are cm/s. Your graph multiplies |speed_cm_per_s| by 0.036 (cm/s -> km/h).
	const double speedKmh = FMath::Abs(GetForwardSpeed()) * 0.036;     // Abs(x)*0.036 nodes
	const bool speedGT1Kmh = (speedKmh > 1.0);               // > 1.0 check
	const bool speedLT_0p001 = (speedKmh < 0.001);             // < 0.001 check (near-still)

	int32 gear = GetCurrentGear();
	const bool autoBox = (MechanicalData.GearBoxData.GearBoxType == EGearBoxType::FullAuto);

	auto smoothPick = [&](double current, double target)->double
		{
			// SelectFloat( bPickA = SmoothBrake, A = FInterpTo(...), B = target )
			if (SmoothBrake)
			{
				const float v = FMath::FInterpTo(
					static_cast<float>(current),
					static_cast<float>(target),
					GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f,
					7.5f); // your InterpSpeed pins
				return static_cast<double>(v);
			}
			return target;
		};

	// ------------------- Branching mirrors -------------------
	if (autoBox) // K2Node_EnumEquality == FullAuto
	{
		if (gear > 1) // Greater_IntInt (A=gear, B=1)
		{
			if (speedGT1Kmh) // Greater_DoubleDouble (Abs*0.036 > 1.0)
			{
				// then: set BrakeValue (maybe smoothed) and IsBraking
				BrakeValue = smoothPick(BrakeValue, Brake);
				IsBraking = (BrakeValue > 0.0);
				return;
			}
			// else -> fall through to the small-speed gate below (Knot_36 -> IfThenElse_15)
		}
		else // gear <= 1  (Else of Greater_IntInt)
		{
			if (!speedLT_0p001) // If speed is NOT tiny, we forward the axis to throttle
			{
				// Else branch of IfThenElse_13 goes directly to SetThrottleInputX
				SetThrottleInput(Brake);         // K2Node_CallFunction_59 (Throttle <- Brake)
				return;
			}
			// else speed ~ 0 -> fall through to gate below (IfThenElse_15)
		}

		const bool GearNotReverse = (gear != -1);
		const bool BrakeInputPositive = (Brake > 0.0);
		if (GearNotReverse && BrakeInputPositive)
		{
			//shift to Reverse immediately (this is what the BP implies)
			SetGear(-1);
			return;
		}
		else
		{
			// Else wires join the throttle call path
			SetThrottleInput(Brake);                           // pass the axis through
			return;
		}
	}
	else
	{
		// Non-FullAuto: just set BrakeValue (smoothed optional) and IsBraking
		BrakeValue = smoothPick(BrakeValue, Brake);
		IsBraking = (BrakeValue > 0.0);
		return;
	}
}

void UACVehiclePhysics::SetClutchInput(double Clutch)
{
	// SmoothClutch ? FInterpTo(ClutchValue -> Clutch) : Clutch
	const double Target =
		SmoothClutch
		? static_cast<double>(FMath::FInterpTo(
			static_cast<float>(ClutchValue),
			static_cast<float>(Clutch),
			GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f,
			7.5f))
		: Clutch;

	ClutchValue = FMath::Clamp(Target, 0.0, 1.0);
}

void UACVehiclePhysics::CreateAxis()
{
	AVehicle* Vehicle = GetVehicle();
	if (!IsValid(Vehicle))
		return;

	for (auto& AxisData : MechanicalData.AxisData)
	{
		USCAxis* NewAxis = Vehicle->CreateAxis(AxisData);
		int AxisArrayIndex = AxisArray.Add(NewAxis);
		//NewAxis->AxisDriveTorqueRatio = AxisDriveTorqueRatios[AxisArrayIndex];
	}

	for (auto& Axis : AxisArray)
	{
		if (IsValid(Axis))
		{
			Axis->Construct();
		}
	}
}

void UACVehiclePhysics::SetHandbrake(bool bNewHandbrake)
{
	HandbrakeValue = bNewHandbrake;
}

void UACVehiclePhysics::SetLoad()
{
	Load = UKismetMathLibrary::FInterpTo(Load, ThrottleValue > 0 ? 1 : 0, GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f, 2.5f);
	#if !(UE_BUILD_SHIPPING)
	
	const FText LoadText = FText::Format(
	NSLOCTEXT("ACVehiclePhysics", "PrintDebug_Load", "Load = {0}"), Load);

	UKismetSystemLibrary::PrintText(
		this, LoadText,
		/*bPrintToScreen*/ true,
		/*bPrintToLog*/   false,
		/*TextColor*/     FLinearColor(1.f, 0.08304f, 0.f, 1.f), // (R=1,G=0.08304,B=0,A=1)
		/*Duration*/      0.f,
		/*Key*/           NAME_None
	);
	#endif
}

void UACVehiclePhysics::CalcTurbo(double TurboMultiplier)
{
	if (MechanicalData.EngineData.IsEngineStarted) {
		if (ThrottleValue <= 0) {
			//Off Load
			if (CanTurboDecrease) {
				//DECREASE TO NEGATIVE CAPACITY
				if (CanTurboDecrease)
				{
					// Pull references from your mechanical data

					const double Capacity = static_cast<double>(MechanicalData.TurboData.TurboPressureCapacity); // float -> double
					const double DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);
					const double Decrease = DeltaSeconds * Capacity * 5.0;

					// TurboPressure (float) math in double, then clamp and write back
					const double Current = static_cast<double>(MechanicalData.TurboData.TurboPressure);
					const double NewPressureD = FMath::Clamp(Current - Decrease, -Capacity, +Capacity);
					MechanicalData.TurboData.TurboPressure = static_cast<float>(NewPressureD);

					// If we've reached the negative capacity, stop further decreasing
					if (NewPressureD <= -Capacity)
					{
						CanTurboDecrease = false;
					}

					// Mirror the Blueprint call
					SetTurboLoadRatioAndTriggerSound(TurboMultiplier);
				}
				else
				{
					SetTurboLoadRatioAndTriggerSound(TurboMultiplier);
				}
			}
			else {
				//If Turbo Pressure reached negative pressure capacity, now increase to 0.
				MechanicalData.TurboData.TurboPressure =
					FMath::Clamp(
						(GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f) * 
						2.5f * 
						MechanicalData.TurboData.TurboPressureCapacity +
				MechanicalData.TurboData.TurboPressure, 
				MechanicalData.TurboData.TurboPressure * -1, 0);

				if (MechanicalData.TurboData.TurboPressure) {
					CanTurboDecrease = false;
				}

				SetTurboLoadRatioAndTriggerSound(TurboMultiplier);
			}
		}
		else {
			//On Load
			double turboTarget = MechanicalData.TurboData.TurboPressure +
				(GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f) *
				ThrottleValue *
				(MechanicalData.TurboData.TurboPressure < 0 ? 5 : 1) *
				MechanicalData.TurboData.TurboPressureCapacity;
			TempTurboPressure = FMath::Clamp(turboTarget, MechanicalData.TurboData.TurboPressureCapacity * -1, MechanicalData.TurboData.TurboPressureCapacity);

			MechanicalData.TurboData.TurboPressure = TempTurboPressure;
			CanTurboDecrease = true;

			SetTurboLoadRatioAndTriggerSound(TurboMultiplier);
		}
	}
}

void UACVehiclePhysics::CalcFuelConsumption(double ConsumptionMultiplier /*= 1.0*/)
{
	// === Inputs/state used by the Blueprint ===
	// MechanicalData (contains EngineData + FuelData)
	// CurrentFuelEnergy (Joules consumed by powertrain this frame/tick)
	// Outputs/side effects:
	//   CanIgnite (bool)
	//   FuelConsumption (double)  // units: cm^3 consumed this tick
	//   MechanicalData.FuelData.FuelLeft is decreased

	// CanIgnite = (FuelLeft > 0)
	CanIgnite = (MechanicalData.FuelData.FuelLeft > 0.0);

	if (!MechanicalData.EngineData.IsEngineStarted)
	{
		FuelConsumption = 0.0;
		return;
	}

	// --- Curve factor based on RPM ---
	const double rpm = MechanicalData.EngineData.CurrentRPM;
	const double maxRPM = FMath::Max(1.0, MechanicalData.EngineData.MaxRPM); // avoid div-by-zero
	const float x = static_cast<float>(rpm / maxRPM);                  // RPM normalized
	const float curveVal = (MechanicalData.FuelData.FuelConsumptionCurve)
		? MechanicalData.FuelData.FuelConsumptionCurve->GetFloatValue(x)
		: 1.0f;

	// --- Fuel energy density (Wh/L) select by fuel type ---
	auto EnergyDensity_WhPerL = [&]() -> double
		{
			switch (MechanicalData.FuelData.FuelType)
			{
			case EFuelType::Electricity: return  500.0;
			case EFuelType::Gasoline:    return 9500.0;
			case EFuelType::Diesel:      return 11000.0;
			case EFuelType::LPG:         return 7000.0;
			default:                     return 0.0;
			}
		}();

	// Convert Wh/L -> J/L (×3600). The BP also multiplies by 5.0 (kept as-is).
	double energy_J_per_L = EnergyDensity_WhPerL * 3600.0 * 5.0;

	// Convert J/L -> J/cm^3 (÷1000)
	double energy_J_per_cm3 = energy_J_per_L / 1000.0;

	// cm^3 consumed from the fuel this tick, based on drivetrain energy draw:
	//   Volume(cm^3) = Energy(J) / (J/cm^3)
	double volumeCm3FromEnergy = (energy_J_per_cm3 > 0.0)
		? (CurrentFuelEnergy / energy_J_per_cm3)
		: 0.0;

	// Engine-type factor (matches the Select node values)
	auto EngineFactor = [&]() -> double
		{
			switch (MechanicalData.EngineData.EngineType)
			{
			case EEngineType::I2:  return  1.8;
			case EEngineType::V2:  return  2.0;
			case EEngineType::I3:  return  3.0;
			case EEngineType::I4:  return  3.8;
			case EEngineType::V4:  return  4.0;
			case EEngineType::I5:  return  4.8;
			case EEngineType::I6:  return  5.8;
			case EEngineType::V6:  return  6.0;
			case EEngineType::V8:  return  8.0;
			case EEngineType::V10: return 10.0;
			case EEngineType::V12: return 12.0;
			default:               return  1.0;
			}
		}();

	const double engineVolumeL = MechanicalData.EngineData.EngineVolume; // L
	const double dt = (GetWorld()) ? GetWorld()->GetDeltaSeconds() : 0.0;

	// FuelConsumption (cm^3) = Multiplier * (cm^3 from energy) * EngineVolume(L) * dt * EngineFactor * Curve(RPM)
	FuelConsumption = ConsumptionMultiplier
		* volumeCm3FromEnergy
		* engineVolumeL
		* dt
		* EngineFactor
		* static_cast<double>(curveVal);

	// Update FuelLeft (Liters): (L*1000 - FuelConsumption(cm^3)) / 1000
	const double prevFuelL = MechanicalData.FuelData.FuelLeft;
	double       newFuelL = (prevFuelL * 1000.0 - FuelConsumption) / 1000.0;

	// Blueprint checks (newFuelL <= 0) and branches; we clamp and reflect the state.
	if (newFuelL <= 0.0)
	{
		newFuelL = 0.0;
		CanIgnite = false;              // out of fuel ⇒ cannot ignite
		// (The BP's Then branch calls another function; if you have an EngineStop(), call it here.)
		// EngineStop(); // ← optional if your class has it
	}

	MechanicalData.FuelData.FuelLeft = newFuelL;
}

void UACVehiclePhysics::TriggerBackfires()
{
	for (double BackFireRpm : BackfireRPMs) {

		if (BackFireRpm >= MechanicalData.EngineData.CurrentRPM * 0.99 && 
			BackFireRpm <= MechanicalData.EngineData.CurrentRPM * 1.01 && 
			ThrottleValue <= 0.1)
		{
			TriggerBackfire.Broadcast();
		}
	}
}

void UACVehiclePhysics::SetGearUp()
{
	if (MechanicalData.GearBoxData.GearBoxType != EGearBoxType::FullAuto) 
	{
		if (MechanicalData.GearBoxData.GearRatios.Num() - 1 != CurrentGear) 
		{
			//clamp to max gear

			SetGear(FMath::Clamp(CurrentGear + 1, 0, MechanicalData.GearBoxData.GearRatios.Num() - 1));
		}
	}
}

void UACVehiclePhysics::SetGearDown()
{
	if (MechanicalData.GearBoxData.GearBoxType != EGearBoxType::FullAuto) 
	{
		if (CurrentGear != 0) 
		{
			//clamp to min gear
			SetGear(FMath::Clamp(CurrentGear - 1, 0, MechanicalData.GearBoxData.GearRatios.Num() - 1));
		}
	}
}

void UACVehiclePhysics::SetGear(int32 InCurrentGear)
{
	// 1) Drop load & mark as changing
	Load = 0.0;                 // K2Node_VariableSet_15 (Load = 0.0)
	GearChange = true;          // K2Node_VariableSet_2  (GearChange = true)

	// 2) Compute per-half delay: GearChangeLatency / 2
	const double Latency = static_cast<double>(MechanicalData.GearBoxData.GearChangeLatency);
	const double HalfLatency = FMath::Max(0.0, Latency / 2.0);

	// Timers replicate the two Delay nodes
	// t = HalfLatency: set TargetGear and go Neutral (BP sets CurrentGear = 1)
	FTimerHandle TempHandle1_;
	GetWorld()->GetTimerManager().SetTimer(
		/*OutHandle*/ TempHandle1_,
		FTimerDelegate::CreateWeakLambda(this, [this, InCurrentGear]()
			{
				TargetGear = InCurrentGear;   // K2Node_VariableSet_12(TargetGear = InCurrentGear)
				CurrentGear = 1;              // K2Node_VariableSet_0  ("Switch to N" in comment; value = 1)
			}),
		HalfLatency, /*bLoop*/ false
	);
	FTimerHandle TempHandle2_;
	// t = 2*HalfLatency: commit gear, fire event, clear flag
	GetWorld()->GetTimerManager().SetTimer(
		/*OutHandle*/ TempHandle2_,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				CurrentGear = TargetGear;     // K2Node_VariableSet_10 (CurrentGear = TargetGear)
				GearChanged.Broadcast(CurrentGear); // K2Node_CallDelegate_7
				GearChange = false;           // K2Node_VariableSet_1 (false)
			}),
		HalfLatency * 2.0, /*bLoop*/ false
	);
}

void UACVehiclePhysics::SetUseAutoGearBox(bool bIsUsingAutoGearBox)
{
	UseAutoGearBox = bIsUsingAutoGearBox;
}

void UACVehiclePhysics::CreateWheelMeshes()
{
	AVehicle* Vehicle = GetVehicle();

	if (!IsValid(Vehicle))
		return;

	TArray<USCWheel*> SuspensionArray = GetSuspensions();

	// For each suspension, create the wheel mesh and add result to WheelMeshArray
	for (USCWheel* Suspension : SuspensionArray)
	{
		if (!IsValid(Suspension))
			continue;

		USkeletalMeshComponent* WheelMesh = Vehicle->CreateWheelMesh(Suspension);

		WheelMeshArray.Add(WheelMesh);
	}
}

void UACVehiclePhysics::ClearArrays()
{
	for(auto WheelMesh : WheelMeshArray)
	{
		if (IsValid(WheelMesh))
		{
			WheelMesh->DestroyComponent();
		}
	}

	WheelMeshArray.Empty();

	for(auto Axis : AxisArray)
	{
		if (IsValid(Axis))
		{
			Axis->DestroyComponent();
		}
	}

	AxisArray.Empty();
}

void UACVehiclePhysics::ReFuel(double LitersPerSecond /*= 1.0*/)
{
	// If engine is running, warn and bail
	if (MechanicalData.EngineData.IsEngineStarted)
	{
		UKismetSystemLibrary::PrintString(
			this,
			TEXT("Stop engine first."),
			/*bPrintToScreen=*/true,
			/*bPrintToLog=*/true,
			FLinearColor::Red,
			/*Duration=*/0.0f
		);
		return;
	}

	const double DeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	// === Math mirrors the Blueprint ===
	// Graph does: (LPS * 1000) * DeltaSeconds * 5  -> add to (FuelLeft * 1000)
	// then divide by 1000 and clamp to capacity.
	// Simplified: add liters directly: LPS * Delta * 5, clamp to capacity.

	FSFuel& Fuel = MechanicalData.FuelData;

	const double LitersToAdd = LitersPerSecond * DeltaSeconds * 5.0;  // 5x scale from the graph
	Fuel.FuelLeft = FMath::Clamp(Fuel.FuelLeft + LitersToAdd, 0.0, Fuel.FuelCapacity);

	// CanIgnite := FuelLeft > 0
	CanIgnite = (Fuel.FuelLeft > 0.0);

	// Debug print (Blueprint prints in cm^3)
	const double ImportCm3PerSec = LitersPerSecond * 1000.0;
	const double FuelLeftCm3 = Fuel.FuelLeft * 1000.0;

	const FText Msg = FText::Format(
		NSLOCTEXT("ACVehiclePhysicsX", "ReFuelXDebug",
			"Fuel Import = {0} cm^3/s\nCurrent Fuel Left = {1} cm^3"),
		FText::AsNumber(ImportCm3PerSec),
		FText::AsNumber(FuelLeftCm3)
	);

	UKismetSystemLibrary::PrintText(
		this,
		Msg,
		/*bPrintToScreen=*/true,
		/*bPrintToLog=*/false,
		FLinearColor(0.0f, 0.66f, 1.0f, 1.0f),
		/*Duration=*/0.0f
	);
}

void UACVehiclePhysics::CalcNitrous(double NitrousPowerMultiplier)
{
	// 1) Sample curve at current RPM
	double Rpm = GetEngineRpm();

	float CurvePct = 0.f; // percent from curve (e.g., 0..50)
	if (MechanicalData.NitroData.NitroCurve)
	{
		CurvePct = MechanicalData.NitroData.NitroCurve->GetFloatValue(Rpm);
	}

	const bool CanBoost = bIsNitroBeingUsed && (MechanicalData.NitroData.NitroLeft > 0.f);

	// 2) Compute Boost multiplier (1 + multiplier * percent/100), or 1 when off
	const float Boost = CanBoost ? 1.f + NitrousPowerMultiplier * (CurvePct * 0.01f) : 1.f;

	NitroBoostMultiplier = Boost;

	// 3) Consume nitro while Boosting
	if (CanBoost)
	{
		const float Rate = 1; //NitroConsumptionRate; L/s
		MechanicalData.NitroData.NitroLeft = FMath::Clamp(
			MechanicalData.NitroData.NitroLeft - Rate * GetWorld()->GetDeltaSeconds(),
			0.0f,
			MechanicalData.NitroData.NitroCapacity
		);

		// Optional: auto-stop when empty
		if (MechanicalData.NitroData.NitroLeft <= KINDA_SMALL_NUMBER)
		{
			bIsNitroBeingUsed = false;
		}
	}
}

void UACVehiclePhysics::SetNitrous(bool bNewNitrous)
{
	if (MechanicalData.EngineData.IsEngineStarted) {
		if (bNewNitrous) {
			// Activate nitro only if CurrentRPM is above 15% of MaxRPM and throttle is pressed
			if (MechanicalData.EngineData.CurrentRPM > MechanicalData.EngineData.MaxRPM * 0.15 && ThrottleValue > 0) {
				bIsNitroBeingUsed = true;
				TriggerNitrous.Broadcast(true);
			}
		}
		else {
			bIsNitroBeingUsed = false;
			TriggerNitrous.Broadcast(false);
		}
	}
	else {
		bIsNitroBeingUsed = false;
		TriggerNitrous.Broadcast(false);
	}
}

void UACVehiclePhysics::InitTransmissionRatios()
{
	// BP: Break SMechanicalData → Break STransmission
	const float SplitClamped = FMath::Clamp(MechanicalData.TransmissionData.AWDDriveSplit, 0.f, 1.f);

	const int32 FrontAxisCount = FrontAxis.Num(); // BP: LENGTH(Front Axis X)
	const int32 RearAxisCount = RearAxis.Num();  // BP: LENGTH(Rear  Axis X)

	// This array mirrors your BP "ADD → SET Axis Drive Torque Ratios"
	AxisDriveTorqueRatios.Reset();
	AxisDriveTorqueRatios.Reserve(FrontAxisCount + RearAxisCount);

	switch (MechanicalData.TransmissionData.DriveType)
	{
		case EDriveType::FWD:
		{
			// If DriveType is FWD:
			//   Set Front Axis ratios by splitting ratios by Front Axis Count.
			//   Set Rear Axis Torque Ratios to 0.
			const float FrontPerAxis = (FrontAxisCount > 0) ? (1.f / float(FrontAxisCount)) : 0.f;

			// ForEach Front Axis → SET + ADD
			for (USCAxis* Axis : FrontAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = FrontPerAxis;
				AxisDriveTorqueRatios.Add(FrontPerAxis);
			}

			// ForEach Rear Axis → SET 0 + ADD 0
			for (USCAxis* Axis : RearAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = 0.f;
				AxisDriveTorqueRatios.Add(0.f);
			}
		}
		break;

		case EDriveType::RWD:
		{
			// If DriveType is RWD:
			//   Set Front Axis Torque Ratios to 0.
			//   Set Rear Axis ratios by splitting ratios by Rear Axis Count.
			const float RearPerAxis = (RearAxisCount > 0) ? (1.f / float(RearAxisCount)) : 0.f;

			// ForEach Front Axis → SET 0 + ADD 0
			for (USCAxis* Axis : FrontAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = 0.f;
				AxisDriveTorqueRatios.Add(0.f);
			}

			// ForEach Rear Axis → SET + ADD
			for (USCAxis* Axis : RearAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = RearPerAxis;
				AxisDriveTorqueRatios.Add(RearPerAxis);
			}
		}
		break;

		case EDriveType::AWD:
		default:
		{
			// If DriveType is AWD:
			//   Split Transmission by AWDFrontRearSplit ratio.
			//   (AWDFrontRearSplit) / FrontAxisCount  → front per-axis
			//   (1 - AWDFrontRearSplit) / RearAxisCount → rear per-axis
			const float FrontPerAxis = (FrontAxisCount > 0) ? (SplitClamped / float(FrontAxisCount)) : 0.f;
			const float RearPerAxis = (RearAxisCount > 0) ? ((1.f - SplitClamped) / float(RearAxisCount)) : 0.f;

			// ForEach Front Axis → SET + ADD
			for (USCAxis* Axis : FrontAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = FrontPerAxis;
				AxisDriveTorqueRatios.Add(FrontPerAxis);
			}

			// ForEach Rear Axis → SET + ADD
			for (USCAxis* Axis : RearAxis)
			{
				if (!IsValid(Axis)) continue;
				Axis->AxisDriveTorqueRatio = RearPerAxis;
				AxisDriveTorqueRatios.Add(RearPerAxis);
			}
		}
		break;
	}
}

void UACVehiclePhysics::PrintDebug()
{
#if !(UE_BUILD_SHIPPING)

	// ---------- Block 1 General Data----------
	// Current gear name (safe index)
	FName GearName = NAME_None;
	if (MechanicalData.GearBoxData.GearRatios.IsValidIndex(CurrentGear))
	{
		GearName = MechanicalData.GearBoxData.GearRatios[CurrentGear].GearName;
	}

	double WT = GetWorldTemperature();

	double VT = MechanicalData.EngineData.CurrentTemperature;
	double EH = MechanicalData.EngineData.EngineHealth;
	double Kmh = CmsToKmh(GetOwner() ? GetOwner()->GetVelocity().Size() : 0.0);

	FNumberFormattingOptions TwoDec;
	TwoDec.MinimumFractionalDigits = 2;
	TwoDec.MaximumFractionalDigits = 2;

	FNumberFormattingOptions SpeedFmt;
	SpeedFmt.MinimumIntegralDigits = 3;
	SpeedFmt.MinimumFractionalDigits = 0;
	SpeedFmt.MaximumFractionalDigits = 0;

	FFormatNamedArguments A1;
	A1.Add(TEXT("CG"), FText::FromName(GearName));
	A1.Add(TEXT("WT"), FText::AsNumber(WT, &TwoDec));
	A1.Add(TEXT("VT"), FText::AsNumber(VT, &TwoDec));
	A1.Add(TEXT("EH"), FText::AsNumber(EH, &TwoDec));
	A1.Add(TEXT("VS"), FText::AsNumber(Kmh, &SpeedFmt));

	const FText T1 = FText::Format(
		NSLOCTEXT("VehiclePhysics", "PrintDebug_1",
			"Current Gear = {CG}\n"
			"World Temperature = {WT} C\n"
			"Vehicle Temperature = {VT} C\n"
			"Engine Health = {EH} %\n"
			"Vehicle Speed = {VS} Km/h"),
		A1);

	UKismetSystemLibrary::PrintText(
		this, T1,
		/*bPrintToScreen*/ true,
		/*bPrintToLog*/   false,
		/*TextColor*/     FLinearColor(1.f, 0.f, 0.965831f, 1.f),
		/*Duration*/      0.f,
		/*Key*/           NAME_None
	);

	// ---------- Block 2 ----------
	double CT = ClutchTorque;
	double CL = ClutchLock;
	double CRPM = RadSToRpm(ClutchDiskAngularVelocity);
	double TTT = TransmissionTorque;

	FNumberFormattingOptions ZeroDec3Int; // CT
	ZeroDec3Int.MinimumIntegralDigits = 3;
	ZeroDec3Int.MinimumFractionalDigits = 0;
	ZeroDec3Int.MaximumFractionalDigits = 0;

	FNumberFormattingOptions TwoDecAgain; // CL
	TwoDecAgain.MinimumFractionalDigits = 2;
	TwoDecAgain.MaximumFractionalDigits = 2;

	FNumberFormattingOptions RpmFmt; // CRPM
	RpmFmt.MinimumIntegralDigits = 3;
	RpmFmt.MinimumFractionalDigits = 0;
	RpmFmt.MaximumFractionalDigits = 2;

	FNumberFormattingOptions TwoDec3Int; // TTT
	TwoDec3Int.MinimumIntegralDigits = 3;
	TwoDec3Int.MinimumFractionalDigits = 2;
	TwoDec3Int.MaximumFractionalDigits = 2;

	FFormatNamedArguments A2;
	A2.Add(TEXT("CT"), FText::AsNumber(CT, &ZeroDec3Int));
	A2.Add(TEXT("CL"), FText::AsNumber(CL, &TwoDecAgain));
	A2.Add(TEXT("CRPM"), FText::AsNumber(CRPM, &RpmFmt));
	A2.Add(TEXT("TTT"), FText::AsNumber(TTT, &TwoDec3Int));

	const FText T2 = FText::Format(
		NSLOCTEXT("VehiclePhysics", "PrintDebug_2",
			"Clutch Torque = {CT} Nm\n"
			"Clutch Lock = {CL} k\n"
			"Clutch RPM = {CRPM} RPM\n"
			"Total Transmission Drive Torque = {TTT} Nm"),
		A2);

	UKismetSystemLibrary::PrintText(
		this, T2,
		/*bPrintToScreen*/ true,
		/*bPrintToLog*/   false,
		/*TextColor*/     FLinearColor(0.f, 1.f, 0.636643f, 1.f),
		/*Duration*/      0.f,
		/*Key*/           NAME_None
	);

	// ---------- Block 3: Engine stats (K2Node_FormatText_5 -> PrintText_61) ----------
	const FText EP = UKismetTextLibrary::Conv_DoubleToText(GetCurrentPower(), ERoundingMode::HalfToEven, /*AlwaysSign*/false, /*Grouping*/true,  /*MinInt*/1,   /*MaxInt*/324, /*MinFrac*/2, /*MaxFrac*/2);
	const FText ET = UKismetTextLibrary::Conv_DoubleToText(EngineTorque, ERoundingMode::HalfToEven, false, true, 3, 324, 0, 0);
	const FText FI = UKismetTextLibrary::Conv_DoubleToText(GetEngineInertia(), ERoundingMode::HalfToEven, false, true, 1, 324, 2, 3);
	const FText EAA = UKismetTextLibrary::Conv_DoubleToText(GetEngineAngularAcceleration(), ERoundingMode::HalfToEven, false, true, 3, 324, 0, 0);
	const FText EAD = UKismetTextLibrary::Conv_DoubleToText(GetEngineAngularDeceleration(), ERoundingMode::HalfToEven, false, true, 3, 324, 0, 0);
	const FText EAV = UKismetTextLibrary::Conv_DoubleToText(EngineAngularVelocity, ERoundingMode::HalfToEven, false, true, 3, 324, 0, 0);
	const FText ERPM = UKismetTextLibrary::Conv_DoubleToText(GetEngineRpm(), ERoundingMode::HalfToEven, false, true, 3, 324, 1, 1);

	const FText EngineText = FText::Format(
		NSLOCTEXT("ACVehiclePhysics", "PrintDebug_Engine",
			"Engine Power = {0} kW\n"
			"Engine Torque = {1} N.m\n"
			"Engine Inertia = {2} kgm^2\n"
			"Engine Angular Acceleration = {3} Rad/s^2\n"
			"Engine Angular Deceleration = {4} Rad/s^2\n"
			"Engine Angular Velocity = {5} Rad/s\n"
			"Current RPM = {6} RPM"),
		EP, ET, FI, EAA, EAD, EAV, ERPM
	);

	UKismetSystemLibrary::PrintText(
		this, EngineText,
		/*bPrintToScreen*/ true,
		/*bPrintToLog*/   false,
		/*TextColor*/     FLinearColor(1.f, 1.f, 0.f, 1.f),   // (R=1,G=1,B=0,A=1)
		/*Duration*/      0.f,
		/*Key*/           NAME_None
	);

	// ---------- Block 4: Fuel & energy (K2Node_FormatText_2 -> PrintText_113) ----------
	const FText FE = UKismetTextLibrary::Conv_DoubleToText(CurrentFuelEnergy, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);
	const FText FC = UKismetTextLibrary::Conv_DoubleToText(FuelConsumption, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 8);
	const double FuelLeftLiters = MechanicalData.FuelData.FuelLeft; // from BreakStruct path
	const FText FL = UKismetTextLibrary::Conv_DoubleToText(FuelLeftLiters, ERoundingMode::HalfToEven, false, true, 1, 324, 2, 8);
	const FText CKE = UKismetTextLibrary::Conv_DoubleToText(GetCurrentEngineKineticEnergy(), ERoundingMode::HalfToEven, false, true, 1, 324, 2, 2);

	const FText FuelText = FText::Format(
		NSLOCTEXT("ACVehiclePhysics", "PrintDebug_Fuel",
			"Current Fuel Energy = {0} Watt*s\n"
			"Current Fuel Consumption = {1} cm^3/s\n"
			"Current Fuel Left = {2} cm^3/s\n"
			"Current Kinetic Energy = {3} Watt*s"),
		FE, FC, FL, CKE
	);

	UKismetSystemLibrary::PrintText(
		this, FuelText,
		/*bPrintToScreen*/ true,
		/*bPrintToLog*/   false,
		/*TextColor*/     FLinearColor(1.f, 0.08304f, 0.f, 1.f), // (R=1,G=0.08304,B=0,A=1)
		/*Duration*/      0.f,
		/*Key*/           NAME_None
	);
#endif // !(UE_BUILD_SHIPPING)
}

void UACVehiclePhysics::CalcTransmissionTorque(double ClutchTorqueP)
{
	double TotalGearRatio = GetTotalGearRatio();
	double TotalDriveTorque = FMath::Max(ClutchTorque, 0.0) * TotalGearRatio;
	
	TransmissionTorque = TotalDriveTorque;

	for(auto Axis : AxisArray)
	{
		if (!IsValid(Axis)) 
			continue;
		
		Axis->SetAxisDriveTorque(TotalDriveTorque);
	}
}

void UACVehiclePhysics::SetWheelFeedback()
{
	ClutchDiskAngularVelocity = GetTotalTransmissionVelocity() * GetTotalGearRatio();
	//EngineAngularVelocity = GetTotalTransmissionVelocity() * GetTotalGearRatio();

	//EngineAngularVelocity - ClutchDiskAngularVelocity

	//GetTotalTractionTorque() - GetTotalFrictionTorque()
	
	//EngineAngularVelocity + GetTotalGearRatio() * GetTotalFrictionTorque() / GetEngineInertia() * CurrentGear != 1 * (GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f);

	//TransmissionTorque = FMath::Lerp(GetTotalFrictionTorque() * GetTotalGearRatio(), EngineTorque, ClutchLock);
	//ClutchTorque = TransmissionTorque;
}

double UACVehiclePhysics::GetTotalFrictionAcceleration()
{

	if (AxisArray.Num() < 2 || AxisDriveTorqueRatios.Num() < 2)
	{
		return 0.f;
	}
	double Inner0 = 0.f;
	double Inner1 = 0.f;

	auto AxisInnerSum = [](const USCAxis* Axis) -> double
		{
			if (!IsValid(Axis) || Axis->Suspensions.Num() < 2) 
				return 0.f;

			const USCWheel* S0 = Axis->Suspensions[0];
			const USCWheel* S1 = Axis->Suspensions[1];

			const double WheelInertia0 = Axis->Suspensions[0]->GetWheelInertia();
			const double WheelInertia1 = Axis->Suspensions[1]->GetWheelInertia();

			return Axis->Suspensions[0]->WheelFrictionTorque * WheelInertia0 + Axis->Suspensions[1]->WheelFrictionTorque * WheelInertia1;
		};

	Inner0 = AxisInnerSum(AxisArray[0]);
	Inner1 = AxisInnerSum(AxisArray[1]);

	// BP: (Inner0 * Ratio0) + (Inner1 * Ratio1)
	double WeightedSum =
		Inner0 * AxisDriveTorqueRatios[0] +
		Inner1 * AxisDriveTorqueRatios[1];

	// BP final: multiply by 0.5 and return (also assigned to "Total Friction Acceleration")
	return 0.5f * WeightedSum;
}

void UACVehiclePhysics::SplitAxisReferences()
{
	// BP: GetVehicle -> VehicleBody (SkeletalMeshComponent)
	AVehicle* Vehicle = GetVehicle();
	if (!Vehicle)
	{
		return;
	}

	// Expect a USkeletalMeshComponent named VehicleBody on Vehicle
	USkeletalMeshComponent* VehicleBody = Vehicle->VehicleBody;
	if (!VehicleBody)
	{
		return;
	}

	FrontAxis.Empty();
	RearAxis.Empty();

	// "Separate Front Axis from Rear Axis. (If X relative location is 0, consider as Front wheel.)"

	for (USCAxis* AxisRef : AxisArray)
	{
		if (!AxisRef) continue;

		// BP: Break SAxis -> AxisName, then GetSocketTransform(VehicleBody, AxisName, RTS_Component)
		// Access the AxisName from the axis’ setup. Adjust to your API if you use an accessor.
		FName AxisSocketName = AxisRef->AxisSetup.AxisName;

		// Get transform in Component space (RTS_Component), just like the BP pin
		const FTransform SocketXf = VehicleBody->GetSocketTransform(AxisSocketName, ERelativeTransformSpace::RTS_Component);
		const double X = static_cast<double>(SocketXf.GetLocation().X);

		// BP: Branch (GreaterEqual_DoubleDouble: X >= 0.0)
		if (X >= 0.0)
		{
			// BP: Array_Add -> FrontAxis
			FrontAxis.Add(AxisRef);
		}
		else
		{
			// BP: Array_Add -> RearAxis
			RearAxis.Add(AxisRef);
		}
	}

}

void UACVehiclePhysics::CalcBrakeTorque()
{
	for(USCAxis* Axis : AxisArray)
	{
		if (!IsValid(Axis)) continue;

		for (auto Suspension : Axis->Suspensions) 
		{
			if (!IsValid(Suspension))
				continue;

			Suspension->SetBrakeTorque(BrakeValue, HandbrakeValue);
		}
	}
}

void UACVehiclePhysics::InitBrakeRatios()
{
	// LENGTH(Front Suspensions)
	int32 FrontWheelCount = 0;
	for (const USCAxis* Axis : FrontAxis)
	{
		if (IsValid(Axis)) { FrontWheelCount += Axis->Suspensions.Num(); }
	}

	// LENGTH(Rear Suspensions)
	int32 RearWheelCount = 0;
	for (const USCAxis* Axis : RearAxis)
	{
		if (IsValid(Axis)) { RearWheelCount += Axis->Suspensions.Num(); }
	}

	double FrontPerWheel = 0.f;
	double RearPerWheel = 0.f;

	// Split & divide exactly like the BP comments
	switch (MechanicalData.TransmissionData.DriveType)
	{
		case EDriveType::FWD:
			// 100% front, 0% rear
			if (FrontWheelCount > 0) 
				FrontPerWheel = 1.f / float(FrontWheelCount);
			RearPerWheel = 0.f;
			break;

		case EDriveType::RWD:
			// 0% front, 100% rear
			FrontPerWheel = 0.f;
			if (RearWheelCount > 0) 
				RearPerWheel = 1.f / float(RearWheelCount);
			break;

		case EDriveType::AWD:
		default:
			// Exactly what your BP’s comment says:
			// Front gets SplitBias; Rear gets (1 - SplitBias), both divided by wheel count.
			const float Clamped = FMath::Clamp(MechanicalData.BrakeData.SplitBias, 0.f, 1.f);
			if (FrontWheelCount > 0) 
				FrontPerWheel = Clamped / float(FrontWheelCount);
			if (RearWheelCount > 0) 
				RearPerWheel = (1.f - Clamped) / float(RearWheelCount);
			break;
	}

	// Apply to each suspension
	for (USCAxis* Axis : FrontAxis)
	{
		if (!IsValid(Axis)) continue;
		for (USCWheel* S : Axis->Suspensions)
		{
			if (IsValid(S)) S->BrakeBias = FrontPerWheel;
		}
	}

	for (USCAxis* Axis : RearAxis)
	{
		if (!IsValid(Axis)) continue;
		for (USCWheel* S : Axis->Suspensions)
		{
			if (IsValid(S)) S->BrakeBias = RearPerWheel;
		}
	}
}

void UACVehiclePhysics::SetTurboLoadRatioAndTriggerSound(double TurboMultiplier)
{
	// if (ThrottleValue > 0)
	if (ThrottleValue > 0.0)
	{
		// --- TRUE branch: build turbo sound ratio & boost multiplier ---
		FSTurbo& Turbo = MechanicalData.TurboData;
		const FSEngine& Engine = MechanicalData.EngineData;

		const double TurboPressureAsDouble = static_cast<double>(Turbo.TurboPressure);
		const bool bTurboPressureLEZero = (TurboPressureAsDouble <= 0.0);

		// Select( cond = (TurboPressure <= 0) ? 0 : abs(TempTurboPressure) / TurboPressureCapacity )
		const double normTurbo =
			bTurboPressureLEZero
			? 0.0
			: FMath::Abs(TempTurboPressure) / FMath::Max(1e-6, static_cast<double>(Turbo.TurboPressureCapacity));

		// FClamp(value, 0.0, 0.95)
		const double Clamped = FMath::Clamp(normTurbo, 0.0, 0.95);

		// Save TurboSoundRatio (VariableSet)
		TurboSoundRatio = Clamped;

		// torqueBoostPercent = TorqueCurve(CurrentRPM) / 100.0
		double torqueBoostPercent = 0.0;
		if (Engine.TorqueCurve)
		{
			const float CurveVal = Engine.TorqueCurve->GetFloatValue(static_cast<float>(Engine.CurrentRPM));
			torqueBoostPercent = static_cast<double>(CurveVal) / 100.0;
		}

		// TurboBoostMultiplier = 1.0 + (Clamped * torqueBoostPercent * TurboMultiplier)
		TurboBoostMultiplier = 1.0 + (Clamped * torqueBoostPercent * TurboMultiplier);

		// SetFieldsInStruct(IsTurboReleased = false)
		Turbo.IsTurboReleased = false;

		// Reset the blow-off DoOnce gate so next release can trigger again
		bTurboBlowOffDoOnceGate = false;
	}
	else
	{
		// --- FALSE branch: trigger blow-off once when turbo wasn't released yet ---
		FSTurbo& Turbo = MechanicalData.TurboData;

		// Condition: Not_PreBool( Break(TurboData).IsTurboReleased )
		const bool bShouldTrigger = !Turbo.IsTurboReleased;

		if (bShouldTrigger)
		{
			// DoOnce -> CallDelegate( Round( MapRangeClamped( abs(TempTurboPressure)/Cap, 0..1 -> 0..3 ) ) )
			if (!bTurboBlowOffDoOnceGate)
			{
				const double BlowOffRatio = FMath::Abs(TempTurboPressure) / FMath::Max(1e-6, static_cast<double>(Turbo.TurboPressureCapacity));
				const double Mapped = FMath::GetMappedRangeValueClamped(
					FVector2D(0.0, 1.0),
					FVector2D(0.0, 3.0),
					BlowOffRatio
				);
				const int32 Stage = FMath::RoundToInt(Mapped);

				PlayTurboLoadSound.Broadcast(Stage);
				bTurboBlowOffDoOnceGate = true;         // macro “Completed”
			}

			// SetFieldsInStruct(IsTurboReleased = true)
			Turbo.IsTurboReleased = true;

			// VariableSet TurboSoundRatio (BP pin was unconnected -> defaults to 0.0)
			TurboSoundRatio = 0.0;

			// VariableSet TurboBoostMultiplier = 1.0
			TurboBoostMultiplier = 1.0;

			// Knot reset wired back to DoOnce.Reset
			bTurboBlowOffDoOnceGate = false;
		}
	}
}

void UACVehiclePhysics::SetSteeringValueByAckermannAccuracy(USCWheel* Suspension, FName SocketName, double CurrentRotationAngle, double AxisWidth)
{
	if (!Suspension)
	{
		return;
	}

	// Neutral: if the input steering value is zero, set the wheel’s steering to 0
	if (FMath::IsNearlyZero(SteeringValue))
	{
		Suspension->SteeringAngle = 0.0;
		return;
	}

	// --- Ackermann geometry prep ---
	double CurrentAndLastAxisDifference = DistanceOfCurrentAxisToLastAxis(SocketName);                         // Axis-to-axis distance
	double tanAbs = FMath::Tan(FMath::DegreesToRadians(FMath::Abs(CurrentRotationAngle))); // DegTan(abs(angle))

	// Guard divisions
	double ackermannAngleDeg = 0.0; // degrees
	if (!FMath::IsNearlyZero(tanAbs))
	{
		// R1 - w = D / tan(|angle|)
		double R1_minus_w = CurrentAndLastAxisDifference / tanAbs;

		// R1 = (R1 - w) + w
		double R1 = R1_minus_w + double(AxisWidth);

		// tan(ackermann) = D / (R1 + w)
		double denom = R1 + double(AxisWidth);
		double ratio = FMath::IsNearlyZero(denom) ? 0.0 : (CurrentAndLastAxisDifference / denom);

		// ackermann = DegAtan(ratio)
		ackermannAngleDeg = FMath::Atan(ratio);
	}

	// GetSteeringAngle(float Steering, USCWheel* Suspension) -> double
	double targetAngle = 0.0;

	if (SteeringValue < 0.0)
	{
		// Left turn: left wheel uses commanded angle, right wheel uses -Ackermann angle
		targetAngle = Suspension->bIsLeft
			? GetSteeringAngle(CurrentRotationAngle, Suspension)
			: GetSteeringAngle(-ackermannAngleDeg, Suspension);
	}
	else
	{
		// Right turn: left wheel uses +Ackermann angle, right wheel uses commanded angle
		targetAngle = Suspension->bIsLeft
			? GetSteeringAngle(+ackermannAngleDeg, Suspension)
			: GetSteeringAngle(CurrentRotationAngle, Suspension);
	}

	Suspension->SteeringAngle = targetAngle;
}

TArray<USCWheel*> UACVehiclePhysics::GetSuspensions()
{
	TArray<USCWheel*> OutSuspensions;
	OutSuspensions.Empty();
	int32 ReserveCount = 0;
	for (const USCAxis* Axis : AxisArray)
	{
		if (IsValid(Axis))
		{
			ReserveCount += Axis->Suspensions.Num();
		}
	}
	OutSuspensions.Reserve(ReserveCount);

	// Outer loop over axes
	for (const USCAxis* Axis : AxisArray)
	{
		if (!IsValid(Axis)) continue;

		// Inner loop over that axis' SuspensionsX
		for (USCWheel* S : Axis->Suspensions)
		{
			if (IsValid(S))
			{
				OutSuspensions.Add(S);
			}
		}
	}
	return OutSuspensions;
}

double UACVehiclePhysics::GetForwardSpeed()
{
	AVehicle* Vehicle = GetVehicle();
	if (!IsValid(Vehicle))
	{
		return Vehicle->GetVelocity().Size();
	}

	return 0.0;
}

double UACVehiclePhysics::CalcClutch(double InEngineTorque)
{
	const double AutoLock = FMath::GetMappedRangeValueClamped(FVector2D(1000, 1300), FVector2D(0, 1), GetEngineRpm());
	const double ManualLock = 1.0 - ClutchValue;
	
	//SelectFloat: if (GearBoxType != HShifter) pick AutoLock, else pick ManualLock
	const double LockCandidate = (MechanicalData.GearBoxData.GearBoxType != EGearBoxType::HShifter) ? AutoLock : ManualLock;

	//If (CurrentGear == 1) ClutchLock = 0.0 else ClutchLock = LockCandidate
	ClutchLock = (CurrentGear == 1) ? 0.0 : LockCandidate;

	//Lerp(TransmissionTorque / GetTotalGearRatio, InEngineTorque, Alpha = ClutchLock )
	const double FromTransmissionSide = (GetTotalGearRatio() != 0.0) ? (TransmissionTorque / GetTotalGearRatio()) : 0.0;
	const double FromEngineSide = InEngineTorque;

	// FMath::Lerp for double
	const double ClutchTorqueLoad = FMath::Lerp(FromTransmissionSide, FromEngineSide, ClutchLock);

	//Clamp to [-ClutchCapacity .. +ClutchCapacity] ---
	const double Cap = static_cast<double>(MechanicalData.ClutchData.ClutchCapacity);
	const double ClutchTorqueLoadClamped = FMath::Clamp(ClutchTorqueLoad, -Cap, Cap);

	//Multiply by SClutch.Efficiency ---
	const double ClutchEfficiency = static_cast<double>(MechanicalData.ClutchData.Efficiency);
	ClutchTorque = ClutchEfficiency * ClutchTorqueLoadClamped;

	//Set variable & FunctionResult(ClutchTorque)
	return ClutchTorque;
}

double UACVehiclePhysics::GetTotalTractionTorque()
{
	double TotalTransmissionDriveTorqueL = 0;
	for(auto& Axis : AxisArray)
	{
		if (!IsValid(Axis)) 
			continue;

		double AxisTractionTorque = Axis->GetCurrentAxisTractionTorque();

		TotalTransmissionDriveTorqueL += AxisTractionTorque * Axis->AxisDriveTorqueRatio;
	}

	return TotalTransmissionDriveTorqueL;
}

double UACVehiclePhysics::GetTotalFrictionTorque()
{
	double TotalTransmissionFrictionTorqueL = 0;
	for (auto& Axis : AxisArray)
	{
		if (!IsValid(Axis))
			continue;

		double AxisFrictionTorque = Axis->GetCurrentAxisFrictionTorque();
		TotalTransmissionFrictionTorqueL += AxisFrictionTorque * Axis->AxisDriveTorqueRatio;
	}

	return TotalTransmissionFrictionTorqueL;
}

double UACVehiclePhysics::GetWorldTemperature() const
{
	return GetVehicle()->GetWorldTemperature();
}

double UACVehiclePhysics::GetSteeringAngle(float Steering, USCWheel* Suspension)
{
	const FSAxis AxisSetup = Suspension->GetAxisSetup();
	if (!IsValid(Suspension) || AxisSetup.AxisName.IsNone())
	{
		return Steering;
	}

	FTransform SockXform = Suspension->GetSocketTransform(AxisSetup.AxisName, ERelativeTransformSpace::RTS_Actor);

	FVector Loc = SockXform.GetLocation(); // BP: Break Transform → Location → Break Vector

	return (Loc.X < 0.0f) ? -1.0f * Steering : Steering;
}

double UACVehiclePhysics::DistanceOfCurrentAxisToLastAxis(FName InSocketName)
{
	// Get vehicle & skeletal mesh (VehicleBody)
	const AVehicle* Vehicle = GetVehicle();
	if (!Vehicle || !Vehicle->VehicleBody)
	{
		return 0.0;
	}

	// Need at least one axis to compare against
	if (AxisArray.Num() <= 0)
	{
		return 0.0;
	}

	// Last axis in the array
	USCAxis* LastAxis = AxisArray[AxisArray.Num() - 1];
	if (!LastAxis)
	{
		return 0.0;
	}

	FName LastAxisSocket = LastAxis->AxisSetup.AxisName;

	// World locations of the sockets (falls back to component location if socket missing)
	FVector CurrentAxisLoc = Vehicle->VehicleBody->GetSocketLocation(InSocketName);
	FVector LastAxisLoc = Vehicle->VehicleBody->GetSocketLocation(LastAxisSocket);

	// Distance (match BP "Vector_Distance")
	return static_cast<double>(FVector::Distance(CurrentAxisLoc, LastAxisLoc));
}

double UACVehiclePhysics::GetEngineInertia() const
{
	return FMath::Square(MechanicalData.EngineData.FlywheelRadius) * MechanicalData.EngineData.FlywheelMass * 0.5;
}

double UACVehiclePhysics::GetEngineMinKineticEnergy() const {
	double IdleAngularVelocity = RpmToRadS(MechanicalData.EngineData.IdleRPM);
	return 0.5 * GetEngineInertia() * FMath::Square(MechanicalData.EngineData.IsEngineStarted ? IdleAngularVelocity : 0.0);
}

double UACVehiclePhysics::GetEngineMaxKineticEnergy() const {
	double MaxAngularVelocity = RpmToRadS(MechanicalData.EngineData.MaxRPM);
	return 0.5 * GetEngineInertia() * FMath::Square(MaxAngularVelocity);
}

double UACVehiclePhysics::GetEngineMinAngularVelocity() const {
	return FMath::Sqrt(GetEngineMinKineticEnergy() * 2 / GetEngineInertia());
}

double UACVehiclePhysics::GetEngineMaxAngularVelocity() const {
	return FMath::Sqrt(GetEngineMaxKineticEnergy() * 2 / GetEngineInertia());
}

double UACVehiclePhysics::GetCurrentEngineKineticEnergy() const
{
	// If engine is not started we still return kinetic energy
	// at its minimum (idle) if you want a warm idle, otherwise 0.
	// Your GetEngineMinKineticEnergy() already handles that logic.
	if (!MechanicalData.EngineData.IsEngineStarted)
	{
		return GetEngineMinKineticEnergy();
	}

	const double Inertia = GetEngineInertia();
	if (Inertia <= KINDA_SMALL_NUMBER)
	{
		return 0.0; // avoid division / invalid math if setup is degenerate
	}

	// Clamp ω into physical range (min idle .. max rpm) before squaring.
	const double Omega = FMath::Clamp(EngineAngularVelocity, GetEngineMinAngularVelocity(), GetEngineMaxAngularVelocity());

	// Ek = 0.5 * I * ω^2
	return 0.5 * Inertia * Omega * Omega;
}

double UACVehiclePhysics::GetEngineAngularAcceleration() const 
{
	/*
	//Set Current Angular Acceleration by calculating KineticEnergy difference, VelocityDiffrence to increase and increase acceleration.
	double SecondsToFinishRevUp = 1.0; // Hardcoded in BP
	double AccelEnergyToBeConsumed = GetEngineMaxKineticEnergy - GetCurrentEngineKineticEnergy();

	//(Δω)^2 = ΔEk*2/I
	double VelocityDiffToAccelIn1Second = FMath::Sqrt(2 * AccelEnergyToBeConsumed / EngineInertia);
	double AccelerationToAcceleratingVelocityDifferenceIn1Scond = FMath::Square(SecondsToFinishRevUp)* VelocityDiffToAccelIn1Second;
	*/

	double EngineAngularVelotity = FMath::Clamp(EngineAngularVelocity, GetEngineMinAngularVelocity(), GetEngineMaxAngularVelocity());
	double EngineRpm = RadSToRpm(EngineAngularVelocity);

	double EngineInlineTorque = MechanicalData.EngineData.TorqueCurve->GetFloatValue(EngineRpm);
	double CurrentTorque = EngineInlineTorque * NitroBoostMultiplier * TurboBoostMultiplier;

	double AngularAcceleration = CurrentTorque / GetEngineInertia();

	//EngineAngularAcceleration = AngularAcceleration * ThrottleValue;
	return UKismetMathLibrary::InRange_FloatFloat(RadSToRpm(EngineAngularVelocity), MechanicalData.EngineData.IdleRPM, MechanicalData.EngineData.MaxRPM, true, false) ? AngularAcceleration * ThrottleValue : 0;

}

double UACVehiclePhysics::GetEngineAngularDeceleration() const
{
	//Calculate Deceleration to return from Current Kinetic Energy to Idle Kinetic Energy when throttle is not more than 0.
	/*
	double EngineAngularVelocityL = FMath::Clamp(EngineAngularVelocity, GetEngineMinAngularVelocity(), GetEngineMaxAngularVelocity());
	double EngineRpm = RadSToRpm(EngineAngularVelocityL);
	double EngineInlineTorque = MechanicalData.EngineData.TorqueCurve->GetFloatValue(EngineRpm);
	double CurrentTorque = EngineInlineTorque;
	double AngularDeceleration = CurrentTorque / GetEngineInertia();
	return (ThrottleValue == 0.0) ? AngularDeceleration : 0.0;
	*/

	
	//Set Current Angular Deceleration by calculating KineticEnergy difference, VelocityDiffrence to decrease and decrease acceleration.
	double SecondsToFinishRevUp = 1.0; // Hardcoded in BP
	double DecelEnergyToBeConsumed = GetCurrentEngineKineticEnergy() - GetEngineMinKineticEnergy();

	//(Δω)^2 = ΔEk*2/I
	double VelocityDiffToDecelIn1Second = FMath::Sqrt(2 * DecelEnergyToBeConsumed / GetEngineInertia());
	double DecelerationToDeceleratingVelocityDifferenceIn1Scond = FMath::Square(SecondsToFinishRevUp) * VelocityDiffToDecelIn1Second * static_cast<double>(ThrottleValue == 0);
	
	return DecelerationToDeceleratingVelocityDifferenceIn1Scond;
	
}

double UACVehiclePhysics::CalcEngine()
{
	//Set EngineAngularVelocity Increase EngineAngularVelocity by NeededAcceleration* ThrottleValue Clamp EngineAngularVelocity to Min and MaxEngineAngularVelocity
	double LocalEngineTorqueP = 0;

	double EngineAngularAccelerationL = GetEngineAngularAcceleration();
	double EngineAngularDecelerationL = GetEngineAngularDeceleration();

	LocalEngineTorqueP = GetEngineInertia() * (EngineAngularAccelerationL - EngineAngularDecelerationL);
	EngineTorque = LocalEngineTorqueP;

	EngineAngularVelocity = FMath::Clamp(
		EngineAngularVelocity + (EngineAngularAccelerationL - EngineAngularDecelerationL) * (GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f),
		GetEngineMinAngularVelocity(),
		GetEngineMaxAngularVelocity()
	);

	MechanicalData.EngineData.CurrentRPM = RadSToRpm(EngineAngularVelocity);
	//Engine Heat Up and Down
	if (MechanicalData.EngineData.CurrentTemperature < MechanicalData.EngineData.AvgTemp) {
		//Engine Heatup to min temp (World Temperature) if engine temp is lower than AvgTemp
		MechanicalData.EngineData.CurrentTemperature = FMath::Clamp(
			MechanicalData.EngineData.CurrentTemperature + 3 * MechanicalData.EngineData.CurrentRPM / MechanicalData.EngineData.MaxRPM * (GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f),
			0,
			MechanicalData.EngineData.AvgTemp
		);
	}
	else {
		//If Current Temp is equal or above Average Temp, start lerping between Avg and Max Temp by respecting RPM Ratio.
		MechanicalData.EngineData.CurrentTemperature = FMath::Clamp(
			UKismetMathLibrary::FInterpTo_Constant(
				MechanicalData.EngineData.CurrentTemperature,
				FMath::Lerp(MechanicalData.EngineData.AvgTemp, MechanicalData.EngineData.MaxRPM, MechanicalData.EngineData.CurrentRPM / MechanicalData.EngineData.MaxRPM * ThrottleValue * 2),
				GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f,
				1.0f),
			0,
			MechanicalData.EngineData.MaxTemp);
	}

	//Is Engine getting pain?
	if (IsEngineOverheating()) {
		//Start to decrease Engine Health if Current Temp/Max Temp ratio is more than or equal to 0,95.
		MechanicalData.EngineData.EngineHealth = MechanicalData.EngineData.EngineHealth - 5 * (GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f);
	}

	if (MechanicalData.EngineData.EngineHealth > 0) {

		return LocalEngineTorqueP;
	}
	else {
		if (MechanicalData.EngineData.IsEngineStarted && !bKillEngineDoOnceGate) {
			KillEngine();
			bKillEngineDoOnceGate = true;
		}
	}

	return LocalEngineTorqueP;

}

double UACVehiclePhysics::GetCurrentFuelEnergy() const{
	return (ThrottleValue > 0 ? GetCurrentEngineKineticEnergy() : GetEngineMinKineticEnergy()) * MechanicalData.EngineData.EnergyCoefficient;
}

double UACVehiclePhysics::GetSteeringValue() const
{
	return SteeringValue;
}

double UACVehiclePhysics::RpmToRadS(double Rpm) const
{
	return Rpm * RPM_TO_RAD_PER_SEC;
}

double UACVehiclePhysics::RadSToRpm(double RadS) const
{
	return RadS / RPM_TO_RAD_PER_SEC;
}

double UACVehiclePhysics::HpToWatt(double Hp) const
{
	return Hp * 745.7;
}

double UACVehiclePhysics::GetThrottleValue() const
{
	return ThrottleValue;
}

double UACVehiclePhysics::KelvinToCelsius(double Kelvin) const
{
	return Kelvin - 273.15;
}

double UACVehiclePhysics::CalorieToJoule(double Calorie) const
{
	return Calorie * 4.184;
}

double UACVehiclePhysics::CelsiusToKelvin(double Celsius) const
{
	return Celsius + 273.15;
}

double UACVehiclePhysics::GetCurrentPower() const
{
	return GetEngineRpm() * EngineTorque / 9.5488 / 1000;
}

double UACVehiclePhysics::GetEngineRpm() const
{
	return MechanicalData.EngineData.CurrentRPM;
}

double UACVehiclePhysics::GetTotalGearRatio()
{
	return MechanicalData.GearBoxData.Efficiency * MechanicalData.GearBoxData.FinalDriveRatio * MechanicalData.GearBoxData.GearRatios[CurrentGear].GearRatio;
}

double UACVehiclePhysics::CmsToKmh(double Cms) const
{
	return Cms * 0.036;
}

double UACVehiclePhysics::GetTotalTransmissionVelocity()
{
	double TransmissionVelocityL = 0.0;

	for (UObject* Obj : AxisArray)
	{
		if (!IsValid(Obj))
			continue;

		USCAxis* Axis = Cast<USCAxis>(Obj);
		if (!Axis) continue;

		double AxisVel = Axis->GetCurrentAxisVelocity();
		TransmissionVelocityL += AxisVel * Axis->AxisDriveTorqueRatio;
	}
	return TransmissionVelocityL;
}

bool UACVehiclePhysics::IsEngineOverheating() const
{
	return MechanicalData.EngineData.CurrentTemperature / MechanicalData.EngineData.MaxTemp >= 0.95;
}

bool UACVehiclePhysics::IsAxisCountEqualToConfig()
{
	TArray<FName> AxisSockets = GetAxisSockets();

	if ((AxisSockets.Num() == MechanicalData.AxisData.Num()))
		return true;

	// BP: Branch → False → Print String (red, 20s)
	const FString Msg = TEXT("You need to match axis data item count to Vehicle Mesh's axis socket count.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, Msg);

	UE_LOG(LogTemp, Warning, TEXT("%s (Sockets=%d, AxisData=%d)"), *Msg, AxisSockets.Num(), MechanicalData.AxisData.Num());

	return false;
}

int UACVehiclePhysics::GetCurrentGear() const
{
	return CurrentGear;
}