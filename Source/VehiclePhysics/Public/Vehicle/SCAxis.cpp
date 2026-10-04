// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SCAxis.h"
#include "Vehicle.h"
#include "SCWheel.h"

// Sets default values for this component's properties
USCAxis::USCAxis()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AxisDriveTorqueRatio = 0.0;
	AxisTractionTorque = 0.0;

	AxisSetup = FSAxis();
}


// Called when the game starts
void USCAxis::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USCAxis::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	PrintDebug();
}

void USCAxis::Construct()
{
	CreateSuspensions();
}

void USCAxis::CreateSuspensions()
{
	AVehicle* Vehicle = GetVehicle();

	if (Vehicle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("USCAxis::CreateSuspensions: Vehicle is nullptr"));
		return;
	}

	double LeftWheelTorqueRatio = AxisSetup.LeftRightSplit * AxisSetup.EfficiencyRatio;
	Vehicle->CreateSuspension(true, this, LeftWheelTorqueRatio, Suspensions.AddDefaulted_GetRef());

	if (AxisSetup.IsLeftRight) {

		double RightWheelTorqueRatio = (1.0 - AxisSetup.LeftRightSplit) * AxisSetup.EfficiencyRatio;
		Vehicle->CreateSuspension(false, this, RightWheelTorqueRatio, Suspensions.AddDefaulted_GetRef());
	}

	//Call Construct for each Suspension
	for (USCWheel* Suspension : Suspensions)
	{
		Suspension->ConstructSuspension();
	}

}

AVehicle* USCAxis::GetVehicle()
{
	return Cast<AVehicle>(this->GetAttachParent()->GetOwner());
}

void USCAxis::CalcFrictionTorqueFeedbackRatioBias(USCWheel* Suspension, UPARAM(ref)TArray<USCWheel*>& AllSuspensions, double& OutputPin)
{
	OutputPin = Suspension->WheelFrictionTorque;
}

double USCAxis::GetCurrentAxisVelocity()
{
	double AxisVelocity = 0.0;
	for(USCWheel* Suspension : Suspensions)
	{
		AxisVelocity += Suspension->WheelAngularVelocity;
	}

	return AxisVelocity;
}

void USCAxis::SetAxisDriveTorque(UPARAM(ref) double& InTotalDriveTorque)
{
	AxisTractionTorque = AxisDriveTorqueRatio * InTotalDriveTorque;
	CalcWheelDriveTorque(AxisTractionTorque);
}

void USCAxis::CalcWheelDriveTorque(double InAxisTractionTorque)
{
	for(USCWheel* Suspension : Suspensions)
	{
		//Calculate Friction Torque Feedback and set ratio to transfer this specific wheel.
		double LimitedSlipTorqueRatio = 0;
		CalcFrictionTorqueFeedbackRatioBias(Suspension, Suspensions, LimitedSlipTorqueRatio);

		double AxisTorqueRatio = AxisSetup.DifferentialType == EDifferentialType::LimitedSlip ? LimitedSlipTorqueRatio : static_cast<double>(1) / Suspensions.Num();

		Suspension->WheelTractionTorque = InAxisTractionTorque * AxisTorqueRatio;
	}
}

void USCAxis::PrintDebug()
{
	AVehicle* Vehicle = GetVehicle();
	double DriveTorque = AxisTractionTorque;               // AT
	double FrictionTorque = GetCurrentAxisFrictionTorque();// FT
	double AxisVelRadS = GetCurrentAxisVelocity();// AV

	double AxisVelRpm = 0;// ARPM

	if (Vehicle != nullptr)
	{
		AxisVelRpm = Vehicle->ACVehiclePhysics->RadSToRpm(AxisVelRadS);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USCAxis::PrintDebug: Vehicle is nullptr"));
	}
	
	const FText Msg = FText::Format(
		NSLOCTEXT("AxisDbg", "Fmt",
			"{0}_DriveTorque = {1} N·m\n"
			"{0}_FrictionTorque = {2} N·m\n"
			"{0}_Velocity = {3} rad/s = {4} RPM"),
		FText::FromName(AxisSetup.AxisName),          // {0} -> AN
		FText::AsNumber(DriveTorque),                 // {1} -> AT
		FText::AsNumber(FrictionTorque),              // {2} -> FT
		FText::AsNumber(AxisVelRadS),                 // {3} -> AV
		FText::AsNumber(AxisVelRpm)                   // {4} -> ARPM
	);

	// Print to screen (same as BP PrintText: green, 0 duration = one frame; set >0 to persist)
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, /*Key*/ 0.0f, FColor::Green, Msg.ToString(), /*bNewerOnTop*/ true, FVector2D::ZeroVector);
	}

	// Also log (optional)
	UE_LOG(LogTemp, Log, TEXT("%s"), *Msg.ToString());
}

double USCAxis::GetCurrentAxisFrictionTorque()
{
	double AxisFrictionTorqueL = 0;
	for(USCWheel* Suspension : Suspensions)
	{
		AxisFrictionTorqueL += Suspension->WheelFrictionTorque * Suspension->WheelTorqueRatio;
	}

	return AxisFrictionTorqueL;
}

double USCAxis::GetCurrentAxisTractionTorque()
{
	double AxisTractionTorqueL = 0;

	for(USCWheel* Suspension : Suspensions)
	{
		AxisTractionTorqueL += Suspension->WheelTractionTorque * Suspension->WheelTorqueRatio;
	}

	return AxisTractionTorqueL;
}

