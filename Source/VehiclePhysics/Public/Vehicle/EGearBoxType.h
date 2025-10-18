#pragma once

#include "EGearBoxType.generated.h"

/**Type of Gear Mechanism*/
UENUM(BlueprintType)
enum class EGearBoxType : uint8
{
	FullAuto UMETA(DisplayName = "FullAuto", Description = ""),
	Sequential UMETA(DisplayName = "Sequential", Description = ""),
	HShifter UMETA(DisplayName = "H-Shifter", Description = "")
};
