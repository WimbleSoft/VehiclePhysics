#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "SCWheel.h"
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
	UFUNCTION(BlueprintCallable, Category = "Default")
	void ConstructVehiclePhysics();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	TArray<FName> GetAxisSockets();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data")
	AVehicle* GetVehicle() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetSteeringInput(double Steering);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetThrottleInput(double Throttle);

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Engine Handling")
	void StartEngine();

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = "Engine Handling")
	void StopEngine();

	UFUNCTION(BlueprintCallable, Category = "Engine Handling")
	void FixEngine();

	UFUNCTION(BlueprintCallable, Category = "Engine Handling")
	void KillEngine();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetBrakeInput(double Brake);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Input Handling")
	void SetClutchInput(double Clutch);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "ReadData")
	TArray<USCWheel*> GetSuspensions();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC_Creator")
	void CreateAxis();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetHandbrake(bool bNewHandbrake);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void SetLoad();

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
    /**
    * Returns the current forward speed of the vehicle in cm/s.
    * Blueprint Tooltip: Hovering the node will show this description.
    * NOTE: You cannot set a dedicated tooltip just for the return value pin.
    * Use ReturnDisplayName to rename the return pin label.
    */
    UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "Forward Speed (cm/s)"))
    double GetForwardSpeed();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "CurrentGear"))
	int GetCurrentGear() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetGearUp();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetGearDown();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetGear(int32 InCurrentGear);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Gear Data")
	void SetUseAutoGearBox(bool bIsUsingAutoGearBox);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "SteeringValue"))
	double GetSteeringValue() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "RadS"))
	double RpmToRadS(double Rpm) const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Rpm"))
	double RadSToRpm(double RadS) const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Watt"))
	double HpToWatt(double Hp) const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "ThrottleValue"))
	double GetThrottleValue() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC_Creator")
	void CreateWheelMeshes();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void ClearArrays();

	/**
		* Calorie to Joule (Watt*s)
		* kg*m^2/s^2
		*/
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Joule"))
	double CalorieToJoule(double Calorie) const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Celcius"))
	double KelvinToCelsius(double Kelvin) const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Kelvin"))
	double CelsiusToKelvin(double Celsius) const;

	/** kW */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "Power"))
	double GetCurrentPower() const;

	/** Revs per Minute */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "Rpm"))
	double GetEngineRpm() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Fuel Data")
	void ReFuel(double LitersPerSecond);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void CalcNitrous(double NitrousPowerMultiplier);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "GearRatio"))
	double GetTotalGearRatio();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "InputHandling")
	void SetNitrous(bool bNewNitrous);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Transmission Data")
	void InitTransmissionRatios();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Misc")
	void PrintDebug();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "UnitConversion", meta = (ReturnDisplayName = "Kmh"))
	double CmsToKmh(double Cms) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Transmission Data")
	void CalcTransmissionTorque(double ClutchTorqueP);

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data", meta = (ReturnDisplayName = "TotalTransmissionVelocity"))
	double GetTotalTransmissionVelocity();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data")
	void SetWheelFeedback();

	/** N.m */
	UFUNCTION(BlueprintPure, Category = "Get Transmission Data")
	double GetTotalFrictionAcceleration();

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
	UFUNCTION(BlueprintCallable, Category = "Set Engine Data", meta = (ReturnDisplayName = "EngineTorque"))
	double CalcEngine();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Set Clutch Data", meta = (ReturnDisplayName = "ClutchTorque"))
	double CalcClutch(double InEngineTorque);

	/** N.m */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Get Transmission Data", meta = (ReturnDisplayName = "TotalTransmissionDriveTorque"))
	double GetTotalTractionTorque();

	/** N.m */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Get Transmission Data", meta = (ReturnDisplayName = "TotalTransmissionFrictionTorque"))
	double GetTotalFrictionTorque();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Misc", meta = (ReturnDisplayName = "Temp"))
	double GetWorldTemperature() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "SteeringAngle"))
	double GetSteeringAngle(float Steering, USCWheel* Suspension);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data", meta = (ReturnDisplayName = "IsEqual"))
	bool IsAxisCountEqualToConfig();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double DistanceOfCurrentAxisToLastAxis(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Set Data")
	void SetSteeringValueByAckermannAccuracy(USCWheel* Suspension, FName SocketName, double CurrentRotationAngle, double AxisWidth);

	/** kg.m^2 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineInertia() const;

	/** Ek = 1/2*I*ω^2 = watt*s */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineMinKineticEnergy() const;

	/** Ek = 1/2*I*ω^2 = watt*s */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineMaxKineticEnergy() const;

	/** ω=2*Ek/I = rad/s */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineMinAngularVelocity() const;

	/** ω=2*Ek/I = rad/s */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineMaxAngularVelocity() const;

	/** Ek = 1/2*I*ω^2 = watt*s */
	UFUNCTION(BlueprintCallable, Category = "Read Data")
	double GetCurrentEngineKineticEnergy() const;
	
	/** α = Δω/Δt = rad/s^2 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineAngularAcceleration() const;

	/** α = Δω/Δt = rad/s^2 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	double GetEngineAngularDeceleration() const;

	/**Calculate Needed Fuel Energy to Supply engine for running and calculate heat energy to increase temperature.*/
	UFUNCTION(BlueprintCallable, Category = "Read Data")
	double GetCurrentFuelEnergy() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Data")
	bool IsEngineOverheating() const;

	UFUNCTION(BlueprintCallable, Category = "Read Data")
	void SetTurboLoadRatioAndTriggerSound(double TurboMultiplier);

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mechanical Setup", meta = (MultiLine = "true"))
	FSMechanicalData MechanicalData = FSMechanicalData();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double ThrottleValue = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothThrottle = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double BrakeValue = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothBrake = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	double ClutchValue = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true"))
	bool SmoothClutch = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "InputValues", meta = (MultiLine = "true", UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
	double SteeringValue = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> AxisArray = TArray<USCAxis*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool SmoothSteering = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool HandbrakeValue = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double Load = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	bool CanIgnite = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	TArray<double> BackfireRPMs = TArray<double>{ 6500, 6750, 7000, 7250, 7500, 7750, 8000, 8250, 8500, 8750, 9000 };;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	int32 CurrentGear = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	bool GearChange = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Brake Data", meta = (MultiLine = "true"))
	bool IsBraking = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	bool UseAutoGearBox = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input Values", meta = (MultiLine = "true"))
	bool AckermannAccuracy = true;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USkeletalMeshComponent*> WheelMeshArray = TArray<USkeletalMeshComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", UIMax = "1", ClampMax = "1"))
	double TurboBoostMultiplier = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true"))
	bool CanTurboDecrease = false;

	/** cm^3 / s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	double FuelConsumption = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineTorque = 0;

	/** rad/s */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Engine Data", meta = (MultiLine = "true"))
	double EngineAngularVelocity = 0;

	/** watt * s  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fuel Data", meta = (MultiLine = "true"))
	double CurrentFuelEnergy = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "Nitro Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", ClampMax = "1", UIMax = "1"))
	double NitroBoostMultiplier = 1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "Nitro Data", meta = (MultiLine = "true"))
	bool bIsNitroBeingUsed = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data", meta = (MultiLine = "true", UIMin = "0", ClampMin = "0", ClampMax = "1", UIMax = "1"))
	double TempTurboPressure = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "Transmission Data", meta = (MultiLine = "true"))
	TArray<double> AxisDriveTorqueRatios = TArray<double>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchDiskAngularVelocity = 0;

	/** k */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchLock = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Clutch Data", meta = (MultiLine = "true"))
	double ClutchTorque = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	double GearBoxInputTorque = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	double GearBoxOutputTorque = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gear Box Data", meta = (MultiLine = "true"))
	int32 TargetGear = 1;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TransmissionTorque = 0;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalDriveAxisAngularVelocity = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalFrictionAcceleration = 0;

	/** Total Drive Torque of Vehicle */
	UPROPERTY(BlueprintReadWrite, Category = "Transmission Data", meta = (MultiLine = "true"))
	double TotalDriveAxisAngularFrictionVelocity = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Brake Data", meta = (MultiLine = "true"))
	TArray<double> BrakeTorqueRatios = TArray<double>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> FrontAxis = TArray<USCAxis*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References", meta = (MultiLine = "true"))
	TArray<USCAxis*> RearAxis = TArray<USCAxis*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turbo Data")
	double TurboSoundRatio = 0;

	UPROPERTY(Transient)
	bool bTurboBlowOffDoOnceGate = true;

	UPROPERTY(Transient)
	bool bKillEngineDoOnceGate = true;

	UPROPERTY(Transient)
	bool bStartEngineDoOnceGate = true;

	UPROPERTY(Transient)
	bool bStopEngineDoOnceGate = true;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerNitrous, bool, Enable);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FTriggerNitrous TriggerNitrous;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGearChanged, int, CurrentGear);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FGearChanged GearChanged;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineStarted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FEngineStarted EngineStarted;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineStopped);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FEngineStopped EngineStopped;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEngineExploded);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default")
	FEngineExploded EngineExploded;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerBackfire);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FTriggerBackfire TriggerBackfire;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayTurboLoadSound, int, TurboReleaseLevel);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default")
	FPlayTurboLoadSound PlayTurboLoadSound;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayEngineIgniteSound);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Default")
	FPlayEngineIgniteSound PlayEngineIgniteSound;

};
