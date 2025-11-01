#pragma once

#include "Vehicle.h"
#include "SCAxis.h"
#include "SCWheel.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VehicleBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleBody"));
	VehicleBody->SetupAttachment(RootComponent);

	ACVehiclePhysics = CreateDefaultSubobject<UACVehiclePhysics>(TEXT("ACVehiclePhysics"));
	//ACVehiclePhysics->SetupAttachment(RootComponent);

	A_Engine = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("A_Engine"));
	A_Engine->SetupAttachment(VehicleBody, FName("Engine"));
	A_Engine->bAutoActivate = false;

	A_TurboLoad = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("A_TurboLoad"));
	A_TurboLoad->SetupAttachment(VehicleBody, FName("Engine"));
	A_TurboLoad->bAutoActivate = false;

	A_Transmission = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("A_Transmission"));
	A_Transmission->SetupAttachment(VehicleBody, FName("Chassis"));
	A_Transmission->bAutoActivate = false;

	A_Horn = CreateDefaultSubobject<UAudioComponent>(TEXT("A_Horn"));
	A_Horn->SetupAttachment(VehicleBody, FName("Engine"));
	A_Horn->bAutoActivate = false;

	A_GearChange = CreateDefaultSubobject<UAudioComponent>(TEXT("A_GearChange"));
	A_GearChange->SetupAttachment(VehicleBody, FName("Engine"));
	A_GearChange->bAutoActivate = false;

	A_EngineStall = CreateDefaultSubobject<UAudioComponent>(TEXT("A_EngineStall"));
	A_EngineStall->SetupAttachment(VehicleBody, FName("Engine"));
	A_EngineStall->bAutoActivate = false;

	A_EngineIgnite = CreateDefaultSubobject<UAudioComponent>(TEXT("A_EngineIgnite"));
	A_EngineIgnite->SetupAttachment(VehicleBody, FName("Engine"));
	A_EngineIgnite->bAutoActivate = false;

	SpringArmFarOpposite = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmFarOpposite"));
	SpringArmFarOpposite->SetupAttachment(VehicleBody);
	SpringArmFarOpposite->SetRelativeRotation(FRotator(-15.0f, -180.0f, 0.0f));
	SpringArmFarOpposite->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmFarOpposite->TargetArmLength = 550.0f;
	SpringArmFarOpposite->bEnableCameraLag = true;
	SpringArmFarOpposite->bEnableCameraRotationLag = true;
	SpringArmFarOpposite->CameraLagSpeed = 20.0f;
	SpringArmFarOpposite->CameraRotationLagSpeed = 5.0f;
	SpringArmFarOpposite->CameraLagMaxDistance = 10.0f;

	SpringArmFar = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmFar"));
	SpringArmFar->SetupAttachment(VehicleBody);
	SpringArmFar->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArmFar->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmFar->TargetArmLength = 550.0f;
	SpringArmFar->bEnableCameraLag = true;
	SpringArmFar->bEnableCameraRotationLag = true;
	SpringArmFar->CameraLagSpeed = 50.0f;
	SpringArmFar->CameraRotationLagSpeed = 10.0f;
	SpringArmFar->CameraLagMaxDistance = 20.0f;

	SpringArmClose = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmClose"));
	SpringArmClose->SetupAttachment(VehicleBody);
	SpringArmClose->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArmClose->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmClose->TargetArmLength = 400.0f;
	SpringArmClose->bEnableCameraLag = true;
	SpringArmClose->bEnableCameraRotationLag = true;
	SpringArmClose->CameraLagSpeed = 50.0f;
	SpringArmClose->CameraRotationLagSpeed = 5.0f;
	SpringArmClose->CameraLagMaxDistance = 20.0f;

	SpringArmCloseOpposite = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmCloseOpposite"));
	SpringArmCloseOpposite->SetupAttachment(VehicleBody);
	SpringArmCloseOpposite->SetRelativeRotation(FRotator(-15.0f, -180.0f, 0.0f));
	SpringArmCloseOpposite->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmCloseOpposite->TargetArmLength = 400.0f;
	SpringArmCloseOpposite->bEnableCameraLag = true;
	SpringArmCloseOpposite->bEnableCameraRotationLag = true;
	SpringArmCloseOpposite->CameraLagSpeed = 50.0f;
	SpringArmCloseOpposite->CameraRotationLagSpeed = 10.0f;
	SpringArmCloseOpposite->CameraLagMaxDistance = 20.0f;

	//ACVehiclePhysics->ConstructVehiclePhysics();
	//CreateDynamicMaterials();
	//CreateLights();
	//CreateCameras();
	//SetHeadAndTailLights();
	//CreateExhaustSounds();
	//CreateExhaustParticles();
	//CreateMirrors();
	//SetDynamicMaterialsToMesh();
	//ConstructGearBox();
	//CreateTireSounds();

}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVehicle, LightMode);
	DOREPLIFETIME(AVehicle, HeadLightsCloseOn);
	DOREPLIFETIME(AVehicle, HeadLightsFarOn);
	DOREPLIFETIME(AVehicle, SignalLOn);
	DOREPLIFETIME(AVehicle, SignalROn);
	DOREPLIFETIME(AVehicle, SignalMode);
	DOREPLIFETIME(AVehicle, TailLightsOn);
	DOREPLIFETIME(AVehicle, BrakeLightsOn);
	DOREPLIFETIME(AVehicle, ReverseLightsOn);
}

