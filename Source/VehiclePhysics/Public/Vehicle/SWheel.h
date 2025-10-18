#pragma once

#include "SWheel.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSWheel
{
	GENERATED_BODY()
public:
	/** cm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Radius", MakeStructureDefaultValue = "33.000000"))
	float Radius;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Width", MakeStructureDefaultValue = "27.000000"))
	float Width;

	/** kg */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Mass", MakeStructureDefaultValue = "15.000000"))
	float Mass;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CorneringStiffness", MakeStructureDefaultValue = "0.700000"))
	float CorneringStiffness;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "LongStiffness", MakeStructureDefaultValue = "1.000000"))
	float LongStiffness;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AffectedByHandbrake"))
	bool AffectedByHandbrake;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Scale", MakeStructureDefaultValue = "1.000000,1.000000,1.000000"))
	FVector Scale;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ForceCurve"))
	TObjectPtr<UCurveFloat> ForceCurve;

	/** Nm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxHandbrakeTorque", MakeStructureDefaultValue = "2000.000000"))
	float MaxHandbrakeTorque;

	/** Degree */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CamberAngle"))
	float CamberAngle;
};
