#pragma once

#include "EFuelType.generated.h"

UENUM(BlueprintType)
enum class EFuelType : uint8
{
	Electricity UMETA(DisplayName = "Electricity", Description = ""),
	Gasoline    UMETA(DisplayName = "Gasoline", Description = ""),
	Diesel      UMETA(DisplayName = "Diesel", Description = ""),
	LPG			UMETA(DisplayName = "LPG", Description = "")

};
