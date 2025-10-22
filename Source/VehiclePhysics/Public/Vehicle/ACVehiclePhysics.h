#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Vehicle.h"
#include "SCWheel.h"
#include "SCAxis.h"
#include "SMechanicalData.h"

#include "ACVehiclePhysics.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UACVehiclePhysics : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACVehiclePhysics();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetAxisSockets(TArray<FName>& AxisSockets, TArray<FName> SocketsP);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetVehicle(AVehicle*& VehicleActor);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetSteeringInput(double Steering);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetThrottleInput(double Throttle);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetBrakeInput(double Brake);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetClutchInput(double Clutch);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "ReadData")
	void GetSuspensions(TArray<USCWheel*>& Suspensions, TArray<USCWheel*> SuspensionArrayP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC_Creator")
	void CreateAxis();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetHandbrake(bool NewHandbrake);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void SetLoad();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void CalcEngine(double& EngineTorqueP, double RestOrIdleKineticEnergyP, double MaxKineticEnergyP, double LocalEngineTorqueP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void CalcTurbo(double TurboMultiplier);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Fuel Data")
	void CalcFuelConsumption(double ConsumptionMultiplier);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void TriggerBackfires();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetForwardSpeed(double& ForwardSpeed);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetCurrentGear(int32& CurrentGearP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetGearUp();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetGearDown();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetUseAutoGearBox(bool IsUsingAutoGearBox);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetSteeringValue(double& SteeringValueP);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void RPMtoRadS(double RPM, double& RadS);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void RadStoRPM(double RadS, double& RPM);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void HPtoWatt(double HP, double& Watt);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetThrottleValue(double& ThrottleValueP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC_Creator")
	void CreateWheelMeshes();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void ClearArrays();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void KelvinToCelsius(double Kelvin, double& Celcius);

	/**
		* Calorie to Joule (Watt*s)
		* kg*m^2/s^2
		*/
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void CalorieToJoule(double Calorie, double& Joule);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void CelsiusToKelvin(double Celsius, double& Kelvin);

	/** kW */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetCurrentPower(double& Power);

	/** kW */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetEngineRPM(double& RPM);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Fuel Data")
	void ReFuel(double LitersPerSecond);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void CalcNitrous(double NitrousPowerMultiplier);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	void GetTotalGearRatio(double& GearRatio);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetNitrous(bool NewNitrous);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Transmission Data")
	void InitTransmissionRatios();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void PrintDebug();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion")
	void CmsToKmh(double Cms, double& Kmh);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Transmission Data")
	void CalcTransmissionTorque(double ClutchTorqueP);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data")
	void GetTotalTransmissionVelocity(double& TotalTransmissionVelocityP, double LocalTransmissionVelocityP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void SetWheelFeedback();

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data")
	void GetTotalFrictionAcceleration(double& Output_Get);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Transmission Data")
	void SplitAxisReferences();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Brake Data")
	void CalcBrakeTorque();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Brake Data")
	void InitBrakeRatios();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Clutch Data")
	void CalcClutch(double EngineTorqueP, double& ClutchTorqueP);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data")
	void GetTotalTractionTorque(double& TotalTransmissionDriveTorqueP, double TotalDriveTorqueP);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data")
	void GetTotalFrictionTorque(double& TotalTransmissionFrictionTorqueP, double TotalFrictionTorqueP);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Misc")
	void GetWorldTemperature(double& Temp);
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mechanical Setup", meta = (MultiLine = "true"))
	FSMechanicalData MechanicalData;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double ThrottleValue;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothThrottle;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double BrakeValue;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothBrake;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double ClutchValue;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothClutch;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true", UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
	double SteeringValue;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> AxisArray;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool SmoothSteering;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool HandbrakeValue;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double Load;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	TObjectPtr<UCurveFloat> FuelConsumptionCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	bool CanIgnite;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineStarted);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FEngineStarted EngineStarted;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineStopped);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FEngineStopped EngineStopped;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	TArray<double> BackfireRPMs;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineExploded);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FEngineExploded EngineExploded;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerBackfire);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FTriggerBackfire TriggerBackfire;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	int32 CurrentGear;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "GearBox Data", meta = (MultiLine = "true"))
	bool GearChange;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGearChanged);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FGearChanged GearChanged;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Brake Data", meta = (MultiLine = "true"))
	bool IsBraking;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	bool UseAutoGearBox;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool AckermannAccuracy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USkeletalMeshComponent*> WheelMeshArray;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", UIMax = "1", ClampMax = "1"))
	double TurboBoostMultiplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true"))
	bool CanTurboDecrease;

	/** cm^3 / s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	double FuelConsumption;

	/** rad/s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineCurrentKineticEnergy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineAngularAcceleration;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineAngularDeceleration;

	/** rad/s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineAngularVelocity;

	/** watt * s  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	double CurrentFuelEnergy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Nitro Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", ClampMax = "1", UIMax = "1"))
	double NitroBoostMultiplier;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Nitro Data", meta = (MultiLine = "true"))
	bool IsNitroBeingUsed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", ClampMax = "1", UIMax = "1"))
	double TempTurboPressure;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerNitrous, bool, Enable);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FTriggerNitrous TriggerNitrous;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Transmission Data", meta = (MultiLine = "true"))
	TArray<double> AxisDriveTorqueRatios;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchDiskAngularVelocity;

	/** k */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchLock;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchTorque;

	/** ω=2*Ek/I = rad/s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineRestOrIdleAngularVelocity;

	/** kg.m^2 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineInertia;

	/** ω=2*Ek/I = rad/s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineMaxAngularVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	double GearBoxInputTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	double GearBoxOutputTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	int32 TargetGear;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TransmissionTorque;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalDriveAxisAngularVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalFrictionAcceleration;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalDriveAxisAngularFrictionVelocity;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Brake Data", meta = (MultiLine = "true"))
	TArray<double> BrakeTorqueRatios;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> FrontAxis;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> RearAxis;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayTurboLoadSound, USoundBase*, TurboSound);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category = "Default")
	FPlayTurboLoadSound PlayTurboLoadSound;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data")
	double TurboSoundRatio;

};
