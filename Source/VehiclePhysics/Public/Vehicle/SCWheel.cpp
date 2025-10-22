// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SCAxis.h"
#include "SCWheel.h"

// Sets default values for this component's properties
USCWheel::USCWheel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	USCWheel::W_RayTotalDegree = 120;
	USCWheel::W_RayCountPerDepth = 14;
	USCWheel::W_RayYaxisDepth = 2;

}


// Called when the game starts
void USCWheel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USCWheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USCWheel::RadialMultiRayCast()
{
}

void USCWheel::SetSuspensionForce()
{
}

void USCWheel::ApplySuspensionForce()
{
}

void USCWheel::ResetHitValues()
{
}

void USCWheel::GetAxis(USCAxis*& Axis)
{
	Axis = Cast<USCAxis>(this->GetAttachParent());
}

void USCWheel::GetAxisSetup(FSAxis& AxisSetup)
{
}

void USCWheel::GetSuspensionSetup(FSSuspension& Suspension)
{
}

void USCWheel::GetWheelSetup(FSWheel& Wheel)
{
}

void USCWheel::DrawDebugLines()
{
}

void USCWheel::GetWheelLinearVelocityLocal(FVector& WheelLinearVelocityLocal)
{
}

FText USCWheel::GetSuspensionTag()
{
	return FText();
}

void USCWheel::PrintDebug()
{
}

void USCWheel::SetFrictionForces()
{
}

void USCWheel::SetWheelVisualRelativeRotation()
{
}

void USCWheel::GetWheelInertia(double& WheelInertia)
{
}

void USCWheel::SetWheelAngularVelocity()
{
}

void USCWheel::GetLongSlipVelocity(double& LongSlipVelocity)
{
}

void USCWheel::SetTireForceCombined()
{
}

void USCWheel::GetVehiclePhysics(UACVehiclePhysics*& VehiclePhysics)
{
}

void USCWheel::ApplyTireForce()
{
}

void USCWheel::ApplyBrakeTorque()
{
}

void USCWheel::WheelAcceleration()
{
}

void USCWheel::SetTractionTorque()
{
}

void USCWheel::SetFrictionTorque()
{
}

void USCWheel::SetBrakeTorque(double BrakeValue, bool HandbrakeValue)
{
}

