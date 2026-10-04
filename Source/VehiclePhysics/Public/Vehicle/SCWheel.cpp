#pragma once

#include "SCWheel.h"
#include "Vehicle.h"
#include "SCAxis.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetTextLibrary.h>

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

void USCWheel::ConstructSuspension()
{
	const FSSuspension SuspensionSetup = GetSuspensionSetup();
	
	LastSpringLength = SuspensionSetup.RestLength;
	CurrentSpringLength = SuspensionSetup.RestLength;
}

void USCWheel::UpdatePhysics(const double& InDeltaTime)
{
	DeltaTimer = InDeltaTime;

	SetRelativeLocation(FVector(0, 0, SteeringAngle));
	RadialMultiRayCast();
	SetSuspensionForce();
	ApplySuspensionForce();
	SetTireForceCombined();
	SetFrictionForces();
	SetTractionTorque();
	ApplyBrakeTorque();
	SetFrictionTorque();
	ApplyTireForce();
	//SetWheelAngularVelocity();
	ResetHitValues();
	DrawDebugLines();
	PrintDebug();
	SetWheelVisualRelativeRotation();


}

void USCWheel::RadialMultiRayCast()
{
	bool bRayCountEven = FMath::Fmod(W_RayCountPerDepth, 2.0) == 0;
	bool bYDepthEven = (W_RayYaxisDepth % 2) == 0;

	if (!(bRayCountEven && bYDepthEven))
	{
		// BP PrintString (red): "RayCount and RayYAxisDepth's %2 must be equal to 0"
		UKismetSystemLibrary::PrintString(
			this,
			TEXT("RayCount and RayYAxisDepth's %2 must be equal to 0"),
			/*bPrintToScreen*/true,
			/*bPrintToLog*/false,
			FLinearColor::Red
		);
		return;
	}

	// --- Outer ForLoop: y = - (W_RayYaxisDepth / 2)  ..  + (W_RayYaxisDepth / 2) ---
	int32 HalfY = W_RayYaxisDepth / 2;

	// --- Inner ForLoop: XIndex = -trunc(W_RayCountPerDepth / 2.0) .. +trunc(W_RayCountPerDepth / 2.0) ---
	int32 HalfX = FMath::TruncToInt(W_RayCountPerDepth / 2.0);

	for (int32 YIndex = -HalfY; YIndex <= HalfY; ++YIndex)
	{
		for (int32 XIndex = -HalfX; XIndex <= HalfX; ++XIndex)
		{
			// BP branch (Index == 0) ultimately feeds the same chain; the “else” explicitly writes the variable.
			// Compute WheelRelativeLocation:
			//   WheelRelativeLocation = GetUpVector()*CurrentSpringLength - GetYAxisOffsetToRayStartForAxisSlopeAngle()
			FVector TempWheelRelativeLocation = GetUpVector() * CurrentSpringLength - GetYAxisOffsetToRayStartForAxisSlopeAngle();
			if (XIndex != 0)
			{
				WheelRelativeLocation = TempWheelRelativeLocation;
			}

			// WheelCenterWorld = K2_GetComponentLocation() - WheelRelativeLocation
			FVector ComponentLoc = GetComponentLocation();
			WheelCenterWorld = ComponentLoc - TempWheelRelativeLocation;

			// Ray start = WheelCenterWorld + RotateAroundX( GetYAxisDepthOffset(YIndex) )
			FVector YDepthOffset = GetYAxisDepthOffset(/*RayIndexOnYAxis=*/YIndex);
			//const FVector RayStartVector = GetRotateRayStartAroundZAxis(YDepthOffset);
			//const FVector StartOffsetCamber = GetRotateRayStartOffsetVectorAroundXAxis(RayStartVector);
			FVector StartOffsetCamber = GetRotateRayStartOffsetVectorAroundXAxis(YDepthOffset);
			//Wheel Location with Steering rotation applied on raystart offsets of specified Ray index at Y axis.
			FVector Start = WheelCenterWorld + StartOffsetCamber;

			FVector RayEndVector = GetRotateRayEndPointAroundYAxis(YIndex);
			//const FVector Tz = GetRotateRayEndAroundZAxis(T);
			FVector End = Start - GetRotateRayEndVectorOffsetAroundXAxis(RayEndVector);

			// Debug color from the first-loop index chain in BP
			FLinearColor TraceColor = GetTraceColorByRayCurrentYAxisDepthIndex(YIndex);

			// Do the trace
			CreateLineTraceForHitResults(TraceColor, Start, End);
		}
	}

	SetSpringLength();
}

