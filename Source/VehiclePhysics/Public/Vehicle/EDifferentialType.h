#pragma once

#include "EDifferentialType.generated.h"

UENUM(BlueprintType)
enum class EDifferentialType : uint8
{
	LimitedSlip UMETA(DisplayName = "Limited Slip"),
	Open        UMETA(DisplayName = "Open")
};