void AVehicle::CreateSuspension(bool bIsLeft, USCAxis* InAxis, double InWheelTorqueRatio, USCWheel*& OutSuspension)
{
	OutSuspension = nullptr;

	if (!InAxis)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSuspension: InAxis is nullptr"));
		return;
	}

	// --- Break SAxis (K2Node_VariableGet_10 + BreakStruct_3) ---
	const FSAxis Axis = InAxis->AxisSetup;              // AxisSetup
	double AxisWidth = static_cast<double>(Axis.AxisWidth);

	// Select (K2Node_Select_1): Y = bIsLeft ? (AxisWidth * -1) : AxisWidth
	const double OffsetY = bIsLeft ? (AxisWidth * -1.0) : AxisWidth;

	// --- AddComponent SCWheel (K2Node_AddComponent_0, manual attach) ---
	// Name is optional but helpful in PIE with multiple calls
	const FName CompName = MakeUniqueObjectName(this, USCWheel::StaticClass(), TEXT("SCWheel"));
	USCWheel* Wheel = NewObject<USCWheel>(this, USCWheel::StaticClass(), CompName);
	if (!Wheel)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSuspension: Failed to allocate USCWheel"));
		return;
	}

	// Expose-on-spawn field in BP: bIsLeft (K2Node_AddComponent_0 input)
	// If your USCWheel doesn't have this property, remove the line.
	Wheel->bIsLeft = bIsLeft;

	// Manual attachment = true in BP; we attach explicitly (K2Node_CallFunction_3 K2_AttachToComponent)
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies*/ true);
	Wheel->AttachToComponent(InAxis, AttachRules);

	// MakeTransform(Location=(0, OffsetY, 0)) (K2Node_CallFunction_4) -> applied as relative transform
	Wheel->SetRelativeLocation(FVector(0.0, OffsetY, 0.0));
	// Rotation defaults to zero; Scale defaults to (1,1,1) as in the BP

	// Register so it becomes active at runtime
	Wheel->RegisterComponent();

	// Set WheelTorqueRatio (K2Node_VariableSet_0)
	Wheel->WheelTorqueRatio = InWheelTorqueRatio;

	// Return new suspension component (K2Node_FunctionResult_0 via knots)
	OutSuspension = Wheel;
}

USkeletalMeshComponent* AVehicle::CreateWheelMesh(USCWheel* InSuspension)
{
	if (!InSuspension)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateWheelMesh: InSuspension is null"));
		return nullptr;
	}

	// --- Add SkeletalMeshComponent (BP: K2Node_AddComponent_1) ---
	USkeletalMeshComponent* WheelMesh = NewObject<USkeletalMeshComponent>(
		this, USkeletalMeshComponent::StaticClass(), TEXT("WheelMesh"));
	WheelMesh->SetMobility(EComponentMobility::Movable);
	WheelMesh->RegisterComponent();

	// BP math:
	//   Yaw = (bIsLeft ? 0 : 180)
	//   Location.Z = -CurrentSpringLength
	//   Scale = (0.6, 0.6, 0.6)
	const float YawDeg = /* InSuspension->bIsLeft ? */
		(InSuspension && InSuspension->bIsLeft ? 0.f : 180.f);
	const FVector RelLoc(0.f, 0.f, -static_cast<float>(InSuspension->CurrentSpringLength));
	const FRotator RelRot(/*Pitch*/0.f, /*Yaw*/YawDeg, /*Roll*/0.f);
	const FVector RelScale(0.6f);

	// Attach to Suspension (BP: K2_AttachToComponent, KeepRelative, Weld=true)
	WheelMesh->AttachToComponent(
		InSuspension,
		FAttachmentTransformRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			/*bWeldSimulatedBodies*/ true));

	// Apply relative transform (equivalent to MakeTransform fed into AddComponent)
	WheelMesh->SetRelativeLocation(RelLoc);
	WheelMesh->SetRelativeRotation(RelRot);
	WheelMesh->SetRelativeScale3D(RelScale);

	// --- Add StaticMeshComponent "Rim" (BP: K2Node_AddComponent_3 -> K2_AttachToComponent socket "Rim") ---
	UStaticMeshComponent* Rim = NewObject<UStaticMeshComponent>(
		this, UStaticMeshComponent::StaticClass(), TEXT("WheelRim"));
	Rim->SetMobility(EComponentMobility::Movable);
	Rim->RegisterComponent();
	Rim->AttachToComponent(
		WheelMesh,
		FAttachmentTransformRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			/*bWeldSimulatedBodies*/ true),
		TEXT("Rim"));
	// BP: Conv_DoubleToVector(11.0) -> Scale
	Rim->SetRelativeScale3D(FVector(11.f));

	// --- Add StaticMeshComponent "Tire" (BP: K2Node_AddComponent_2 -> K2_AttachToComponent socket "Tire") ---
	UStaticMeshComponent* Tire = NewObject<UStaticMeshComponent>(
		this, UStaticMeshComponent::StaticClass(), TEXT("WheelTire"));
	Tire->SetMobility(EComponentMobility::Movable);
	Tire->RegisterComponent();
	Tire->AttachToComponent(
		WheelMesh,
		FAttachmentTransformRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			/*bWeldSimulatedBodies*/ true),
		TEXT("Tire"));
	// Same scale forwarded via knot in BP
	Tire->SetRelativeScale3D(FVector(11.f));

	// BP: Set InSuspension.WheelMesh = WheelMesh, then return OutWheelMesh
	InSuspension->WheelMesh = WheelMesh;
	return WheelMesh;
}