void USCWheel::SetSpringLength()
{
	// 1) Array_Contains(True)
	bWheelContact = WheelContacts.Contains(true);
	const FSWheel Wheel = GetWheelSetup();
	const FSSuspension Spring = GetSuspensionSetup();

	if (bWheelContact)
	{
		// Guard: arrays must be valid/non-empty
		if (HitDepthArray.Num() == 0 || HitResults.Num() == 0)
		{
			bWheelContact = false; // fall through to else-branch
		}
		else
		{
			// MaxOfFloatArray => index of deepest hit
			int IndexOfMax = 0;
			float MaxVal = 0;
			UKismetMathLibrary::MaxOfFloatArray(HitDepthArray, IndexOfMax, MaxVal);

			if (HitResults.IsValidIndex(IndexOfMax))
			{
				// Set HitResult = HitResults[idx]
				HitResult = HitResults[IndexOfMax];

				// Compute spring length:
				// center - (hit.location + (UpVector * Wheel.Radius)) -> magnitude
				FVector Center = GetComponentLocation();
				FVector Contact = HitResult.Location + GetUpVector() * Wheel.Radius;
				CurrentSpringLength = (Center - Contact).Size();

				// HitDepthVector & MaxHitDepth
				MaxHitDepth = (HitResult.Location - HitResult.TraceEnd).Size();
				HitDepthVector = HitResult.Location - HitResult.TraceEnd;
			}
			else
			{
				bWheelContact = false; // invalid index -> treat as no contact
			}
		}
	}

	if (!bWheelContact)
	{
		// No contact (air): spring release model
		float DeltaTime = DeltaTimer;                  // typically GetWorld()->GetDeltaSeconds()
		const float Target = Spring.RestLength;

		// ΔXIndex = RestLength - LastSpringLength
		float DeltaX = Spring.RestLength - LastSpringLength;

		// Force ≈ ΔXIndex * SpringRate
		float AucumulatedSpringForce = DeltaX * Spring.SpringStiffness;

		// a = F / m   (clamp mass to avoid div-by-zero)
		float Acceleration = AucumulatedSpringForce / FMath::Max(Wheel.Mass, 1.0f);

		// InterpSpeed = a * Δt
		float InterpSpeed = Acceleration * DeltaTime;

		// CurrentSpringLength = FInterpTo(Current, Target, Δt, InterpSpeed)
		CurrentSpringLength = FMath::FInterpTo(CurrentSpringLength, Target, DeltaTime, InterpSpeed);

		// LastSpringLength = CurrentSpringLength (matches Set->Output_Get wiring)
		LastSpringLength = CurrentSpringLength;

		// MakeHitResult() — reset
		/*HitResult = FHitResult();

		MaxHitDepth = 0.0f;
		HitDepthVector = FVector::ZeroVector;*/
	}
}

void USCWheel::CreateLineTraceForHitResults(FLinearColor TraceColor, const FVector Start, const FVector End)
{
	FHitResult OutHit;
	const TArray<AActor*> ActorsToIgnore; // BP pin left unconnected

	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		/*bTraceComplex=*/true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		/*bIgnoreSelf=*/true,
		TraceColor,
		FLinearColor::Black,
		/*DrawTime=*/0.0f
	);

	if (bHit)
	{
		WheelContacts.Add(true);
		HitResults.Add(OutHit);
		HitLocations.Add(OutHit.Location);

		// "Difference between 'wheel can't pass' point and real hit" -> (Location - End)
		HitDepthArray.Add((OutHit.Location - End).Z);
	}
	// BP 'else' does nothing (no false added)
}

FLinearColor USCWheel::GetTraceColorByRayCurrentYAxisDepthIndex(int32 InCurrentYAxisDepthIndex) const
{
	// Map InCurrentYAxisDepth from [-W/2, +W/2] → [0, W], clamped
	const int32  Half = W_RayYaxisDepth / 2;
	const double mapped =
		FMath::GetMappedRangeValueClamped(
			FVector2D(-double(Half), double(Half)),
			FVector2D(0.0, double(W_RayYaxisDepth)),
			double(InCurrentYAxisDepthIndex));

	// Truncate toward zero (Blueprint used FTrunc)
	int32 Index = FMath::TruncToInt(mapped);

	// The Select node has 9 options (0..8). Make sure we stay in range.
	Index = FMath::Clamp(Index, 0, 8);

	switch (Index)
	{
		case 0: return FLinearColor(0.f, 1.f, 0.f, 1.f);       // green
		case 1: return FLinearColor(1.f, 0.f, 0.f, 1.f);       // red
		case 2: return FLinearColor(0.f, 0.7f, 1.f, 1.f);      // cyan-ish
		case 3: return FLinearColor(1.f, 0.f, 0.75f, 1.f);     // magenta-ish
		case 4: return FLinearColor(1.f, 1.f, 0.f, 1.f);       // yellow
		case 5: return FLinearColor(1.f, 1.f, 1.f, 1.f);       // white
		case 6: return FLinearColor(0.f, 0.f, 1.f, 1.f);       // blue
		case 7: return FLinearColor(0.086115f, 0.f, 1.f, 1.f); // violet-blue
		case 8: default: return FLinearColor(0.f, 0.f, 0.f, 0.f); // (Option 8 had no default in dump)
	}
}

FVector USCWheel::GetRotateRayEndPointAroundYAxis(int32 RayIndexOnXAxis)
{
	// BP: Delta RayDegree = W_RayTotalDegree / W_RayCountPerDepth
	double DeltaRayDegree = (W_RayCountPerDepth != 0.0) ? (W_RayTotalDegree / W_RayCountPerDepth) : 0.0;

	// BP: RayDegree = RayIndexOnXAxis * DeltaRayDegree
	double AngleDeg = static_cast<double>(RayIndexOnXAxis) * DeltaRayDegree;

	// BP: InVect = GetUpVector() * Wheel.Radius
	const FSWheel Wheel = GetWheelSetup();  // SWheel struct (Radius in cm)
	FVector InVect = GetUpVector() * Wheel.Radius;

	// BP: Axis = GetRightVector()  (local Y axis)
	FVector Axis = GetRightVector();

	// BP: RotateAngleAxis(InVect, AngleDeg, Axis)
	return InVect.RotateAngleAxis(AngleDeg, Axis);
}

