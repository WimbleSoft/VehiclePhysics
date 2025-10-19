// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/SCAxis.h"

// Sets default values for this component's properties
USCAxis::USCAxis()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
}

void USCAxis::CreateSuspensionsX()
{
}

void USCAxis::GetVehicleX(AVehicle*& CarActor)
{
}

void USCAxis::CalcFrictionTorqueFeedbackRatioBiasX(USCWheel* Suspension, UPARAM(ref)TArray<USCWheel*>& AllSuspensions, double& OutputPin)
{
}

void USCAxis::GetCurrentAxisVelocityX(double& AxisVelocity, double TotalAngularVelocity)
{
}

void USCAxis::SetAxisDriveTorqueX(UPARAM(ref) double& TotalDriveTorque)
{
}

void USCAxis::CalcWheelDriveTorqueX(double AxisTractionTorqueL)
{
}

void USCAxis::PrintDebugX()
{
}

void USCAxis::GetCurrentAxisFrictionTorqueX(double& AxisFrictionTorqueL, double TotalAxisFrictionTorqueL)
{
}

void USCAxis::GetCurrentAxisTractionTorqueX(double& AxisTractionTorqueL, double TotalAxisTractionTorqueL)
{
}

