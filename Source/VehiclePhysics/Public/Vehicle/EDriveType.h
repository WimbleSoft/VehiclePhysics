#pragma once

#include "EDriveType.generated.h"

UENUM(BlueprintType)
enum class EDriveType : uint8
{
	FWD UMETA(DisplayName = "FWD", Description = "Front Wheel Drive"),
	RWD UMETA(DisplayName = "RWD", Description = "Rear Wheel Drive"),
	AWD UMETA(DisplayName = "AWD", Description = "All Wheel Drive")
};