FVector USCWheel::GetRotateRayEndVectorOffsetAroundXAxis(FVector RayEndVector)
{
	// BP: CamberAngle comes from Wheel setup (degrees)
	const FSWheel Wheel = GetWheelSetup();
	const double CamberDeg = static_cast<double>(Wheel.CamberAngle);

	// BP: SelectFloat( A = -Camber, B = Camber, bPickA = bIsLeft )
	const double AngleDeg = bIsLeft ? -CamberDeg : CamberDeg;

	// BP: Axis = GetForwardVector()  (local X axis)
	FVector Axis = GetForwardVector();

	// BP: RotateAngleAxis(InVect=RayEndVector, AngleDeg=AngleDeg, Axis=Forward)
	return RayEndVector.RotateAngleAxis(AngleDeg, Axis);
}

FVector USCWheel::GetRotateRayEndAroundZAxis(FVector RayEndVector)
{
	// BP: AngleDeg comes from SteeringAngle (degrees)
	double AngleDeg = SteeringAngle;

	// BP: Axis = GetUpVector()  (local Z axis)
	FVector Axis = GetUpVector();

	// BP: RotateAngleAxis(InVect=RayEndVector, AngleDeg=SteeringAngle, Axis=Up)
	return RayEndVector.RotateAngleAxis(static_cast<float>(AngleDeg), Axis);
}

FVector USCWheel::GetRotateRayStartAroundZAxis(FVector RayStartVector)
{
	// BP: SteeringAngle (degrees)
	const double AngleDeg = SteeringAngle;

	// BP: Axis = GetUpVector()  (local Z axis)
	const FVector Axis = GetUpVector();

	// BP: RotateAngleAxis(InVect=RayStartVector, AngleDeg=SteeringAngle, Axis=Up)
	return RayStartVector.RotateAngleAxis(static_cast<float>(AngleDeg), Axis);
}

FVector USCWheel::GetYAxisDepthOffset(int32 RayIndexOnYAxis)
{
	// BP: GetWheelSetup -> Break -> Width
	const FSWheel Wheel = GetWheelSetup();
	const double WidthCm = static_cast<double>(Wheel.Width);   // cm

	// BP: W_RayYaxisDepth (int)
	const int32 DepthCount = W_RayYaxisDepth;

	// BP: DeltaRayYpos = Width / W_RayYaxisDepth
	// (Guard against 0 just in case; BP Divide node would return 0/NaN otherwise)
	const double DeltaRayPosY = (DepthCount != 0) ? WidthCm / static_cast<double>(DepthCount) : 0.0;

	// BP: scale = double(RayIndexOnYAxis) * DeltaRayPosY
	const double Scale = static_cast<double>(RayIndexOnYAxis) * DeltaRayPosY;

	// BP: Return GetRightVector * scale
	return GetRightVector() * static_cast<float>(Scale);
}

FVector USCWheel::GetRotateRayStartOffsetVectorAroundXAxis(FVector RayStartVector)
{
	// BP: GetWheelSetup->Break->CamberAngle
	const FSWheel Wheel = GetWheelSetup();

	const float CamberAngleDeg = Wheel.CamberAngle; // degrees

	// BP: SelectFloat( A = Camber * -1, B = Camber, bPickA = bIsLeft )
	const float AngleDeg = bIsLeft ? -CamberAngleDeg : CamberAngleDeg;

	// BP: Axis = GetForwardVector()  (local X axis)
	FVector Axis = GetForwardVector();

	// BP: RotateAngleAxis(InVect=RayStartVector, AngleDeg=AngleDeg, Axis=Axis)
	return RayStartVector.RotateAngleAxis(AngleDeg, Axis);
}

FVector USCWheel::GetYAxisOffsetToRayStartForAxisSlopeAngle()
{
	// BP: GetAxisSetup -> Break -> AxisSlopeAngle
	const FSAxis AxisSetup = GetAxisSetup();

	const double AxisSlopeDeg = static_cast<double>(AxisSetup.AxisSlopeAngle);

	// BP: SelectFloat  (bIsLeft ? -AxisSlopeAngle : AxisSlopeAngle)
	const double SignedAngleDeg = bIsLeft ? -AxisSlopeDeg : AxisSlopeDeg;

	// BP: DegSin( angle ) * CurrentSpringLength
	double Scale = FMath::Sin(SignedAngleDeg) * CurrentSpringLength;

	// BP: GetRightVector * (that scale)
	return GetRightVector() * Scale;
}

void USCWheel::SetSuspensionForce()
{
	const FSSuspension SuspensionSetup = GetSuspensionSetup();

	double SpringForce = SuspensionSetup.SpringStiffness * (SuspensionSetup.RestLength - CurrentSpringLength);
	double DamperForce = SuspensionSetup.DamperStiffness * ((LastSpringLength - CurrentSpringLength) / DeltaTimer);

	Fz = FMath::Max(SpringForce + DamperForce, 0);

	int IndexOfMax = 0;
	float MaxHitDepthL = 0.0f;
	UKismetMathLibrary::MaxOfFloatArray(HitDepthArray, IndexOfMax, MaxHitDepthL);

	FHitResult HitDepth = HitResults.IsValidIndex(IndexOfMax) ? HitResults[IndexOfMax] : FHitResult();
	FVector FzNormal = HitDepth.Normal;
	FzNormal.Normalize();

	FzVec = FzNormal * Fz * 100.0;

	//Update last spring length for next frame
	LastSpringLength = CurrentSpringLength;
}

void USCWheel::ApplySuspensionForce()
{
	USCAxis* Axis = GetAxis();

	if (Axis)
	{
		AVehicle* Vehicle = Axis->GetVehicle();
		if (Vehicle)
		{
			//Apply Suspension Force from wheel center location to vehicle body
			Vehicle->VehicleBody->AddForceAtLocation(FzVec, WheelCenterWorld + GetUpVector() * CurrentSpringLength);

			auto RpmRatio = Vehicle->ACVehiclePhysics->MechanicalData.EngineData.CurrentRPM / Vehicle->ACVehiclePhysics->MechanicalData.EngineData.MaxRPM;

			//Apply Extra Force at -Z axis on Engine Socket when Engine Compression ratio is more than 0,99
			Vehicle->VehicleBody->AddForceAtLocation(Vehicle->VehicleBody->GetSocketLocation("Engine"), FVector(0, 0, RpmRatio * (RpmRatio > 0.99 ? 1 : 0) * -10000.0));
			
		}
	}
}