USCAxis* AVehicle::CreateAxis(const FSAxis& InAxisData)
{
	if (!VehicleBody)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateAxis: VehicleBody is null"));
		return nullptr;
	}

	// BP: AddComponent (TemplateType=SCAxis, bManualAttachment=true)
	const FName AxisObjName = InAxisData.AxisName.IsNone()
		? FName(TEXT("Axis"))
		: FName(*FString::Printf(TEXT("Axis_%s"), *InAxisData.AxisName.ToString()));

	USCAxis* Axis = NewObject<USCAxis>(this, USCAxis::StaticClass(), AxisObjName);
	if (!Axis) { return nullptr; }

	// BP: Expose-on-spawn pin "AxisSetup" <- InAxisData
	// (Assumes USCAxis has a UPROPERTY named AxisSetup)
	Axis->AxisSetup = InAxisData;

	Axis->SetMobility(EComponentMobility::Movable);
	Axis->RegisterComponent();

	// BP: K2_AttachToComponent(Self=Axis, Parent=VehicleBody, Socket=InAxisData.AxisName,
	//     LocationRule/RotationRule/ScaleRule = SnapToTarget, bWeldSimulatedBodies = true)
	Axis->AttachToComponent(
		VehicleBody,
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			/*bWeldSimulatedBodies*/ true),
		InAxisData.AxisName);

	// BP: RelativeTransform fed to AddComponent was identity; with manual attach + SnapToTarget
	// it's effectively ignored. Nothing else to set.

	return Axis; // BP FunctionResult.ReturnValue
}

void AVehicle::HandleCameraLookUp(double DeltaRotationPitch)
{
	// 1) Add relative pitch to every spring arm (as in the BP "AddRelativeRotation")
	auto AddPitch = [&](USpringArmComponent* Arm)
		{
			if (Arm)
			{
				Arm->AddRelativeRotation(
					FRotator(DeltaRotationPitch, 0.f, 0.f),
					/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);
			}
		};

	AddPitch(SpringArmClose);
	AddPitch(SpringArmFar);
	AddPitch(SpringArmCloseOpposite);
	AddPitch(SpringArmFarOpposite);

	// 2) Use ONLY SpringArmClose->GetTargetRotation() to build the world rotation
	if (SpringArmClose)
	{
		const FRotator BaseTarget = SpringArmClose->GetTargetRotation(); // BP: self = SpringArmClose
		const double ClampedPitch = FMath::ClampAngle(BaseTarget.Pitch, -45.0, 15.0);
		const float BaseYaw = BaseTarget.Yaw;

		const FRotator Desired(static_cast<float>(ClampedPitch), BaseYaw, 0.f);

		auto SetWorldRot = [&](USpringArmComponent* Arm)
			{
				if (Arm)
				{
					Arm->SetWorldRotation(
						Desired,
						/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);
				}
			};

		SetWorldRot(SpringArmClose);
		SetWorldRot(SpringArmFar);
		SetWorldRot(SpringArmCloseOpposite);
		SetWorldRot(SpringArmFarOpposite);
	}

	// 3) In-car camera at index 2: add pitch, then clamp its own RELATIVE Pitch/Yaw to [-45, +45]
	if (Cameras.IsValidIndex(2) && Cameras[2])
	{
		UCameraComponent* Cam = Cameras[2];

		Cam->AddRelativeRotation(
			FRotator(DeltaRotationPitch, 0.f, 0.f),
			/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);

		const FRotator Rel = Cam->GetRelativeRotation();
		const double ClampedPitchCam = FMath::ClampAngle(Rel.Pitch, -45.0, 45.0);
		const double ClampedYawCam = FMath::ClampAngle(Rel.Yaw, -45.0, 45.0);

		Cam->SetRelativeRotation(
			FRotator(static_cast<float>(ClampedPitchCam), static_cast<float>(ClampedYawCam), 0.f),
			/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);
	}
}

