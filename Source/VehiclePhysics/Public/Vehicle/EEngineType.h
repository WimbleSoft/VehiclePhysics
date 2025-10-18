#pragma once

#include "EEngineType.generated.h"

UENUM(BlueprintType)
enum class EEngineType : uint8
{
	I2 UMETA(DisplayName = "I2", Description = "I-2 Cylinder"),
	V2 UMETA(DisplayName = "V2", Description = "V-2 Cylinder"),
	I3 UMETA(DisplayName = "I3", Description = "I-3 Cylinder"),
	I4 UMETA(DisplayName = "I4", Description = "I-4 Cylinder"),
	V4 UMETA(DisplayName = "V4", Description = "V-4 Cylinder"),
	I5 UMETA(DisplayName = "I5", Description = "I-5 Cylinder"),
	I6 UMETA(DisplayName = "I6", Description = "I-6 Cylinder"),
	V6 UMETA(DisplayName = "V6", Description = "V-6 Cylinder"),
	V8 UMETA(DisplayName = "V8", Description = "V-8 Cylinder"),
	V10 UMETA(DisplayName = "V10", Description = "V-10 Cylinder"),
	V12 UMETA(DisplayName = "V12", Description = "V-12 Cylinder")
};
