#pragma once

#include "STurbo.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSTurbo
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TurboPressure"))
	float TurboPressure;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TurboPressureCapacity", MakeStructureDefaultValue = "30.000000"))
	float TurboPressureCapacity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsTurboReleased", MakeStructureDefaultValue = "True"))
	bool IsTurboReleased;

	/** Torque Increase Percent @ Current RPM */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TurboCurve"/*, MakeStructureDefaultValue = "CurveFloat'/Game/Blueprints/TurboCurve.TurboCurve'"*/))
	TObjectPtr<UCurveFloat> TurboCurve;
};
