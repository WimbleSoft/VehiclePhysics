#pragma once

#include "SNitro.generated.h"
/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSNitro
{
	GENERATED_BODY()
public:
	/** Liter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NitroLeft", MakeStructureDefaultValue = "2.000000"))
	float NitroLeft;

	/** Liter */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NitroCapacity", MakeStructureDefaultValue = "5.000000"))
	float NitroCapacity;

	/** Torque Increase Percent @ Current RPM */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NitroCurve", MakeStructureDefaultValue = "CurveFloat'/Game/Blueprints/NitroCurve.NitroCurve'"))
	TObjectPtr<UCurveFloat> NitroCurve;
};