void AVehicle::HandleCameraLookRight(double DeltaRotationYaw)
{
	// 1) Add relative Yaw to ALL four spring arms (no sweep, no teleport)
	auto AddYaw = [&](USpringArmComponent* Arm)
		{
			if (Arm)
			{
				Arm->AddRelativeRotation(
					FRotator(0.f, DeltaRotationYaw, 0.f),
					/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);
			}
		};

	AddYaw(SpringArmClose);
	AddYaw(SpringArmFar);
	AddYaw(SpringArmCloseOpposite);
	AddYaw(SpringArmFarOpposite);

	// 2) If Cameras[2] is valid: add relative Yaw, then clamp its RELATIVE Pitch & Yaw to [-45, +45]
	if (Cameras.IsValidIndex(2) && Cameras[2])
	{
		UCameraComponent* Cam = Cameras[2];

		Cam->AddRelativeRotation(
			FRotator(0.f, DeltaRotationYaw, 0.f),
			/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);

		const FRotator Rel = Cam->GetRelativeRotation();
		const double ClampedPitch = FMath::ClampAngle(Rel.Pitch, -45.0, 45.0);
		const double ClampedYaw = FMath::ClampAngle(Rel.Yaw, -45.0, 45.0);

		// BP sets Roll to its default 0.0 on SetRelativeRotation
		Cam->SetRelativeRotation(
			FRotator(static_cast<float>(ClampedPitch), static_cast<float>(ClampedYaw), 0.f),
			/*bSweep=*/false, /*OutHit=*/nullptr, /*bTeleport=*/ETeleportType::None);
	}
}

void AVehicle::HandleLookBackCameras(bool bIsLookingBack)
{
	// Even/odd pair for the current driving camera
	const int32 Base = LastActiveCamIndex * 2;
	const int32 Back = Base + 1;

	if (!Cameras.IsValidIndex(Base) || !Cameras.IsValidIndex(Back))
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleLookBackCameras: invalid indices (Base=%d, Back=%d, Num=%d)"), Base, Back, Cameras.Num());
		return;
	}

	Cameras[Base]->SetActive(!bIsLookingBack, /*bReset=*/false);
	Cameras[Back]->SetActive(bIsLookingBack,  /*bReset=*/false);
}

void AVehicle::HandleCameraSwitching()
{
	const int32 Len = Cameras.Num();
	if (Len <= 0)
	{
		return;
	}

	// Deactivate currently-active camera at index (LastActiveCamIndex * 2)
	{
		const int32 DeactivateIdx = LastActiveCamIndex * 2;
		if (Cameras.IsValidIndex(DeactivateIdx) && Cameras[DeactivateIdx])
		{
			// ActorComponent::SetActive(bNewActive, bReset)
			Cameras[DeactivateIdx]->SetActive(false, /*bReset*/ true);
		}
	}

	// If Len == (LastActiveCamIndex + 1) * 2  => wrap to 0, else increment
	if (Len == (LastActiveCamIndex + 1) * 2)
	{
		LastActiveCamIndex = 0;
	}
	else
	{
		LastActiveCamIndex = LastActiveCamIndex + 1;
	}

	// Activate new camera at index (LastActiveCamIndex * 2)
	{
		const int32 ActivateIdx = LastActiveCamIndex * 2;
		if (Cameras.IsValidIndex(ActivateIdx) && Cameras[ActivateIdx])
		{
			Cameras[ActivateIdx]->SetActive(true, /*bReset*/ true);
		}
	}

	if (SpringArmClose)
	{
		SpringArmClose->SetRelativeRotation(FRotator(-5.f, 0.f, 0.f), false, nullptr, ETeleportType::None);
	}
	if (SpringArmFar)
	{
		SpringArmFar->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f), false, nullptr, ETeleportType::None);
	}
	if (SpringArmCloseOpposite)
	{
		SpringArmCloseOpposite->SetRelativeRotation(FRotator(-5.f, 180.f, 0.f), false, nullptr, ETeleportType::None);
	}
	if (SpringArmFarOpposite)
	{
		SpringArmFarOpposite->SetRelativeRotation(FRotator(-15.f, 180.f, 0.f), false, nullptr, ETeleportType::None);
	}
}

void AVehicle::ResetCameraHandling()
{
	if (!CameraReleased)
	{
		return;
	}

	// 1) Snap spring arms
	if (SpringArmClose)
	{
		// Pitch -15, Yaw 0, Roll 0
		SpringArmClose->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	}
	if (SpringArmCloseOpposite)
	{
		// Pitch -15, Yaw 180, Roll 0
		SpringArmCloseOpposite->SetRelativeRotation(FRotator(-15.f, 180.f, 0.f));
	}
	if (SpringArmFar)
	{
		// Pitch -15, Yaw 0, Roll 0
		SpringArmFar->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	}
	if (SpringArmFarOpposite)
	{
		// Pitch -15, Yaw 180, Roll 0
		SpringArmFarOpposite->SetRelativeRotation(FRotator(-15.f, 180.f, 0.f));
	}

	// 2) For each camera, interpolate Pitch & Yaw back to 0 (Roll forced to 0)
	const float DeltaTime = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
	const float InterpSpeed = 5.f;

	for (UCameraComponent* Cam : Cameras)
	{
		if (!Cam) continue;

		const FRotator Cur = Cam->GetRelativeRotation();

		const float NewPitch = FMath::FInterpTo(Cur.Pitch, 0.f, DeltaTime, InterpSpeed);
		const float NewYaw = FMath::FInterpTo(Cur.Yaw, 0.f, DeltaTime, InterpSpeed);
		const float NewRoll = 0.f; // BP pin left at default 0

		Cam->SetRelativeRotation(FRotator(NewPitch, NewYaw, NewRoll));
	}
}

