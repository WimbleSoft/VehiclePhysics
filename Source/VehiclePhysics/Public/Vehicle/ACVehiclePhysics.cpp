#pragma once
#include "ACVehiclePhysics.h"
#include "Vehicle.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UACVehiclePhysics::UACVehiclePhysics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ClearArrays();
	/*if (IsAxisCountEqualToConfig()) {
		CreateAxis();

		double Temperature = 0;
		GetWorldTemperature(Temperature);
		MechanicalData.EngineData.CurrentTemperature = Temperature;

		SplitAxisReferences();
		InitTransmissionRatios();
		InitBrakeRatios();
		CreateWheelMeshes();
	}*/
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
}

void UACVehiclePhysics::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

namespace
{
	static constexpr double RPM_TO_RAD_PER_SEC = PI / 30.0;          // ω = RPM * 2π / 60 = π / 30
}

void UACVehiclePhysics::GetAxisSockets(TArray<FName>& AxisSockets)
{
	TArray<FName> AxisSocketsL;
	AVehicle* Vehicle = nullptr;
	
	GetVehicle(Vehicle);
	AxisSockets = AxisSocketsL;

	USkeletalMeshComponent* BodyMesh = Vehicle->VehicleBody;
	if (!IsValid(BodyMesh))
	{
		AxisSockets = AxisSocketsL;
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

	AxisSockets = AxisSocketsL;
}

void UACVehiclePhysics::GetVehicle(AVehicle*& VehicleActor)
{
	VehicleActor = Cast<AVehicle>(GetOwner());
}

void UACVehiclePhysics::SetSteeringInput(double Steering)
{
}

void UACVehiclePhysics::SetThrottleInput(double Throttle)
{
}

void UACVehiclePhysics::SetBrakeInput(double Brake)
{
	// UE velocities are cm/s. Your graph multiplies |speed_cm_per_s| by 0.036 (cm/s -> km/h).
	const double speedKmh = FMath::Abs(GetForwardSpeed()) * 0.036;     // Abs(x)*0.036 nodes
	const bool speedGT1Kmh = (speedKmh > 1.0);               // > 1.0 check
	const bool speedLT_0p001 = (speedKmh < 0.001);             // < 0.001 check (near-still)

	int32 gear = 0;                     // used by >1 and != -1 checks
	GetCurrentGear(gear);
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

void UACVehiclePhysics::GetSuspensions(TArray<USCWheel*>& OutSuspensions)
{
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

}

void UACVehiclePhysics::CreateAxis()
{
	AVehicle* Vehicle = nullptr;
	GetVehicle(Vehicle);
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
}

void UACVehiclePhysics::CalcEngine(double& EngineTorqueP, double RestOrIdleKineticEnergyP, double MaxKineticEnergyP, double LocalEngineTorqueP)
{
}

void UACVehiclePhysics::CalcTurbo(double TurboMultiplier)
{
}

void UACVehiclePhysics::CalcFuelConsumption(double ConsumptionMultiplier)
{
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

double UACVehiclePhysics::GetForwardSpeed()
{
	AVehicle* Vehicle = nullptr;
	GetVehicle(Vehicle);
	if (!IsValid(Vehicle))
	{
		return Vehicle->GetVelocity().Size();
	}

	return 0.0;
}

void UACVehiclePhysics::GetCurrentGear(int32& CurrentGearP) const
{
	CurrentGearP = CurrentGear;
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

void UACVehiclePhysics::SetUseAutoGearBox(bool bIsUsingAutoGearBox)
{
	UseAutoGearBox = bIsUsingAutoGearBox;
}

void UACVehiclePhysics::GetSteeringValue(double& OutSteeringValue) const
{
	OutSteeringValue = SteeringValue;
}

void UACVehiclePhysics::RpmToRadS(double Rpm, double& RadS)
{
	RadS = Rpm * RPM_TO_RAD_PER_SEC;
}

void UACVehiclePhysics::RadSToRpm(double RadS, double& Rpm)
{
	Rpm = RadS / RPM_TO_RAD_PER_SEC;
}

void UACVehiclePhysics::HpToWatt(double Hp, double& Watt)
{
	Watt = Hp * 745.7;
}

void UACVehiclePhysics::GetThrottleValue(double& ThrottleValueP)
{
	ThrottleValueP = ThrottleValue;
}

void UACVehiclePhysics::CreateWheelMeshes()
{
	AVehicle* Vehicle = nullptr;
	GetVehicle(Vehicle);

	if (!IsValid(Vehicle))
		return;

	TArray<USCWheel*> SuspensionArray;
	GetSuspensions(SuspensionArray);

	// For each suspension, create the wheel mesh and add result to WheelMeshArray
	for (USCWheel* Suspension : SuspensionArray)
	{
		if (!IsValid(Suspension))
			continue;

		USkeletalMeshComponent* WheelMesh = nullptr;
		Vehicle->CreateWheelMesh(Suspension, WheelMesh);

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

void UACVehiclePhysics::KelvinToCelsius(double Kelvin, double& Celcius)
{
	Celcius = Kelvin - 273.15;
}

void UACVehiclePhysics::CalorieToJoule(double Calorie, double& Joule)
{
	Joule = Calorie * 4.184;
}

void UACVehiclePhysics::CelsiusToKelvin(double Celsius, double& Kelvin)
{
	Kelvin = Celsius + 273.15;
}

void UACVehiclePhysics::GetCurrentPower(double& Power) const
{
	double Rpm = 0;
	GetEngineRpm(Rpm);
	//Convert to KW
	Power = Rpm * EngineTorque / 9.5488 / 1000;
}

void UACVehiclePhysics::GetEngineRpm(double& Rpm) const
{
	Rpm = MechanicalData.EngineData.CurrentRPM;
}

void UACVehiclePhysics::ReFuel(double LitersPerSecond)
{
}

void UACVehiclePhysics::CalcNitrous(double NitrousPowerMultiplier)
{
	// 1) Sample curve at current RPM
	double Rpm = 0;
	GetEngineRpm(Rpm);


	float CurvePct = 0.f; // percent from curve (e.g., 0..50)
	if (MechanicalData.NitroData.NitroCurve)
	{
		CurvePct = MechanicalData.NitroData.NitroCurve->GetFloatValue(Rpm);
	}

	const bool bCanBoost = IsNitroBeingUsed && (MechanicalData.NitroData.NitroLeft > 0.f);

	// 2) Compute Boost multiplier (1 + multiplier * percent/100), or 1 when off
	const float Boost = bCanBoost ? 1.f + NitrousPowerMultiplier * (CurvePct * 0.01f) : 1.f;

	NitroBoostMultiplier = Boost;

	// 3) Consume nitro while Boosting
	if (bCanBoost)
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
			IsNitroBeingUsed = false;
		}
	}
}

void UACVehiclePhysics::GetTotalGearRatio(double& GearRatio)
{
	GearRatio = MechanicalData.GearBoxData.Efficiency * MechanicalData.GearBoxData.FinalDriveRatio * MechanicalData.GearBoxData.GearRatios[CurrentGear].GearRatio;
}

void UACVehiclePhysics::SetNitrous(bool bNewNitrous)
{
	if (MechanicalData.EngineData.IsEngineStarted) {
		if (bNewNitrous) {
			// Activate nitro only if CurrentRPM is above 15% of MaxRPM and throttle is pressed
			if (MechanicalData.EngineData.CurrentRPM > MechanicalData.EngineData.MaxRPM * 0.15 && ThrottleValue > 0) {
				IsNitroBeingUsed = true;
				TriggerNitrous.Broadcast(true);
			}
		}
		else {
			IsNitroBeingUsed = false;
			TriggerNitrous.Broadcast(false);
		}
	}
	else {
		IsNitroBeingUsed = false;
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
}

void UACVehiclePhysics::CmsToKmh(double Cms, double& Kmh)
{
	Kmh = Cms * 0.036;
}

void UACVehiclePhysics::CalcTransmissionTorque(double ClutchTorqueP)
{
	double TotalGearRatio = 0;
	GetTotalGearRatio(TotalGearRatio);
	double TotalDriveTorque = FMath::Max(ClutchTorque, 0.0) * TotalGearRatio;
	
	TransmissionTorque = TotalDriveTorque;

	for(auto Axis : AxisArray)
	{
		if (!IsValid(Axis)) 
			continue;
		
		Axis->SetAxisDriveTorque(TotalDriveTorque);
	}
}

void UACVehiclePhysics::GetTotalTransmissionVelocity(double& TotalTransmissionVelocity)
{
	double TransmissionVelocityL = 0.0;

	for (UObject* Obj : AxisArray)
	{
		if (!IsValid(Obj)) 
			continue;

		USCAxis* Axis = Cast<USCAxis>(Obj);
		if (!Axis) continue;

		double AxisVel = 0;
		Axis->GetCurrentAxisVelocity(AxisVel);
		TransmissionVelocityL += AxisVel * Axis->AxisDriveTorqueRatio;
	}

}

void UACVehiclePhysics::SetWheelFeedback()
{
}

void UACVehiclePhysics::GetTotalFrictionAcceleration(double& Output_Get)
{

	if (AxisArray.Num() < 2 || AxisDriveTorqueRatios.Num() < 2)
	{
		Output_Get = 0.f;
	}
	double Inner0 = 0.f;
	double Inner1 = 0.f;

	auto AxisInnerSum = [](const USCAxis* Axis) -> double
		{
			if (!IsValid(Axis) || Axis->Suspensions.Num() < 2) 
				return 0.f;

			const USCWheel* S0 = Axis->Suspensions[0];
			const USCWheel* S1 = Axis->Suspensions[1];

			double WheelInertia0 = 0;
			Axis->Suspensions[0]->GetWheelInertia(WheelInertia0);
			double WheelInertia1 = 0;
			Axis->Suspensions[1]->GetWheelInertia(WheelInertia1);

			return Axis->Suspensions[0]->WheelFrictionTorque * WheelInertia0 + Axis->Suspensions[1]->WheelFrictionTorque * WheelInertia1;
		};

	Inner0 = AxisInnerSum(AxisArray[0]);
	Inner1 = AxisInnerSum(AxisArray[1]);

	// BP: (Inner0 * Ratio0) + (Inner1 * Ratio1)
	double WeightedSum =
		Inner0 * AxisDriveTorqueRatios[0] +
		Inner1 * AxisDriveTorqueRatios[1];

	// BP final: multiply by 0.5 and return (also assigned to "Total Friction Acceleration")
	Output_Get = 0.5f * WeightedSum;
}

void UACVehiclePhysics::SplitAxisReferences()
{
	// BP: GetVehicle -> VehicleBody (SkeletalMeshComponent)
	AVehicle* Vehicle = nullptr;
	GetVehicle(Vehicle);
	if (!Vehicle)
	{
		return;
	}

	// Expect a USkeletalMeshComponent named VehicleBody on Vehicle
	USkeletalMeshComponent* VehicleBody = nullptr;
	// If VehicleBody is public:
	//     VehicleBody = Vehicle->VehicleBody;
	// If not, replace with your accessor, e.g. Vehicle->GetVehicleBody();
	VehicleBody = /* Vehicle->VehicleBody or Vehicle->GetVehicleBody() */ nullptr;

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

void UACVehiclePhysics::CalcClutch(double InEngineTorque, double& OutClutchTorque)
{
	double Rpm = 0.0;
	GetEngineRpm(Rpm);

	
	const double AutoLock = FMath::GetMappedRangeValueClamped(FVector2D(1000, 1300), FVector2D(0, 1), Rpm);
	const double ManualLock = 1.0 - ClutchValue;
	
	//SelectFloat: if (GearBoxType != HShifter) pick AutoLock, else pick ManualLock
	const double LockCandidate = (MechanicalData.GearBoxData.GearBoxType != EGearBoxType::HShifter) ? AutoLock : ManualLock;

	//If (CurrentGear == 1) ClutchLock = 0.0 else ClutchLock = LockCandidate
	ClutchLock = (CurrentGear == 1) ? 0.0 : LockCandidate;

	//Lerp(TransmissionTorque / GetTotalGearRatio, InEngineTorque, Alpha = ClutchLock )
	double TotalGearRatioL = 0.0;
	GetTotalGearRatio(TotalGearRatioL);

	const double FromTransmissionSide = (TotalGearRatioL != 0.0) ? (TransmissionTorque / TotalGearRatioL) : 0.0;
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
	OutClutchTorque = ClutchTorque;
}

void UACVehiclePhysics::GetTotalTractionTorque(double& TotalTransmissionDriveTorque)
{
	TotalTransmissionDriveTorque = 0;
	for(auto& Axis : AxisArray)
	{
		if (!IsValid(Axis)) 
			continue;

		double AxisTractionTorque = 0;
		Axis->GetCurrentAxisTractionTorque(AxisTractionTorque);

		TotalTransmissionDriveTorque += AxisTractionTorque * Axis->AxisDriveTorqueRatio;
	}
}

void UACVehiclePhysics::GetTotalFrictionTorque(double& TotalTransmissionFrictionTorque)
{
	TotalTransmissionFrictionTorque = 0;
	for (auto& Axis : AxisArray)
	{
		if (!IsValid(Axis))
			continue;

		double AxisFrictionTorque = 0;
		Axis->GetCurrentAxisFrictionTorque(AxisFrictionTorque);
		TotalTransmissionFrictionTorque += AxisFrictionTorque * Axis->AxisDriveTorqueRatio;
	}
}

void UACVehiclePhysics::GetWorldTemperature(double& Temp)
{
	AVehicle* Vehicle = nullptr;
	GetVehicle(Vehicle);
	Vehicle->GetWorldTemperature(Temp);
}

double UACVehiclePhysics::GetSteeringAngle(float Steering, USCWheel* Suspension)
{
	FSAxis AxisSetup;
	Suspension->GetAxisSetup(AxisSetup);
	if (!IsValid(Suspension) || AxisSetup.AxisName.IsNone())
	{
		return Steering;
	}

	FTransform SockXform = Suspension->GetSocketTransform(AxisSetup.AxisName, ERelativeTransformSpace::RTS_Actor);

	FVector Loc = SockXform.GetLocation(); // BP: Break Transform → Location → Break Vector

	return (Loc.X < 0.0f) ? -1.0f * Steering : Steering;
}

bool UACVehiclePhysics::IsAxisCountEqualToConfig()
{
	TArray<FName> AxisSockets;
	GetAxisSockets(AxisSockets);

	if ((AxisSockets.Num() == MechanicalData.AxisData.Num()))
		return true;

	// BP: Branch → False → Print String (red, 20s)
	const FString Msg = TEXT("You need to match axis data item count to Vehicle Mesh's axis socket count.");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, Msg);

	UE_LOG(LogTemp, Warning, TEXT("%s (Sockets=%d, AxisData=%d)"), *Msg, AxisSockets.Num(), MechanicalData.AxisData.Num());

	return false;
}

double UACVehiclePhysics::DistanceOfCurrentAxisToLastAxis(FName InSocketName)
{
	// Get vehicle & skeletal mesh (VehicleBody)
	AVehicle* VehicleActor;
	GetVehicle(VehicleActor);
	if (!VehicleActor || !VehicleActor->VehicleBody)
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
	FVector CurrentAxisLoc = VehicleActor->VehicleBody->GetSocketLocation(InSocketName);
	FVector LastAxisLoc = VehicleActor->VehicleBody->GetSocketLocation(LastAxisSocket);

	// Distance (match BP "Vector_Distance")
	return static_cast<double>(FVector::Distance(CurrentAxisLoc, LastAxisLoc));
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