void USCWheel::ResetHitValues()
{
	HitLocations.Empty();
	WheelContacts.Empty();
	HitResults.Empty();
	HitDepthArray.Empty();

	Fx = 0.0;
	Fy = 0.0;
	Fz = 0.0;
	FxVec = FVector::ZeroVector;
	FyVec = FVector::ZeroVector;
	FzVec = FVector::ZeroVector;

}

USCAxis* USCWheel::GetAxis() const
{
	return Cast<USCAxis>(this->GetAttachParent());
}

FSAxis USCWheel::GetAxisSetup() const
{
	return GetAxis()->AxisSetup;
}

FSSuspension USCWheel::GetSuspensionSetup() const
{
	return GetAxisSetup().Suspension;
}

FSWheel USCWheel::GetWheelSetup() const
{
	return GetAxis()->AxisSetup.Wheel;	
}

void USCWheel::DrawDebugLines()
{
#if UE_BUILD_DEVELOPMENT
	UWorld* World = GetWorld();
	if (!World) return;

	// --- Nodes: GetComponentLocation, WheelCenterWorld, CurrentSpringLength / RestLength -> grayscale color ---
	FVector CompLoc = GetComponentLocation();
	FVector WheelCenter = WheelCenterWorld;
	const FSSuspension SuspensionSetup = GetSuspensionSetup();

	double Ratio = (SuspensionSetup.RestLength != 0.f) ? (CurrentSpringLength / SuspensionSetup.RestLength) : 0.0;

	FLinearColor Gray(static_cast<float>(Ratio), static_cast<float>(Ratio), static_cast<float>(Ratio), 1.0f);

	// Draw: component location -> wheel center (gray)
	UKismetSystemLibrary::DrawDebugLine(this, CompLoc, WheelCenter, Gray, 0.0f, 5.0f);

	// Utility: safe divide for doubles
	auto SafeDiv = [](double A, double B) -> double
		{
			return FMath::IsNearlyZero(static_cast<float>(B)) ? 0.0 : (A / B);
		};

	// --- Red line: WheelCenter -> WheelCenter + UpVector * ((Fz/Fz) * 100) ---
	double FzVal = Fz;
	float UpScale = static_cast<float>(SafeDiv(FzVal, FzVal) * 100.0); // effectively 100 if Fz != 0
	FVector UpVecDraw = GetUpVector() * UpScale;

	UKismetSystemLibrary::DrawDebugLine(this, WheelCenter, WheelCenter + UpVecDraw, FLinearColor::Red, 0.0f, 5.0f);

	// --- Green line: WheelCenter -> WheelCenter + (FyVec / Fz) * 100 ---
	float FyScaleDen = FMath::IsNearlyZero(static_cast<float>(FzVal)) ? 1.0f : static_cast<float>(FzVal);
	FVector FyDraw = (FyVec / FyScaleDen) * 100.0f;

	UKismetSystemLibrary::DrawDebugLine(this, WheelCenter, WheelCenter + FyDraw, FLinearColor::Green, 0.0f, 5.0f);

	// --- Blue line: WheelCenter -> WheelCenter + (FxVec / Fz) * 100 ---
	FVector FxDraw = (FxVec / FyScaleDen) * 100.0f;

	UKismetSystemLibrary::DrawDebugLine(this, WheelCenter, WheelCenter + FxDraw, FLinearColor::Blue, 0.0f, 5.0f);
#endif
}

FVector USCWheel::GetWheelLinearVelocityLocal()
{
	FVector WheelLinearVelocityLocal = FVector::ZeroVector;
	USCAxis* Axis = GetAxis();
	if (Axis)
	{
		AVehicle* Vehicle = Axis->GetVehicle();
		if (Vehicle)
		{
			FVector WheelVelocity = Vehicle->VehicleBody->GetPhysicsLinearVelocityAtPoint(HitResult.Location);
			FVector HitSurfaceVelocity;
			TWeakObjectPtr<UPrimitiveComponent> HitComponent = HitResult.Component;
			if (HitComponent.IsValid())
			{
				HitSurfaceVelocity = HitComponent->GetPhysicsLinearVelocityAtPoint(HitResult.Location);
			}

			FVector RelativeVelocity = HitSurfaceVelocity - WheelVelocity;
			//m/s
			FVector WheelForwardVelocity = HitComponent.IsValid() ? RelativeVelocity / 100 : WheelVelocity / 100;
						
			//GetWorldTransform for this scene component
			FTransform WorlTransform = this->GetComponentTransform();
			WheelLinearVelocityLocal = UKismetMathLibrary::InverseTransformDirection(WorlTransform, WheelForwardVelocity);
		}
	}

	return WheelLinearVelocityLocal;
}

FText USCWheel::GetSuspensionTag() const
{
	USCAxis* Axis = GetAxis();
	if (Axis)
	{
		FString AxisName = UKismetStringLibrary::GetSubstring(Axis->AxisSetup.AxisName.ToString(), 10, 1);
		AxisName.Append(bIsLeft ? "L" : "R");

		return FText::FromString(AxisName);
	}
	return FText();
}

