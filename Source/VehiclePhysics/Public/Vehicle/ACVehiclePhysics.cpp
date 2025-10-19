#include "ACVehiclePhysics.h"

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

void UACVehiclePhysics::GetAxisSockets(TArray<FName>& AxisSockets, TArray<FName> SocketsX)
{
}

void UACVehiclePhysics::GetVehicle(AVehicle*& VehicleActor)
{
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

void UACVehiclePhysics::GetSuspensions(TArray<USCWheel*>& Suspensions, TArray<USCWheel*> SuspensionArrayX)
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

void UACVehiclePhysics::CalcEngine(double& EngineTorqueP, double RestOrIdleKineticEnergyX, double MaxKineticEnergyX, double LocalEngineTorqueP)
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

void UACVehiclePhysics::GetCurrentGear(int32& CurrentGearP)
{
}

void UACVehiclePhysics::SetGearUp()
{
}

void UACVehiclePhysics::SetGearDown()
{
}

void UACVehiclePhysics::SetUseAutoGearBox(bool IsUsingAutoGearBox)
{
}

void UACVehiclePhysics::GetSteeringValue(double& SteeringValueP)
{
}

void UACVehiclePhysics::RPMtoRadS(double RPM, double& RadS)
{
}

void UACVehiclePhysics::RadStoRPM(double RadS, double& RPM)
{
}

void UACVehiclePhysics::HPtoWatt(double HP, double& Watt)
{
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
}

void UACVehiclePhysics::CalorieToJoule(double Calorie, double& Joule)
{
}

void UACVehiclePhysics::CelsiusToKelvin(double Celsius, double& Kelvin)
{
}

void UACVehiclePhysics::GetCurrentPower(double& Power)
{
}

void UACVehiclePhysics::GetEngineRPM(double& RPM)
{
}

void UACVehiclePhysics::ReFuel(double LitersPerSecond)
{
}

void UACVehiclePhysics::CalcNitrous(double NitrousPowerMultiplier)
{
}

void UACVehiclePhysics::GetTotalGearRatio(double& GearRatio)
{
}

void UACVehiclePhysics::SetNitrous(bool NewNitrous)
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
}

void UACVehiclePhysics::CalcTransmissionTorque(double ClutchTorqueP)
{
}

void UACVehiclePhysics::GetTotalTransmissionVelocity(double& TotalTransmissionVelocity, double LocalTransmissionVelocityP)
{
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

void UACVehiclePhysics::CalcClutch(double EngineTorqueP, double& ClutchTorqueP)
{
}

void UACVehiclePhysics::GetTotalTractionTorque(double& TotalTransmissionDriveTorque, double TotalDriveTorqueP)
{
}

void UACVehiclePhysics::GetTotalFrictionTorque(double& TotalTransmissionFrictionTorque, double TotalFrictionTorqueP)
{
}

void UACVehiclePhysics::GetWorldTemperature(double& Temp)
{
}



