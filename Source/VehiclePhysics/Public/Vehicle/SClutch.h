#pragma once

#include "SClutch.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSClutch
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Efficiency", MakeStructureDefaultValue = "0.980000"))
	float Efficiency;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ScaleCurve"/*, MakeStructureDefaultValue = "CurveFloat'/Game/Blueprints/ClutchCurve.ClutchCurve'" */ ))
	TObjectPtr<UCurveFloat> ScaleCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ClutchInertia", MakeStructureDefaultValue = "5.000000"))
	float ClutchInertia;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ClutchCapacity", MakeStructureDefaultValue = "400.000000"))
	float ClutchCapacity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ClutchSpringStiffness", MakeStructureDefaultValue = "0.010000"))
	float ClutchSpringStiffness;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AutoClutchLockCurve"/*, MakeStructureDefaultValue = "CurveFloat'/Game/Blueprints/ClutchLockCurve.ClutchLockCurve'"*/))
	TObjectPtr<UCurveFloat> AutoClutchLockCurve;
};