namespace
{
	static constexpr double RPM_TO_RAD_PER_SEC = PI / 30.0;          // ω = RPM * 2π / 60 = π / 30
}

void USCWheel::PrintDebug()
{
	// -----------------------------
	// Sequence(6 outputs) in BP
	// then_0, then_1, then_2, then_3, then_4, then_5
	// We’ll just execute them in that order.
	// -----------------------------
	USCAxis* Axis = GetAxis();
	AVehicle* Vehicle = Axis->GetVehicle();

	if (Axis && Vehicle)
	{
		// ---------- then_0 : "Steering Angle {SuspensionName} = {Angle}"
		{
			// GetAttachParent(self) → GetSocketTransform( InSocketName = GetAttachSocketName(self), RTS_Actor ) → Location → BreakVector.X
			USceneComponent* AttachParentL = GetAttachParent();
			const FName SocketOnSelf = GetAttachSocketName();

			FTransform PTransform = FTransform::Identity;
			if (AttachParentL)
			{
				PTransform = AttachParentL->GetSocketTransform(SocketOnSelf, ERelativeTransformSpace::RTS_Actor);
			}
			const FVector PLoc = PTransform.GetLocation();

			// FormatText: "Steering Angle {SuspensionName} = {Angle}"
			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionName"), GetSuspensionTag());
			Args.Add(TEXT("Angle"), FText::AsNumber(PLoc.X >= 0 ? SteeringAngle : SteeringAngle * -1.0));
			const FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgSteerFmt", "Steering Angle {SuspensionName} = {Angle}"), Args);

			// PrintText (DevelopmentOnly in BP)
			UKismetSystemLibrary::PrintText(
				this,                         // WorldContext (hidden pin)
				Msg,
				/*bPrintToScreen*/ false,      // matches default overridden to false in your first PrintText
				/*bPrintToLog*/    false,      // disabled in the pasted node
				FLinearColor(0.f, 0.66f, 1.f, 1.f),
				/*Duration*/       0.f,
				/*Key*/            NAME_None);
		}

		// ---------- then_1 : "{SuspensionTag} World Velocity = {WorldVelocity}"	
		{
			// Vehicle → VehicleBody (Skeletal/Primitive) → GetPhysicsLinearVelocityAtPoint(0,0,0)
			// Raw export shows "Point" left default (0,0,0) and BoneName=None.
			USkeletalMeshComponent* VehicleBody = (Vehicle && Vehicle->VehicleBody) ? Vehicle->VehicleBody : nullptr;

			FVector WorldVelocityCms = FVector::ZeroVector;
			if (VehicleBody)
			{
				WorldVelocityCms = VehicleBody->GetPhysicsLinearVelocityAtPoint(FVector::ZeroVector);
			}

			// Divide_VectorFloat (cm/s to m/s) with B=100
			FVector WorldVelocityMs = WorldVelocityCms / 100.0f;

			// Suspension tag (same source pin as above)
			const FText SuspensionTag = FText::FromString(GetName());

			// Format
			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionTag"), SuspensionTag);
			Args.Add(TEXT("WorldVeloctiy"), UKismetTextLibrary::Conv_VectorToText(WorldVelocityMs));
			FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgWorldVelFmt", "{SuspensionTag} World Velocity = {WorldVeloctiy}"), Args);

			UKismetSystemLibrary::PrintText(
				this,
				Msg,
				/*Screen*/ true,
				/*Log*/    false,
				FLinearColor(0.880561f, 1.f, 0.f, 1.f),
				/*Duration*/ 0.f,
				NAME_None);
		}

		// ---------- then_2 : "{SuspensionTag} Local Velocity = {LocalVelocity}"
		{
			USkeletalMeshComponent* VehicleBody = (Vehicle && Vehicle->VehicleBody) ? Vehicle->VehicleBody : nullptr;

			FVector LocalVelocity = FVector::ZeroVector;
			if (VehicleBody)
			{
				// The pasted graph converts to text from a vector produced by an upstream node (id …11).
				// That node is not included in the dump, but in this pattern it’s the world velocity
				// transformed into the parent’s local space. We reproduce that exactly:
				FVector WorldVelocityCmss = VehicleBody->GetPhysicsLinearVelocityAtPoint(FVector::ZeroVector);
				const FVector WorldVelocityMs = WorldVelocityCmss / 100.0f;
				FTransform BodyXform = VehicleBody->GetComponentTransform();
				LocalVelocity = BodyXform.InverseTransformVectorNoScale(WorldVelocityMs);
			}

			const FText SuspensionTag = FText::FromString(GetName());

			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionTag"), SuspensionTag);
			Args.Add(TEXT("LocalVeloctiy"), UKismetTextLibrary::Conv_VectorToText(LocalVelocity));
			FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgLocalVelFmt", "{SuspensionTag} Local Velocity = {LocalVeloctiy}"), Args);

			UKismetSystemLibrary::PrintText(
				this,
				Msg,
				/*Screen*/ true,
				/*Log*/    false,
				FLinearColor(1.f, 0.5f, 0.f, 1.f),
				/*Duration*/ 0.f,
				NAME_None);
		}

		// ---------- then_3 : "{SuspensionTag} Rpm / Fx / Fy"
		{
			// Rpm from WheelAngularVelocity (rad/s) — the BP uses a node chain; result equals rad/s → Rpm
			const double Rpm = WheelAngularVelocity * 1 / RPM_TO_RAD_PER_SEC; // 9.549296...

			const FText SuspensionTag = FText::FromString(GetName());

			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionTag"), SuspensionTag);
			Args.Add(TEXT("WheelRPM"), FText::AsNumber(Rpm));
			Args.Add(TEXT("Fx"), FText::AsNumber(Fx));
			Args.Add(TEXT("Fy"), FText::AsNumber(Fy));

			FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgRpmForcesFmt", "{SuspensionTag} RPM = {WheelRPM} RPM\n{SuspensionTag} Fx = {Fx} N\n{SuspensionTag} Fy = {Fy} N"), Args);

			UKismetSystemLibrary::PrintText(
				this,
				Msg,
				/*Screen*/ true,
				/*Log*/    false,
				FLinearColor(1.f, 0.5f, 0.f, 1.f),
				/*Duration*/ 0.f,
				NAME_None);
		}

		// ---------- then_4 : "{SuspensionTag} Friction Torque = {FrictionTorque} Nm"
		{
			const FText SuspensionTag = FText::FromString(GetName());

			// mirror Conv_DoubleToText settings (same as brake torque formatting is fine)
			FNumberFormattingOptions FricFmt;
			FricFmt.MinimumIntegralDigits = 3;
			FricFmt.MaximumIntegralDigits = 324;
			FricFmt.MinimumFractionalDigits = 2;
			FricFmt.MaximumFractionalDigits = 2;
			FricFmt.RoundingMode = ERoundingMode::HalfToEven;

			FText FrictionTorqueTxt = FText::AsNumber(WheelFrictionTorque, &FricFmt); // uses your BP variable "FrictionTorque"

			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionTag"), SuspensionTag);
			Args.Add(TEXT("FrictionTorque"), FrictionTorqueTxt);

			FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgFricTorqueFmt", "{SuspensionTag} Friction Torque = {FrictionTorque} Nm"), Args);

			UKismetSystemLibrary::PrintText(
				this,
				Msg,
				/*Screen*/ true,
				/*Log*/    false,
				/*TextColor*/ FLinearColor(0.304931f, 1.f, 0.072683f, 1.f),
				/*Duration*/ 0.f,
				NAME_None);
		}

		// ---------- then_5 : "{SuspensionTag} Brake Torque / Bias"
		{
			const FText SuspensionTag = FText::FromString(GetName());

			// The two Conv_DoubleToText nodes in BP specify custom digit settings.
			// We mirror those using FNumberFormattingOptions.
			FNumberFormattingOptions TorqFmt;
			TorqFmt.MinimumIntegralDigits = 3;
			TorqFmt.MaximumIntegralDigits = 324;
			TorqFmt.MinimumFractionalDigits = 2;
			TorqFmt.MaximumFractionalDigits = 2;
			TorqFmt.RoundingMode = ERoundingMode::HalfToEven;

			FNumberFormattingOptions BiasFmt;
			BiasFmt.MinimumIntegralDigits = 1;
			BiasFmt.MaximumIntegralDigits = 324;
			BiasFmt.MinimumFractionalDigits = 2;
			BiasFmt.MaximumFractionalDigits = 2;
			BiasFmt.RoundingMode = ERoundingMode::HalfToEven;

			FText BrakeTorqueTxt = FText::AsNumber(WheelBrakeTorque, &TorqFmt);
			FText BrakeBiasTxt = FText::AsNumber(BrakeBias, &BiasFmt);

			FFormatNamedArguments Args;
			Args.Add(TEXT("SuspensionTag"), SuspensionTag);
			Args.Add(TEXT("BrakeTorque"), BrakeTorqueTxt);
			Args.Add(TEXT("BrakeBias"), BrakeBiasTxt);

			FText Msg = FText::Format(NSLOCTEXT("SCWheel", "DbgBrakeFmt", "{SuspensionTag} Brake Torque = {BrakeTorque} Nm\n{SuspensionTag} Brake Bias = {BrakeBias} k"), Args);

			UKismetSystemLibrary::PrintText(
				this,
				Msg,
				/*Screen*/ true,
				/*Log*/    false,
				FLinearColor(1.f, 0.f, 0.f, 1.f),
				/*Duration*/ 0.f,
				NAME_None);
		}
	}

}

