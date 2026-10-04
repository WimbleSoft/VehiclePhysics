#pragma once

#include "CoreMinimal.h"
#include "SWheel.h"
#include "SSuspension.h"
#include "EDifferentialType.h"
#include "SAxis.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSAxis
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsSteerAxis", MakeStructureDefaultValue = "True"))
	bool IsSteerAxis;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsLeftRight", MakeStructureDefaultValue = "True"))
	bool IsLeftRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "LeftRightSplit", MakeStructureDefaultValue = "0.500000"))
	float LeftRightSplit;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EfficiencyRatio", MakeStructureDefaultValue = "1.000000"))
	float EfficiencyRatio;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DifferentialType", MakeStructureDefaultValue = "LimitedSlip"))
	EDifferentialType DifferentialType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxRotationAngle", MakeStructureDefaultValue = "45.000000"))
	float MaxRotationAngle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AxisName"))
	FName AxisName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AxisWidth", MakeStructureDefaultValue = "120.000000"))
	float AxisWidth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AxisSlopeAngle", MakeStructureDefaultValue = "5.000000"))
	float AxisSlopeAngle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Suspension"))
	FSSuspension Suspension;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Wheel"))
	FSWheel Wheel;
};


