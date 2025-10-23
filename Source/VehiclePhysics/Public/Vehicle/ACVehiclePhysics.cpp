#pragma once
#include "ACVehiclePhysics.h"
#include "Vehicle.h"

// Sets default values for this component's properties
UACVehiclePhysics::UACVehiclePhysics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
}

void UACVehiclePhysics::SetClutchInput(double Clutch)
{
}

void UACVehiclePhysics::GetSuspensions(TArray<USCWheel*>& Suspensions, TArray<USCWheel*> SuspensionArrayP)
{
}

void UACVehiclePhysics::CreateAxis()
{
}

void UACVehiclePhysics::SetHandbrake(bool NewHandbrake)
{
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
}

void UACVehiclePhysics::GetForwardSpeed(double& ForwardSpeed)
{
}

void UACVehiclePhysics::GetCurrentGear(int32& CurrentGearP) const
{
	CurrentGearP = CurrentGear;
}

void UACVehiclePhysics::SetGearUp()
{
}

void UACVehiclePhysics::SetGearDown()
{
}

void UACVehiclePhysics::SetUseAutoGearBox(bool bIsUsingAutoGearBox)
{
	UseAutoGearBox = bIsUsingAutoGearBox;
}

void UACVehiclePhysics::GetSteeringValue(double& OutSteeringValue)
{
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
}

void UACVehiclePhysics::CreateWheelMeshes()
{
}

void UACVehiclePhysics::ClearArrays()
{
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
}

void UACVehiclePhysics::GetTotalGearRatio(double& GearRatio)
{
	GearRatio = MechanicalData.GearBoxData.Efficiency * MechanicalData.GearBoxData.FinalDriveRatio * MechanicalData.GearBoxData.GearRatios[CurrentGear].GearRatio;
}

void UACVehiclePhysics::SetNitrous(bool bNewNitrous)
{
}

void UACVehiclePhysics::InitTransmissionRatios()
{
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
}

void UACVehiclePhysics::GetTotalTransmissionVelocity(double& TotalTransmissionVelocity)
{
	double LocalTransmissionVelocityP = 0.0;
}

void UACVehiclePhysics::SetWheelFeedback()
{
}

void UACVehiclePhysics::GetTotalFrictionAcceleration(double& Output_Get)
{
}

void UACVehiclePhysics::SplitAxisReferences()
{
}

void UACVehiclePhysics::CalcBrakeTorque()
{
}

void UACVehiclePhysics::InitBrakeRatios()
{
}

void UACVehiclePhysics::CalcClutch(double InEngineTorque, double& OutClutchTorque)
{
}

void UACVehiclePhysics::GetTotalTractionTorque(double& TotalTransmissionDriveTorqueP, double TotalDriveTorqueP)
{
}

void UACVehiclePhysics::GetTotalFrictionTorque(double& TotalTransmissionFrictionTorqueP, double TotalFrictionTorqueP)
{
}

void UACVehiclePhysics::GetWorldTemperature(double& Temp)
{
	Temp = 25.0;
}



