#pragma once

#include "Vehicle.h"
#include "SCAxis.h"
#include "SCWheel.h"
#include "Net/UnrealNetwork.h"

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

void AVehicle::CreateSuspension_Implementation(bool bIsLeft, USCAxis* InAxis, double InWheelTorqueRatio, USCWheel*& OutSuspension)
{
}

USCAxis* AVehicle::CreateAxis_Implementation(FSAxis InAxisData)
{
	return nullptr;
}

void AVehicle::CreateWheelMesh_Implementation(USCWheel* InSuspension, USkeletalMeshComponent*& OutWheelMesh)
{
}

void AVehicle::HandleCameraLookUp(double DeltaRotationPitchP)
{
}

void AVehicle::HandleCameraLookRight(double DeltaRotationYawP)
{
}

void AVehicle::CreateLights()
{
}

void AVehicle::CreateExhaustSounds()
{
}

void AVehicle::TriggerBackfires()
{
}

void AVehicle::SetHeadAndTailLights()
{
}

void AVehicle::HandeHeadAndTailLights()
{
}

void AVehicle::GetWorldTemperature(double& TempP)
{
	TempP = 25.0;
}

void AVehicle::ConstructGearBox()
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

void AVehicle::OnRep_HeadLightsFarOn_Implementation()
{
}

void AVehicle::OnRep_HeadLightsCloseOn_Implementation()
{
}

void AVehicle::OnRep_SignalLOn_Implementation()
{
}

void AVehicle::OnRep_SignalROn_Implementation()
{
}

void AVehicle::OnRep_TailLightsOn_Implementation()
{
}

void AVehicle::OnRep_BrakeLightsOn_Implementation()
{
}

void AVehicle::OnRep_ReverseLightsOn_Implementation()
{
}

void AVehicle::CreateDynamicMaterials()
{
}

void AVehicle::SetDynamicMaterialsToMesh()
{
}

void AVehicle::CreateCameras()
{
}

void AVehicle::CreateExhaustParticles()
{
}

void AVehicle::CreateMirrors()
{
}

void AVehicle::CreateTireSounds()
{
}

void AVehicle::TriggerNitrous(bool bEnableP)
{
}




