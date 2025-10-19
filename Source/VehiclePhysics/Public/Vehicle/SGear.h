#pragma once
#include "SGear.generated.h"

/** Gear Info */
USTRUCT(BlueprintType)
struct FSGear
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearName", MakeStructureDefaultValue = "R"))
	FName GearName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearRatio", MakeStructureDefaultValue = "1.000000"))
	float GearRatio;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DownRatio", MakeStructureDefaultValue = "0.350000"))
	float DownRatio;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "UpRatio", MakeStructureDefaultValue = "0.950000"))
	float UpRatio;
};
