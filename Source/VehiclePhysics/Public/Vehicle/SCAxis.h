#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "SAxis.h"
#include "SCWheel.h"

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
	void CreateSuspensions();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetVehicle(AVehicle*& CarActor);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void CalcFrictionTorqueFeedbackRatioBias(USCWheel* Suspension, UPARAM(ref) TArray<USCWheel*>& AllSuspensions, double& OutputPin);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisVelocity(double& AxisVelocityP, double TotalAngularVelocityL);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetAxisDriveTorque(UPARAM(ref) double& TotalDriveTorqueP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void CalcWheelDriveTorque(double AxisTractionTorqueL);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void PrintDebug();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisFrictionTorque(double& AxisFrictionTorqueL, double TotalAxisFrictionTorqueL);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	void GetCurrentAxisTractionTorque(double& AxisTractionTorqueL, double TotalAxisTractionTorqueL);
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