void USCWheel::SetFrictionForces()
{
	int IndexOfMaxHitDepth = 0;
	float MaxHitDepthL = 0.0f;
	UKismetMathLibrary::MaxOfFloatArray(HitDepthArray, IndexOfMaxHitDepth, MaxHitDepthL);

	FHitResult MaxHitDepthResultL = HitResults.IsValidIndex(IndexOfMaxHitDepth) ? HitResults[IndexOfMaxHitDepth] : FHitResult();
	FVector WheelContactNormal = MaxHitDepthResultL.ImpactNormal;
	WheelContactNormal.Normalize();
	FxVec = UKismetMathLibrary::ProjectVectorOnToPlane(GetForwardVector(), WheelContactNormal) * Fx;
	FyVec = UKismetMathLibrary::ProjectVectorOnToPlane(GetRightVector(), WheelContactNormal) * Fy;

}

void USCWheel::SetWheelVisualRelativeRotation()
{
	// --- Break current WheelRelativeRotation (K2Node_CallFunction_4)
	float CurrRoll = WheelRelativeRotation.Roll;   // X
	float CurrPitch = WheelRelativeRotation.Pitch;  // Y
	float CurrYaw = WheelRelativeRotation.Yaw;    // Z

	// --- Pitch target = CurrPitch + RadiansToDegrees(WheelAngularVelocity) * DeltaTimer * -1
	double DegPerSecNeg = FMath::RadiansToDegrees(WheelAngularVelocity) * -1.0; // (CallFunction_20 -> Mult_5 with C=-1)
	double PitchTarget = static_cast<double>(CurrPitch) + (DegPerSecNeg * DeltaTimer); // (Add_4)

	// --- Interp pitch (FInterpTo with speed = 10) (CallFunction_23)
	float NewPitch = FMath::FInterpTo(CurrPitch, static_cast<float>(PitchTarget), static_cast<float>(DeltaTimer), 10.0f);

	// --- If NewPitch == -360 choose 0 else NewPitch (Select_2 with EqualEqual_DoubleDouble)
	if (NewPitch == -360.0f)
	{
		NewPitch = 0.0f;
	}

	// --- First Set: WheelRelativeRotation = (Roll = CurrRoll, Pitch = NewPitch, Yaw = CurrYaw) (VariableSet_2 via MakeRotator_14)
	WheelRelativeRotation = FRotator(CurrRoll, NewPitch, CurrYaw);

	// --- Second Set: keep Roll/Pitch, override Yaw with SteeringAngle (VariableSet_0 via Break_12 -> Make_6 with Yaw=SteeringAngle)
	WheelRelativeRotation.Yaw = static_cast<float>(SteeringAngle);

	// --- Apply to WheelMesh: Location.Z = -CurrentSpringLength, Rotation = (Roll = WheelRelativeRotation.Roll, Pitch = 0, Yaw = Left?0:180)
	if (WheelMesh)
	{
		FVector NewLoc(0.f, 0.f, static_cast<float>(-CurrentSpringLength));

		const float YawForMesh = bIsLeft ? 0.0f : 180.0f; // (False=180, True=0)

		WheelMesh->SetRelativeLocationAndRotation(
			NewLoc,
			FRotator(WheelRelativeRotation.Roll, 0.0f, YawForMesh),
			/*bSweep*/false,
			/*OutHit*/nullptr,
			ETeleportType::None
		);

		// --- Spin all direct children around their local Pitch (ForEachChildren -> AddLocalRotation)
		TArray<USceneComponent*> Children;
		WheelMesh->GetChildrenComponents(/*bIncludeAllDescendants*/false, Children);

		// Delta pitch per tick for child parts:
		// RadiansToDegrees(WheelAngularVelocity * -1) * DeltaTimer * (bIsLeft ? -1 : 1) (CallFunction_18, Mult_6, Select_1, Mult_3)
		double ChildDegPerFrame =
			FMath::RadiansToDegrees(WheelAngularVelocity * -1.0) *
			DeltaTimer *
			(bIsLeft ? -1.0 : 1.0);

		for (USceneComponent* Child : Children)
		{
			if (!Child) continue;
			Child->AddLocalRotation(FRotator(0.f, ChildDegPerFrame, 0.f), false, nullptr, ETeleportType::None);
		}
	}
}

