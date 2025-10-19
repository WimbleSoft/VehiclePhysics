// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SSuspension.h"

#include "SCWheel.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USCWheel : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCWheel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void RadialMultiRayCast();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetSuspensionForce();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ApplySuspensionForce();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ResetHitValues();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Ownership")
	void GetAxis(USCAxis*& Axis);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Setup Data")
	void GetAxisSetup(FSAxis& AxisSetup);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data")
	void GetSuspensionSetup(FSSuspension& Suspension);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data")
	void GetWheelSetup(FSWheel& Wheel);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void DrawDebugLines();

	/** Vector m/s */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Tire Data")
	void GetWheelLinearVelocityLocal(FVector& WheelLinearVelocityLocal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data")
	FText GetSuspensionTag();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void PrintDebug();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetFrictionForces();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetWheelVisualRelativeRotation();

	/** kg*m^2 */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data")
	void GetWheelInertia(double& WheelInertia);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetWheelAngularVelocity();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Tire Data")
	void GetLongSlipVelocity(double& LongSlipVelocity);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetTireForceCombined();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Ownership")
	void GetVehiclePhysics(UACVehiclePhysics*& VehiclePhysics);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ApplyTireForce();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ApplyBrakeTorque();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void WheelAcceleration();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetTractionTorque();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetFrictionTorque();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Default")
	void SetBrakeTorque(double BrakeValue, bool HandbrakeValue);
public:
	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
	double CurrentSpringLength;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
	double LastSpringLength;
private:
	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fx;

	/** Tire Friction */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FxVec;

	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fy;

	/** Tire Friction */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FyVec;

	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fz;

	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FzVec;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast Setup", meta = (MultiLine = "true"))
	double W_RayTotalDegree;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Ray Cast Setup", meta = (MultiLine = "true"))
	double W_RayCountPerDepth;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Ray Cast Setup", meta = (MultiLine = "true"))
	int32 W_RayYaxisDepth;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	FHitResult HitResult;
private:
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<FHitResult> HitResults;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	bool WheelContact;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<bool> WheelContacts;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<FVector> HitLocations;

	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<double> HitDepthArray;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuspensionData", meta = (MultiLine = "true", ExposeOnSpawn = "true"))
	bool IsLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double SteeringAngle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Wheel Data", meta = (MultiLine = "true"))
	FVector WheelCenterWorld;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Suspension Data", meta = (MultiLine = "true"))
	double MaxHitDepth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Suspension Data", meta = (MultiLine = "true"))
	FVector HitDepthVector;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	FVector WheelRelativeLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	FRotator WheelRelativeRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double WheelTractionTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double WheelFrictionTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double WheelBrakeTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double WheelAngularVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Wheel Data", meta = (MultiLine = "true"))
	double LongSlipNormalized;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Wheel Data", meta = (MultiLine = "true"))
	double LateralSlipNormalized;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ray Cast", meta = (MultiLine = "true"))
	TObjectPtr<USkeletalMeshComponent> WheelMesh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double WheelAngularDeceleration;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double RelaxationLength;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double DeltaTimer;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double SlipAnglePeak;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double SlipAngleDyn;
private:
	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Slip", meta = (MultiLine = "true"))
	double Sx;

	/** Suspension Force */
	UPROPERTY(EditDefaultsOnly, Category = "Slip", meta = (MultiLine = "true"))
	double Sy;
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double BrakeBias;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelData", meta = (MultiLine = "true"/*, ExposeOnSpawn = "true" */ ))
	double WheelTorqueRatio;
		
};