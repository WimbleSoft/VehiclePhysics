// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SAxis.h"

#include "SCAxis.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USCAxis : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USCAxis();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void CreateSuspensionsX();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetVehicleX(AVehicle*& CarActor);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void CalcFrictionTorqueFeedbackRatioBiasX(USCWheel* Suspension, UPARAM(ref) TArray<USCWheel*>& AllSuspensions, double& OutputPin);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisVelocityX(double& AxisVelocity, double TotalAngularVelocity);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetAxisDriveTorqueX(UPARAM(ref) double& TotalDriveTorque);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void CalcWheelDriveTorqueX(double AxisTractionTorqueL);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void PrintDebugX();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisFrictionTorqueX(double& AxisFrictionTorqueL, double TotalAxisFrictionTorqueL);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisTractionTorqueX(double& AxisTractionTorqueL, double TotalAxisTractionTorqueL);
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default", meta = (MultiLine = "true", ExposeOnSpawn = "true"))
	FSAxis AxisSetup;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	TArray<USCWheel*> Suspensions;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double AxisTractionTorque;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	double AxisDriveTorqueRatio;
};