double USCWheel::GetWheelInertia() const
{
	const FSAxis AxisSetup = GetAxisSetup();
	return 0.5 * (AxisSetup.Wheel.Radius / 100.0 * AxisSetup.Wheel.Radius / 100.0) * AxisSetup.Wheel.Mass;
}

void USCWheel::SetWheelAngularVelocity()
{

	const double WheelInertia = GetWheelInertia();

	UACVehiclePhysics* VehiclePhysics = nullptr;
	GetVehiclePhysics(VehiclePhysics);

	double CurrentTotalGearRatio = VehiclePhysics->GetTotalGearRatio();

	int CurrentGear = VehiclePhysics->GetCurrentGear();

	double WheelAngularAcceleration = WheelTractionTorque / WheelInertia;	
	double MaxWheelSpeedOnCurrentGear = CurrentGear != 1 ? VehiclePhysics->EngineAngularVelocity / CurrentTotalGearRatio : 99999.0;

	WheelAngularVelocity = FMath::Min(FMath::Abs(WheelAngularAcceleration * DeltaTimer + WheelAngularVelocity), FMath::Abs(MaxWheelSpeedOnCurrentGear)) * FMath::Sign(MaxWheelSpeedOnCurrentGear);
	

}

double USCWheel::GetLongSlipVelocity()
{
	FSWheel WheelSetup = GetWheelSetup();
	FVector WheelLinearVelocity = GetWheelLinearVelocityLocal();

	double LongSlipVelocity =  WheelAngularVelocity * (WheelSetup.Radius / 100.0) - WheelLinearVelocity.X;

	return LongSlipVelocity;
}

void USCWheel::SetTireForceCombined()
{
	// 0) Early exit if no valid contact/condition
	if (!bWheelContact) { 
		Fx = 0.0; 
		Fy = 0.0; 
		FxVec = FVector::ZeroVector;
		FyVec = FVector::ZeroVector;

		return; 
	}

	// 1) Lateral slip from corner stiffness island
	FVector WheelLinearVelocity = GetWheelLinearVelocityLocal();
	const FSWheel WheelSetup = GetWheelSetup();
	const FSAxis AxisSetup = GetAxisSetup();
	
	double CorneringStiffness = WheelLinearVelocity.Y * WheelSetup.CorneringStiffness * -1;
	
	LateralSlipNormalized = FMath::Clamp(CorneringStiffness, -1.0, 1.0);

	double LongSlipVelocityL = GetLongSlipVelocity();
	// 2) Traction vs Friction selection
	bool bTractionCase = WheelLinearVelocity.X * LongSlipVelocityL > 0.0;

	// 3) Longitudinal slip (two ways)
	if (bTractionCase)
	{
		//Traction; Long Slip Normalized=Clamp(TractionForce/MaxFrictionForce, -2, 2)
		const double TractionForce = (WheelTractionTorque / (AxisSetup.Wheel.Radius / 100));

		const double MaxFrictionForce = FMath::Max(Fz * HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->Friction : 1.0, 0.000001);

		LongSlipNormalized = FMath::Clamp(TractionForce / MaxFrictionForce, -2.0, 2.0);
	}
	else
	{
		//Friction; LongSlipNormalized = Clamp((LongSlipVelocity*WheelLongStiffness),-2, 2)
		LongSlipNormalized = FMath::Clamp(LongSlipVelocityL * WheelSetup.LongStiffness, -2.0, 2.0);
	}

	// 4) Build normalized tire force from slip vector and curve
	FVector2D SlipVec(LongSlipNormalized, LateralSlipNormalized);
	FVector2D SlipDir = SlipVec.GetSafeNormal();
	double SlipMag = SlipVec.Size();

	float CurveScale = AxisSetup.Wheel.ForceCurve->GetFloatValue((float)SlipMag);
	FVector2D TireForceNormalized = SlipDir * (double)CurveScale;

	// 5) Scale by load and friction
	double HitPointFrictionCoefficient = HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->Friction : 1.0;
	double FrictionForce = FMath::Max(Fz * HitPointFrictionCoefficient, 0.0);
	FVector2D TireForceVector = TireForceNormalized * FrictionForce;

	// 6) Outputs
	Fx = TireForceVector.X;
	Fy = TireForceVector.Y;
}

