#pragma once

#include "SEngine.h"
#include "SFuel.h"
#include "SClutch.h"
#include "SGearBox.h"
#include "STransmission.h"
#include "STurbo.h"
#include "SNitro.h"
#include "SBrake.h"
#include "SAxis.h"
#include "Engine/DataTable.h"

#include "SMechanicalData.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSMechanicalData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "EngineData", MakeStructureDefaultValue = "(EngineType=I4,IsEngineStarted=False,TorqueCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/EngineTorqueCurve.EngineTorqueCurve'\",EngineVolume=2.000000,IdleRPM=750.000000,CurrentRPM=0.000000,MaxRPM=8000.000000,EnergyCoefficient=0.750000,FlywheelRadius=0.500000,FlywheelMass=2.400000,RevLimiterDuration=0.050000,CurrentTemperature=0.000000,AvgTemp=70.000000,MaxTemp=110.000000,EngineHealth=100.000000)"))
	FSEngine EngineData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "FuelData", MakeStructureDefaultValue = "(FuelType=Gasoline,FuelLeft=10.000000,FuelCapacity=50.000000,FuelConsumptionCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/FuelConsumptionCurve.FuelConsumptionCurve'\")"))
	FSFuel FuelData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ClutchData", MakeStructureDefaultValue = "(Efficiency=0.980000,ScaleCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/ClutchCurve.ClutchCurve'\",ClutchInertia=5.000000,ClutchCapacity=750.000000,ClutchSpringStiffness=0.020000,AutoClutchLockCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/ClutchLockCurve.ClutchLockCurve'\")"))
	FSClutch ClutchData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "GearBoxData", MakeStructureDefaultValue = "(GearBoxType=Sequential,Efficiency=1.000000,GearRatios=((GearName=\"R\",GearRatio=-3.382000,DownRatio=0.250000,UpRatio=0.750000),(GearName=\"N\",DownRatio=0.150000,UpRatio=0.150000),(GearName=\"1\",GearRatio=2.906000,DownRatio=0.250000,UpRatio=0.750000),(GearName=\"2\",GearRatio=1.902000,DownRatio=0.250000,UpRatio=0.750000),(GearName=\"3\",GearRatio=1.308000,DownRatio=0.250000,UpRatio=0.750000),(GearName=\"4\",GearRatio=1.000000,DownRatio=0.250000,UpRatio=0.750000),(GearName=\"5\",GearRatio=0.864000,DownRatio=0.250000,UpRatio=0.750000)),FinalDriveRatio=4.444000,GearChangeLatency=0.500000,ClutchStrength=10.000000)"))
	FSGearBox GearBoxData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TransmissionData", MakeStructureDefaultValue = "(DriveType=AWD,AWDDriveSplit=0.500000)"))
	FSTransmission TransmissionData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "TurboData", MakeStructureDefaultValue = "(TurboPressure=0.000000,TurboPressureCapacity=30.000000,IsTurboReleased=True,TurboCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/TurboCurve.TurboCurve'\")"))
	FSTurbo TurboData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "NitroData", MakeStructureDefaultValue = "(NitroLeft=5.000000,NitroCapacity=10.000000,NitroCurve=\" / Script / Engine.CurveFloat'/Game/Blueprints/Curves/NitroCurve.NitroCurve'\")"))
	FSNitro NitroData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "BrakeData", MakeStructureDefaultValue = "(SplitBias=1.000000,Radius=30.000000,SinglePistonForce=1500.000000,PistonCount=4.000000,FrictionCoefficient=1.000000)"))
	FSBrake BrakeData;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "AxisData"))
	TArray<FSAxis> AxisData;
};
