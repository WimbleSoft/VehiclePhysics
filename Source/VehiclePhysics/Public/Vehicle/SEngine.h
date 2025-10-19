#pragma once

#include "EEngineType.h"
#include "SEngine.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSEngine
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EngineType"))
	EEngineType EngineType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IsEngineStarted", MakeStructureDefaultValue = "False"))
	bool IsEngineStarted;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TorqueCurve", MakeStructureDefaultValue = "None"))
	TObjectPtr<UCurveFloat> TorqueCurve;

	/** L */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EngineVolume", MakeStructureDefaultValue = "1.500000"))
	double EngineVolume;

	/** Rounds Per Minute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "IdleRPM", MakeStructureDefaultValue = "750.000000"))
	double IdleRPM;

	/** Revelations Per Minute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CurrentRPM", MakeStructureDefaultValue = "0.000000"))
	double CurrentRPM;

	/** Revelations Per Minute */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxRPM", MakeStructureDefaultValue = "8000.000000"))
	double MaxRPM;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EnergyCoefficient", MakeStructureDefaultValue = "0.750000"))
	double EnergyCoefficient;

	/** m */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FlywheelRadius", MakeStructureDefaultValue = "0.300000"))
	double FlywheelRadius;

	/** kg */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FlywheelMass", MakeStructureDefaultValue = "30.000000"))
	double FlywheelMass;

	/** Seconds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "RevLimiterDuration", MakeStructureDefaultValue = "0.050000"))
	double RevLimiterDuration;

	/** Celsius */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CurrentTemperature", MakeStructureDefaultValue = "0.000000"))
	double CurrentTemperature;

	/** Avg Celsius @ Avg RPM */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AvgTemp", MakeStructureDefaultValue = "75.000000"))
	double AvgTemp;

	/** Max Celsius @ Max RPM */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "MaxTemp", MakeStructureDefaultValue = "110.000000"))
	double MaxTemp;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EngineHealth", MakeStructureDefaultValue = "100.000000"))
	double EngineHealth;
};