void AVehicle::CreateLights()
{
	if (!VehicleBody) return;

	const TArray<FName> SocketNames = VehicleBody->GetAllSocketNames();

	for (const FName& Socket : SocketNames)
	{
		const FString NameStr = Socket.ToString();
		const FString Prefix = NameStr.Left(5); // same as GetSubstring(Start=0, Length=5) in BP

		if (Prefix.Equals(TEXT("RvrsL"), ESearchCase::CaseSensitive))
		{
			AddReverseLights(Socket);
		}
		else if (Prefix.Equals(TEXT("HeadL"), ESearchCase::CaseSensitive))
		{
			AddFrontFarAndCloseLights(Socket);
		}
		else if (Prefix.Equals(TEXT("BrakL"), ESearchCase::CaseSensitive))
		{
			AddBrakeLights(Socket);
		}
		else if (Prefix.Equals(TEXT("TailL"), ESearchCase::CaseSensitive))
		{
			AddTailLights(Socket);
		}
		else if (Prefix.Equals(TEXT("SgnlL"), ESearchCase::CaseSensitive))
		{
			AddSignalLights(Socket);
		}
		else
		{
			// Default branch in BP: do nothing. (Optional) UE_LOG for unknown sockets.
			// UE_LOG(LogTemp, Verbose, TEXT("CreateLightsX: Ignored socket %s"), *NameStr);
		}
	}
}

void AVehicle::AddFrontFarAndCloseLights(FName Socket)
{
	if (!VehicleBody) return;

	// Extract the 7th character to decide Far vs Close (StartIndex=6, Length=1)
	const FString SockStr = Socket.ToString();
	const TCHAR Tag = (SockStr.Len() > 6) ? SockStr[6] : TEXT('\0'); // 'F' or 'C'

	// Spawn component (equivalent to BP AddComponent with Manual Attachment)
	USpotLightComponent* NewLight = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
	if (!NewLight) return;

	// Defaults per branch
	if (Tag == TEXT('F'))
	{
		NewLight->SetMobility(EComponentMobility::Movable);
		NewLight->Intensity = 100000.f;
		NewLight->IntensityUnits = ELightUnits::Unitless;
		NewLight->AttenuationRadius = 65536.f;
		NewLight->InnerConeAngle = 30.f;
		NewLight->OuterConeAngle = 45.f;
		NewLight->SourceRadius = 30.f;
		NewLight->SoftSourceRadius = 0.f;
		NewLight->SourceLength = 0.f;
		NewLight->bUseTemperature = true;
		NewLight->Temperature = 18000.f;
		NewLight->bAffectsWorld = true;
		NewLight->SetCastShadows(true);
		NewLight->IndirectLightingIntensity = 1.f;
		NewLight->VolumetricScatteringIntensity = 1.f;
		NewLight->SetVisibility(false, true);
	}
	else
	{
		NewLight->SetMobility(EComponentMobility::Movable);
		NewLight->Intensity = 20000.f;
		NewLight->IntensityUnits = ELightUnits::Unitless;
		NewLight->AttenuationRadius = 8192.f;
		NewLight->InnerConeAngle = 45.f;
		NewLight->OuterConeAngle = 60.f;
		NewLight->SourceRadius = 0.f;
		NewLight->SoftSourceRadius = 0.f;
		NewLight->SourceLength = 0.f;
		NewLight->bUseTemperature = true;
		NewLight->Temperature = 8000.f;
		NewLight->bAffectsWorld = true;
		NewLight->SetCastShadows(true);
		NewLight->IndirectLightingIntensity = 1.f;
		NewLight->VolumetricScatteringIntensity = 1.f;
		NewLight->SetVisibility(false, true);
	}

	// Register and attach (KeepRelative + weld, RelativeTransform = identity)
	NewLight->RegisterComponent();

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true);
	NewLight->AttachToComponent(VehicleBody, AttachRules, Socket);
	NewLight->SetRelativeTransform(FTransform::Identity);

	// Store in the right array
	if (Tag == TEXT('F'))  
		FarHeadLights.Add(NewLight);
	else
		CloseHeadLights.Add(NewLight);
}