void USCWheel::GetVehiclePhysics(UACVehiclePhysics*& VehiclePhysics)
{
	USCAxis* Axis = GetAxis();
	if (Axis)
	{
		AVehicle* Vehicle = Axis->GetVehicle();
		if (Vehicle)
		{
			VehiclePhysics = Vehicle->ACVehiclePhysics;
		}
	}
}

void USCWheel::ApplyTireForce()
{
	USCAxis* Axis = GetAxis();

	if (Axis)
	{
		AVehicle* Vehicle = Axis->GetVehicle();
		if (Vehicle)
		{
			//Apply Tire Force from wheel hit location to vehicle body
			Vehicle->VehicleBody->AddForceAtLocation((FxVec + FyVec) * 100, HitResult.Location);
		}
	}
}

void USCWheel::ApplyBrakeTorque()
{
	//Wheel Deceleration with brake
	if (WheelBrakeTorque > 0.0)
	{
		const double WheelInertia = GetWheelInertia();

		WheelAngularVelocity =
			//Compare WheelVelocity direction before and after braking
			FMath::Sign(WheelAngularVelocity - WheelBrakeTorque / WheelInertia * DeltaTimer) 
			== 
			FMath::Sign(WheelBrakeTorque * WheelAngularVelocity) 
			?
			WheelAngularVelocity - WheelBrakeTorque / WheelInertia * DeltaTimer :
			0.0;
	}
}

void USCWheel::WheelAcceleration()
{
}

void USCWheel::SetTractionTorque()
{
	USCAxis* Axis = GetAxis();

	if (Axis)
	{
		WheelTractionTorque = Axis->AxisTractionTorque * WheelTorqueRatio;

		SetWheelAngularVelocity();
	}
}

void USCWheel::SetFrictionTorque()
{
	const FSAxis AxisSetup = GetAxisSetup();
	double LongSlipVelocityL = GetLongSlipVelocity();

	//FrictionTorque = FrictionForce * FrictionCoefficient * Radius * Velocity
	//If no contact give a small friction torque to avoid infinite wheel spin.
	WheelFrictionTorque = bWheelContact ? 
		FMath::Max(Fz, 0)* AxisSetup.Wheel.Radius / 100.0 * FMath::Clamp(LongSlipVelocityL / 100, -1, 1) * (HitResult.PhysMaterial.IsValid() ? HitResult.PhysMaterial->Friction : 1) 
		: 
		50;

	const double WheelInertia = GetWheelInertia();

	UACVehiclePhysics* VehiclePhysics = nullptr;
	GetVehiclePhysics(VehiclePhysics);

	double CurrentTotalGearRatio = VehiclePhysics->GetTotalGearRatio();
	
	int CurrentGear = VehiclePhysics->GetCurrentGear();
	double WheelAngularDecelaration = WheelFrictionTorque / WheelInertia;
	//TODO: ClutchAngularVelocity might be needed here
	double MaxWheelSpeedOnCurrentGear = CurrentGear != 1 ? VehiclePhysics->EngineAngularVelocity / CurrentTotalGearRatio : 99999.0;

	WheelAngularVelocity = FMath::Min(FMath::Abs(WheelAngularVelocity - WheelFrictionTorque / WheelInertia * DeltaTimer), FMath::Abs(MaxWheelSpeedOnCurrentGear)) * FMath::Sign(MaxWheelSpeedOnCurrentGear);

}

void USCWheel::SetBrakeTorque(double bBrakeValue, bool bHandbrakeValue)
{
	const FSWheel WheelSetup = GetWheelSetup();

	UACVehiclePhysics* VehiclePhysics = nullptr;
	GetVehiclePhysics(VehiclePhysics);

	if (VehiclePhysics)
	{
		double BrakeTorque =
			bBrakeValue *
			BrakeBias *
			VehiclePhysics->MechanicalData.BrakeData.Radius / 100 *
			VehiclePhysics->MechanicalData.BrakeData.SinglePistonForce *
			VehiclePhysics->MechanicalData.BrakeData.PistonCount *
			VehiclePhysics->MechanicalData.BrakeData.FrictionCoefficient;
		
		double HandBrakeTorque = (WheelSetup.AffectedByHandbrake ? 1.0 : 0.0) * WheelSetup.MaxHandbrakeTorque* (bHandbrakeValue ? 1.0 : 0.0);

		WheelBrakeTorque = BrakeTorque + HandBrakeTorque;

	}
}

