#pragma once
#include "SSuspension.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSSuspension
{
	GENERATED_BODY()
public:
	/** cm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "RestLength", MakeStructureDefaultValue = "50.000000"))
	float RestLength;

	/** cm */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Travel", MakeStructureDefaultValue = "20.000000"))
	float Travel;

	/** N/m */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "SpringStiffness", MakeStructureDefaultValue = "250.000000"))
	float SpringStiffness;

	/** N/m */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DamperStiffness", MakeStructureDefaultValue = "20.000000"))
	float DamperStiffness;

	/** N */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ForceMin", MakeStructureDefaultValue = "-2000.000000"))
	float ForceMin;

	/** N */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ForceMax", MakeStructureDefaultValue = "6000.000000"))
	float ForceMax;
};