void AVehicle::AddBrakeLights(FName Socket)
{
	if (!VehicleBody) return;

	USpotLightComponent* NewBrake = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
	if (!NewBrake) return;

	NewBrake->SetMobility(EComponentMobility::Movable);

	// Light section (from your screenshot)
	NewBrake->Intensity = 8000.f;
	NewBrake->IntensityUnits = ELightUnits::Unitless;
	NewBrake->SetLightColor(FLinearColor::Red, /*bSRGB=*/true);

	NewBrake->AttenuationRadius = 500.f;
	NewBrake->InnerConeAngle = 40.f;
	NewBrake->OuterConeAngle = 80.f;
	NewBrake->SourceRadius = 0.f;
	NewBrake->SoftSourceRadius = 0.f;
	NewBrake->SourceLength = 0.f;

	NewBrake->bUseTemperature = false;
	NewBrake->bAffectsWorld = true;
	NewBrake->SetCastShadows(true);
	NewBrake->IndirectLightingIntensity = 1.f;
	NewBrake->VolumetricScatteringIntensity = 1.f;

	// BP had "Visible" unchecked
	NewBrake->SetVisibility(false, /*bPropagateToChildren=*/true);

	// Manual attach + KeepRelative + weld bodies; RelativeTransform = identity
	NewBrake->RegisterComponent();

	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true);
	NewBrake->AttachToComponent(VehicleBody, AttachRules, Socket);
	NewBrake->SetRelativeTransform(FTransform::Identity);

	BrakeLights.Add(NewBrake);
}

void AVehicle::AddTailLights(FName Socket)
{
	if (!VehicleBody) return;

	USpotLightComponent* NewTail =
		NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
	if (!NewTail) return;

	NewTail->SetMobility(EComponentMobility::Movable);

	// Light section (from your screenshot)
	NewTail->Intensity = 1000.f;
	NewTail->IntensityUnits = ELightUnits::Unitless;
	NewTail->SetLightColor(FLinearColor::Red, /*bSRGB=*/true);

	NewTail->AttenuationRadius = 3000.f;
	NewTail->InnerConeAngle = 40.f;
	NewTail->OuterConeAngle = 70.f;
	NewTail->SourceRadius = 0.f;
	NewTail->SoftSourceRadius = 0.f;
	NewTail->SourceLength = 0.f;

	NewTail->bUseTemperature = false;
	NewTail->bAffectsWorld = true;
	NewTail->SetCastShadows(true);

	NewTail->IndirectLightingIntensity = 1.f;
	NewTail->VolumetricScatteringIntensity = 1.f;

	// Rendering → Visible unchecked
	NewTail->SetVisibility(false, /*bPropagateToChildren=*/true);
	NewTail->RegisterComponent();

	// ManualAttachment=true in BP → attach explicitly and keep relative
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true);
	NewTail->AttachToComponent(VehicleBody, AttachRules, Socket);
	NewTail->SetRelativeTransform(FTransform::Identity);

	TailLights.Add(NewTail);
}

void AVehicle::AddSignalLights(FName Socket)
{
	// Safety checks (BP graph assumes these exist)
	if (!VehicleBody)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSignalLights: VehicleBody is null."));
		return;
	}

	// --- Extract side from socket, equivalent to:
	// Name->String, GetSubstring(StartIndex=6, Length=1), Switch "L"/"R"
	const FString SocketStr = Socket.ToString();
	const TCHAR SideChar = (SocketStr.Len() >= 7) ? FChar::ToUpper(SocketStr[6]) : TEXT('\0'); // 0-based index 6 == 7th char

	// --- Add new SpotLightComponent (matches AddComponent + manual attach)
	USpotLightComponent* NewLight = NewObject<USpotLightComponent>(this);
	if (!NewLight)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSignalLights: Failed to create SpotLightComponent."));
		return;
	}

	NewLight->SetMobility(EComponentMobility::Movable);
	NewLight->Intensity = 2000.f;
	NewLight->IntensityUnits = ELightUnits::Unitless;
	NewLight->SetLightColor(FLinearColor(1.f, 156.f / 255.f, 0.f), /*bSRGB=*/true); // amber
	NewLight->AttenuationRadius = 300.f;
	NewLight->InnerConeAngle = 40.f;
	NewLight->OuterConeAngle = 70.f;
	NewLight->SourceRadius = 0.f;
	NewLight->SoftSourceRadius = 0.f;
	NewLight->SourceLength = 0.f;
	NewLight->bUseTemperature = false;
	NewLight->bAffectsWorld = true;
	NewLight->SetCastShadows(true);
	NewLight->IndirectLightingIntensity = 1.f;
	NewLight->VolumetricScatteringIntensity = 1.f;
	NewLight->SetVisibility(false, /*bPropagateToChildren=*/true);

	NewLight->SetRelativeTransform(FTransform::Identity);

	// KeepRelative rules and weld simulated bodies = true (matches K2_AttachToComponent with bWeldSimulatedBodies = TRUE)
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true);

	NewLight->AttachToComponent(VehicleBody, AttachRules, Socket);

	// Make the component live
	NewLight->RegisterComponent();
	NewLight->SetMobility(EComponentMobility::Movable);

	// --- Push into the correct array (Array_Add in BP)
	if (SideChar == TEXT('L'))
	{
		LeftSignalLights.Add(NewLight);
	}
	else if (SideChar == TEXT('R'))
	{
		RightSignalLights.Add(NewLight);
	}
	else
	{
		// If the naming doesn't contain 'L' or 'R' at the 7th character, warn (the BP had only L/R pins)
		UE_LOG(LogTemp, Warning, TEXT("AddSignalLights: Socket '%s' doesn't map to L/R (expected 7th char)."), *SocketStr);
	}
}

