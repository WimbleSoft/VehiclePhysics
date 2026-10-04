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

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ConstructSuspension();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void UpdatePhysics(const double& InDeltaTime);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void RadialMultiRayCast();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetSpringLength();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FLinearColor GetTraceColorByRayCurrentYAxisDepthIndex(int32 InCurrentYAxisDepthIndex) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void CreateLineTraceForHitResults(FLinearColor TraceColor, const FVector Start, const FVector End);

	/** Rotate RayEnd Point around Y Axis to have circle effect around wheel. */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetRotateRayEndPointAroundYAxis(int32 RayIndexOnXAxis);

	/**
	 * Rotate Ray End vector offset around X axis to
	 * have Wheel Camber angle effect on WheelCenter
	 */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetRotateRayEndVectorOffsetAroundXAxis(FVector RayEndVector);

	/** Rotate RayEnd Around Z axis to have wheel steering effect on RayEnd point. */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetRotateRayEndAroundZAxis(FVector RayEndVector);

	/** Rotate RayStart around Z axis to have wheel steering effect on RayStart point. */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetRotateRayStartAroundZAxis(FVector RayStartVector);

	/** Y axis depth offset for having RayStart points spread across wheel width. */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetYAxisDepthOffset(int32 RayIndexOnYAxis);

	/** Rotate Ray Start offset vector around X axis to have Wheel Camber angle effect on WheelCenter. */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetRotateRayStartOffsetVectorAroundXAxis(FVector RayStartVector);

	/** Add Y_Axis offset to RayStart offset caused by AxisSlopeAngle */
	UFUNCTION(BlueprintPure, BlueprintCallable)
	FVector GetYAxisOffsetToRayStartForAxisSlopeAngle();

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
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Ownership", meta = (ReturnDisplayName = "Axis"))
	USCAxis* GetAxis() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Setup Data", meta = (ReturnDisplayName = "AxisSetup"))
	FSAxis GetAxisSetup() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data", meta = (ReturnDisplayName = "SuspensionSetup"))
	FSSuspension GetSuspensionSetup() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data", meta = (ReturnDisplayName = "WheelSetup"))
	FSWheel GetWheelSetup() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void DrawDebugLines();

	/** Vector m/s */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Tire Data", meta = (ReturnDisplayName = "WheelLinearVelocityLocal"))
	FVector GetWheelLinearVelocityLocal();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data")
	FText GetSuspensionTag() const;

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
	UFUNCTION(BlueprintPure, Category = "Read Data |Read Setup Data", meta = (ReturnDisplayName = "WheelInertia"))
	double GetWheelInertia() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetWheelAngularVelocity();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category = "Read Data | Read Tire Data", meta = (ReturnDisplayName = "LongSlipVelocity"))
	double GetLongSlipVelocity();

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
	void SetBrakeTorque(double bBrakeValue, bool bHandbrakeValue);

public:
	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
	double CurrentSpringLength;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
	double LastSpringLength;

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fx;

	/** Tire Friction */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FxVec;

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fy;

	/** Tire Friction */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FyVec;

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	double Fz;

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Forces", meta = (MultiLine = "true"))
	FVector FzVec;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCastSetup", meta = (MultiLine = "true"))
	double W_RayTotalDegree;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCastSetup", meta = (MultiLine = "true"))
	double W_RayCountPerDepth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCastSetup", meta = (MultiLine = "true"))
	int32 W_RayYaxisDepth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	FHitResult HitResult;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<FHitResult> HitResults;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	bool bWheelContact;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<bool> WheelContacts;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<FVector> HitLocations;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
	TArray<float> HitDepthArray;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuspensionData", meta = (MultiLine = "true", ExposeOnSpawn = "true"))
	bool bIsLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelData", meta = (MultiLine = "true"))
	double WheelTorqueRatio;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double SteeringAngle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	FVector WheelCenterWorld;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
	double MaxHitDepth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "SuspensionData", meta = (MultiLine = "true"))
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double LongSlipNormalized;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "WheelData", meta = (MultiLine = "true"))
	double LateralSlipNormalized;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RayCast", meta = (MultiLine = "true"))
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

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, Category = "Slip", meta = (MultiLine = "true"))
	double Sx;

	/** Suspension Force */
	UPROPERTY(BlueprintReadWrite, Category = "Slip", meta = (MultiLine = "true"))
	double Sy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double BrakeBias;
		
};