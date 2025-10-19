#pragma once
#include "EGearBoxType.h"
#include "SGear.h"
#include "SGearBox.generated.h"

/** Gear Info */
USTRUCT(BlueprintType)
struct FSGearBox
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearBoxType"))
	EGearBoxType GearBoxType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Efficiency", MakeStructureDefaultValue = "0.980000"))
	double Efficiency;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearRatios", MakeStructureDefaultValue = "((GearRatio_31=-3.382000,DownRatio_33=0.250000,UpRatio_35=0.750000),(GearName_42='N',GearRatio_31=0.000000,DownRatio_33=0.150000,UpRatio_35=0.150000),(GearName_42='1',GearRatio_31=2.906000,DownRatio_33=0.250000,UpRatio_35=0.750000),(GearName_42='2',GearRatio_31=1.902000,DownRatio_33=0.250000,UpRatio_35=0.750000),(GearName_42='3',GearRatio_31=1.308000,DownRatio_33=0.250000,UpRatio_35=0.750000),(GearName_42='4',DownRatio_33=0.250000,UpRatio_35=0.750000),(GearName_42='5',GearRatio_31=0.864000,DownRatio_33=0.250000,UpRatio_35=0.750000))"))
	TArray<FSGear> GearRatios;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FinalDriveRatio", MakeStructureDefaultValue = "4.444000"))
	double FinalDriveRatio;

	/** Seconds */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearChangeLatency", MakeStructureDefaultValue = "0.500000"))
	double GearChangeLatency;

	/** Angular Momentum of Clutch kg*m^2/s */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ClutchStrength"))
	double ClutchStrength;
};