void AVehicle::AddReverseLights(FName Socket)
{
	if (!VehicleBody) return;

	USpotLightComponent* NewReverse = NewObject<USpotLightComponent>(this, USpotLightComponent::StaticClass());
	if (!NewReverse) return;

	NewReverse->SetMobility(EComponentMobility::Movable);

	// Light (from your Reverse defaults screenshot)
	NewReverse->Intensity = 2000.f;
	NewReverse->IntensityUnits = ELightUnits::Unitless;
	NewReverse->SetLightColor(FLinearColor::White, /*bSRGB=*/true);

	NewReverse->AttenuationRadius = 200.f;
	NewReverse->InnerConeAngle = 40.f;
	NewReverse->OuterConeAngle = 70.f;
	NewReverse->SourceRadius = 0.f;
	NewReverse->SoftSourceRadius = 0.f;
	NewReverse->SourceLength = 0.f;

	NewReverse->bUseTemperature = false;
	NewReverse->bAffectsWorld = true;
	NewReverse->SetCastShadows(true);

	NewReverse->IndirectLightingIntensity = 1.f;
	NewReverse->VolumetricScatteringIntensity = 1.f;

	// Rendering → Visible unchecked
	NewReverse->SetVisibility(false, /*bPropagateToChildren=*/true);

	NewReverse->RegisterComponent();

	// Attach like the BP: KeepRelative rules, weld on
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true);
	NewReverse->AttachToComponent(VehicleBody, AttachRules, Socket);

	NewReverse->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(0.1f)));

	ReverseLights.Add(NewReverse);
}

//void AVehicle::CreateExhaustSounds_Implementation()
//{
//}
//
//void AVehicle::TriggerBackfires_Implementation()
//{
//}
//

void AVehicle::SetHeadAndTailLights()
{
	// Matches the BP Switch on Int (no Default pin there):
	// 0: All off
	// 1: Close on, Far off, Tail on
	// 2: Close on, Far on,  Tail on
	// 3: All off
	switch (LightMode)
	{
		case 0:
		case 3:
			HeadLightsFarOn = false;
			HeadLightsCloseOn = false;
			TailLightsOn = false;
			break;

		case 1:
			HeadLightsFarOn = false;
			HeadLightsCloseOn = true;
			TailLightsOn = true;
			break;

		case 2:
			HeadLightsFarOn = true;
			HeadLightsCloseOn = true;
			TailLightsOn = true;
			break;

		default:
			// BP had no Default pin; leave current state unchanged.
			break;
	}
}

void AVehicle::HandleHeadAndTailLights()
{
	LightMode += 1;
	if (LightMode == 3) {
		LightMode = 0;
	}

	SetHeadAndTailLights();
}

double AVehicle::GetWorldTemperature()
{
	return 25.0;
}

void AVehicle::ConstructGearBox() const
{
	switch (ACVehiclePhysics->MechanicalData.GearBoxData.GearBoxType)
	{
		case EGearBoxType::FullAuto:
			ACVehiclePhysics->SetUseAutoGearBox(true);
			break;
		case EGearBoxType::Sequential:
			ACVehiclePhysics->SetUseAutoGearBox(false);
			break;
		case EGearBoxType::HShifter:
			ACVehiclePhysics->SetUseAutoGearBox(false);
			break;
	default:
		break;
	}
}

//void AVehicle::CreateDynamicMaterials_Implementation()
//{
//}
//

void AVehicle::SetDynamicMaterialsToMesh() const
{
	// Matches the BP sequence exactly (SignalL → SignalR → BrakeLights → TailLights → HeadLightsClose → HeadLightsFar → ReverseLights)
	if (!VehicleBody) return;

	VehicleBody->SetMaterialByName(FName(TEXT("SignalL")), SignalLeftDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("SignalR")), SignalRightDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("BrakeLights")), BrakeLightsDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("TailLights")), TailLightsDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("HeadLightsClose")), CloseHeadLightsDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("HeadLightsFar")), FarHeadLightsDMI);
	VehicleBody->SetMaterialByName(FName(TEXT("ReverseLights")), ReverseLightsDMI);
}

