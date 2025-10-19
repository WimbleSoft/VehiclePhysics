#pragma once

#include "SBrake.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSBrake
{
	GENERATED_BODY()
public:
	/** Front / Rear wheel brake bias. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "SplitBias", MakeStructureDefaultValue = "0.500000"))
	float SplitBias;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Radius", MakeStructureDefaultValue = "30.000000"))
	float Radius;

	/** N */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "SinglePistonForce", MakeStructureDefaultValue = "500.000000"))
	float SinglePistonForce;

	/** piece */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "PistonCount", MakeStructureDefaultValue = "4.000000"))
	float PistonCount;

	/** k */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FrictionCoefficient", MakeStructureDefaultValue = "1.000000"))
	float FrictionCoefficient;
};
