#pragma once
#include "EDriveType.h"
#include "STransmission.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSTransmission
{
	GENERATED_BODY()
public:
	/** Drive Type for GearBox Torque output to be converted to differentials. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DriveType"))
	EDriveType DriveType;

	/** Front / Rear Split ratio for All Wheel Drive selection. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AWDDriveSplit", MakeStructureDefaultValue = "0.550000"))
	double AWDDriveSplit;
};