void AVehicle::CreateCameras()
{
	// Safety: if this is called multiple times (e.g., Construction Script), clean up first.
	for (UCameraComponent* Cam : Cameras)
	{
		if (IsValid(Cam))
		{
			Cam->DestroyComponent();
		}
	}
	Cameras.Reset();

	// Equivalent to KismetMathLibrary::MakeTransform with Location=(0,0,0), Rotation=(0,0,0), Scale=(0.1,0.1,0.1)
	const FTransform BaseXform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(0.1f));

	auto SpawnAndAttachCam =
		[this, &BaseXform](const FString& Name, USceneComponent* Parent, const FName Socket, bool bAutoActivate = false)
		-> UCameraComponent*
		{
			if (!IsValid(this)) return nullptr;

			UCameraComponent* Cam = NewObject<UCameraComponent>(this, UCameraComponent::StaticClass(), *Name);
			if (!Cam) return nullptr;

			Cam->SetRelativeTransform(BaseXform);
			Cam->bAutoActivate = bAutoActivate;

			// Register so it exists in the world
			Cam->RegisterComponent();

			// Attach (KeepRelative, weld = true) — mirrors K2_AttachToComponent settings
			if (USceneComponent* EffectiveParent = Parent)
			{
				Cam->AttachToComponent(
					EffectiveParent,
					FAttachmentTransformRules(EAttachmentRule::KeepRelative, /*bWeldSimulatedBodies=*/true),
					Socket
				);
			}

			// In BP they only add on the "true" branch of K2_AttachToComponent; in C++ there’s no return value.
			// Pragmatically add if it has any parent (or parent was null but we still registered it).
			if (Cam->GetAttachParent() != nullptr || Parent == nullptr)
			{
				Cameras.Add(Cam);
			}

			return Cam;
		};

	// 0) CloseFollowCamera (Auto Activate)
	SpawnAndAttachCam(TEXT("CloseFollowCamera"), SpringArmClose, NAME_None, /*bAutoActivate=*/true);

	// 1) CloseFollowCameraOpposite
	SpawnAndAttachCam(TEXT("CloseFollowCameraOpposite"), SpringArmCloseOpposite, NAME_None);

	// 2–7) "Armless" cameras — attach directly by socket names (no spring arm)
	// NOTE: If your sockets live on a mesh (likely), replace RootComponent below with that mesh component.
	USceneComponent* SocketParent = RootComponent; // TODO: set to your vehicle mesh (e.g., VehicleMesh) if sockets are on it.
	for (int32 i = 0; i < CameraSocketNames.Num(); ++i)
	{
		const FName SocketName = CameraSocketNames[i];
		const FString CamName = FString::Printf(TEXT("ArmlessCamera_%02d"), i + 2);
		SpawnAndAttachCam(CamName, SocketParent, SocketName);
	}

	// 8) FarFollowCamera
	SpawnAndAttachCam(TEXT("FarFollowCamera"), SpringArmFar, NAME_None);

	// 9) FarFollowCameraOpposite
	SpawnAndAttachCam(TEXT("FarFollowCameraOpposite"), SpringArmFarOpposite, NAME_None);
}

//void AVehicle::CreateExhaustParticles_Implementation()
//{
//}
//
//void AVehicle::CreateMirrors_Implementation()
//{
//}
//
//void AVehicle::CreateTireSounds_Implementation()
//{
//}
//

void AVehicle::TriggerNitrousParticles(bool bEnableP)
{
	for(UParticleSystemComponent* Particle : NitrousParticles)
	{
		if (Particle)
		{
			if (bEnableP)
			{
				Particle->Activate(true);
			}
			else
			{
				Particle->Deactivate();
			}
		}
	}
}

void AVehicle::OnRep_HeadLightsFarOn()
{
	for(USpotLightComponent* HeadLight : FarHeadLights)
	{
		if (HeadLight)
		{
			HeadLight->SetVisibility(HeadLightsFarOn);
		}
	}
	FarHeadLightsDMI->SetScalarParameterValue(FName("LightOn"), HeadLightsFarOn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_HeadLightsCloseOn()
{
	for(USpotLightComponent* HeadLight : CloseHeadLights)
	{
		if (HeadLight)
		{
			HeadLight->SetVisibility(HeadLightsCloseOn);
		}
	}
	FarHeadLightsDMI->SetScalarParameterValue(FName("LightOn"), HeadLightsCloseOn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_SignalLOn()
{
	for(USpotLightComponent* SignalLight : LeftSignalLights)
	{
		if (SignalLight)
		{
			SignalLight->SetVisibility(SignalLOn);
		}
	}
	SignalLeftDMI->SetScalarParameterValue(FName("LightOn"), SignalLOn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_SignalROn()
{
	for(USpotLightComponent* SignalLight : RightSignalLights)
	{
		if (SignalLight)
		{
			SignalLight->SetVisibility(SignalROn);
		}
	}
	
	SignalRightDMI->SetScalarParameterValue(FName("LightOn"), SignalROn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_TailLightsOn()
{
	for(USpotLightComponent* TailLight : TailLights)
	{
		if (TailLight)
		{
			TailLight->SetVisibility(TailLightsOn);
		}
	}
	TailLightsDMI->SetScalarParameterValue(FName("LightOn"), TailLightsOn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_BrakeLightsOn()
{
	for(USpotLightComponent* BrakeLight : BrakeLights)
	{
		if (BrakeLight)
		{
			BrakeLight->SetVisibility(BrakeLightsOn);
		}
	}

	BrakeLightsDMI->SetScalarParameterValue(FName("LightOn"), BrakeLightsOn ? 1.0f : 0.0f);
}

void AVehicle::OnRep_ReverseLightsOn()
{
	for(USpotLightComponent* ReverseLight : ReverseLights)
	{
		if (ReverseLight)
		{
			ReverseLight->SetVisibility(ReverseLightsOn);
		}
	}
	ReverseLightsDMI->SetScalarParameterValue(FName("LightOn"), ReverseLightsOn ? 1.0f : 0.0f);
}




