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

void USCAxis::CreateSuspensions()
{
}

void USCAxis::GetVehicle(AVehicle*& CarActor)
{
	CarActor = Cast<AVehicle>(this->GetAttachParent()->GetOwner());
}

void USCAxis::CalcFrictionTorqueFeedbackRatioBias(USCWheel* Suspension, UPARAM(ref)TArray<USCWheel*>& AllSuspensions, double& OutputPin)
{
}

void USCAxis::GetCurrentAxisVelocity(double& AxisVelocityP, double TotalAngularVelocityP)
{
}

void USCAxis::SetAxisDriveTorque(UPARAM(ref) double& TotalDriveTorqueP)
{
}

void USCAxis::CalcWheelDriveTorque(double AxisTractionTorqueL)
{
}

void USCAxis::PrintDebug()
{
}

void USCAxis::GetCurrentAxisFrictionTorque(double& AxisFrictionTorqueL, double TotalAxisFrictionTorqueL)
{
}

void USCAxis::GetCurrentAxisTractionTorque(double& AxisTractionTorqueL, double TotalAxisTractionTorqueL)
{
}

