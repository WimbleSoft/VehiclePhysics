#pragma once

#include "EFuelType.h"
#include "SFuel.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSFuel
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FuelType"))
	EFuelType FuelType;

	/** Liters */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FuelLeft", MakeStructureDefaultValue = "5.000000"))
	double FuelLeft;

	/** Liters */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FuelCapacity", MakeStructureDefaultValue = "100.000000"))
	double FuelCapacity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FuelConsumptionCurve"/*, MakeStructureDefaultValue = "CurveFloat'/Game/Blueprints/FuelConsumptionCurve.FuelConsumptionCurve'"*/))
	TObjectPtr<UCurveFloat> FuelConsumptionCurve;
};
