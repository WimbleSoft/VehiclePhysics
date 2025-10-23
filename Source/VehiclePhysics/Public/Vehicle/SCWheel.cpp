#pragma once

#include "SCWheel.h"
#include "Vehicle.h"
#include "SCAxis.h"
#include "Kismet/KismetStringLibrary.h"
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
	USCAxis* Axis = nullptr;
	GetAxis(Axis);
	if (Axis)
	{
		AxisSetup = Axis->AxisSetup;
	}
}

void USCWheel::GetSuspensionSetup(FSSuspension& Suspension)
{
	USCAxis* Axis = nullptr;
	GetAxis(Axis);
	if (Axis)
	{
		Suspension = Axis->AxisSetup.Suspension;
	}
}

void USCWheel::GetWheelSetup(FSWheel& Wheel)
{
	USCAxis* Axis = nullptr;
	GetAxis(Axis);
	if (Axis)
	{
		Wheel = Axis->AxisSetup.Wheel;
	}
	
}

void USCWheel::DrawDebugLines()
{
}

void USCWheel::GetWheelLinearVelocityLocal(FVector& WheelLinearVelocityLocal)
{
}

FText USCWheel::GetSuspensionTag()
{
	USCAxis* Axis = nullptr;
	GetAxis(Axis);
	if (Axis)
	{
		FString AxisName = UKismetStringLibrary::GetSubstring(Axis->AxisSetup.AxisName.ToString(), 10, 1);
		AxisName.Append(IsLeft ? "L" : "R");

		return FText::FromString(AxisName);
	}
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
	USCAxis* Axis = nullptr;
	GetAxis(Axis);
	if (Axis)
	{
		AVehicle* Vehicle = nullptr;
		Axis->GetVehicle(Vehicle);
		if (Vehicle)
		{
			VehiclePhysics = Vehicle->ACVehiclePhysics;
		}
	}
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

void USCWheel::SetBrakeTorque(double BrakeValue, bool bHandbrakeValue)
{
}

