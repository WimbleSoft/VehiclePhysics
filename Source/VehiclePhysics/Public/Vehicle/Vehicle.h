#pragma once

#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components/SpotLightComponent.h>
#include <Camera/CameraComponent.h>
#include "ACVehiclePhysics.h"
#include "SCWheel.h"
#include "SCAxis.h"

#include "Vehicle.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Scene Component Creation Functions */
	UFUNCTION(BlueprintCallable, Category = "SC Creation")
	void CreateSuspension(bool bIsLeft, USCAxis* InAxis, double InWheelTorqueRatio, USCWheel*& OutSuspension);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC Creation")
	USCAxis* CreateAxis(const FSAxis& InAxisData);

	/** Scene Component Creation Functions */
	UFUNCTION(BlueprintCallable, Category = "SC Creation")
	USkeletalMeshComponent* CreateWheelMesh(USCWheel* InSuspension);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleCameraLookUp(double DeltaRotationPitch);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleCameraLookRight(double DeltaRotationYaw);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleLookBackCameras(bool bIsLookingBack);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleCameraSwitching();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void ResetCameraHandling();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void AddFrontFarAndCloseLights(FName Socket);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void AddBrakeLights(FName Socket);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void AddTailLights(FName Socket);

	/** Not needed in BMW vehicles. */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void AddSignalLights(FName Socket);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void AddReverseLights(FName Socket);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateLights();

	/** Please add a function description */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Component Construction")
	void CreateExhaustSounds();

	/** Please add a function description */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Particle Handling")
	void TriggerBackfires();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Lights Handling")
	void SetHeadAndTailLights();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Lights Handling")
	void HandleHeadAndTailLights();

	/** Get world temperature in Celsius unit. */
	UFUNCTION(BlueprintPure, Category = "Default", meta = (ReturnParameterName = "Temp"))
	double GetWorldTemperature();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ConstructGearBox() const;

	/** Please add a function description */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Materials")
	void CreateDynamicMaterials();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Materials")
	void SetDynamicMaterialsToMesh() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateCameras();

	/** Please add a function description */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Component Construction")
	void CreateExhaustParticles();

	/** Please add a function description */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Component Construction")
	void CreateMirrors();

	/** Scene Component Creation Functions */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Component Construction")
	void CreateTireSounds();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Particle Handling")
	void TriggerNitrousParticles(bool bEnableP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_HeadLightsFarOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_HeadLightsCloseOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_SignalLOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_SignalROn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_TailLightsOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_BrakeLightsOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Replication")
	void OnRep_ReverseLightsOn();
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UFMODAudioComponent> A_Transmission;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UACVehiclePhysics> ACVehiclePhysics;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAudioComponent> A_Horn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAudioComponent> A_GearChange;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAudioComponent> A_EngineStall;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAudioComponent> A_EngineIgnite;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UFMODAudioComponent> A_TurboLoad;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<UFMODAudioComponent> A_Engine;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<USkeletalMeshComponent> VehicleBody;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArmFarOpposite;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArmFar;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArmClose;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArmCloseOpposite;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	TArray<UCameraComponent*> Cameras = TArray<UCameraComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	int32 LastActiveCamIndex = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> ReverseLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> CloseHeadLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> FarHeadLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", Replicated, meta = (MultiLine = "true"))
	TArray<UFMODAudioComponent*> ExhaustSounds = TArray<UFMODAudioComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", Replicated, meta = (MultiLine = "true"))
	TArray<UFMODAudioComponent*> BackfireSounds = TArray<UFMODAudioComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> BrakeLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> TailLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> LeftSignalLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", Replicated, meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> RightSignalLights = TArray<USpotLightComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	bool CameraReleased = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FName RPMTextForEngineAudio = FName("RPM");

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light Data", Replicated, meta = (MultiLine = "true", UIMax = "3", ClampMax = "3", ClampMin = "0", UIMin = "0"))
	int32 LightMode = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_HeadLightsFarOn", meta = (MultiLine = "true"))
	bool HeadLightsFarOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_HeadLightsCloseOn", meta = (MultiLine = "true"))
	bool HeadLightsCloseOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_SignalLOn", meta = (MultiLine = "true"))
	bool SignalLOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_SignalROn", meta = (MultiLine = "true"))
	bool SignalROn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TArray<UMaterialInstance*> MI_Array = TArray<UMaterialInstance*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	bool InvertMouseLook = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> FarHeadLightsDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> CloseHeadLightsDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> TailLightsDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> SignalLeftDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> SignalRightDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> BrakeLightsDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> ReverseLightsDMI = TObjectPtr<UMaterialInstanceDynamic>();

	/** Camera Socket Names to fit in SkeletalMesh Socket positions. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	TArray<FName> CameraSocketNames;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle Components", meta = (MultiLine = "true"))
	TArray<UParticleSystemComponent*> BackfireParticles = TArray<UParticleSystemComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle Components", meta = (MultiLine = "true"))
	TArray<UParticleSystemComponent*> NitrousParticles = TArray<UParticleSystemComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TArray<UCameraComponent*> MirrorCameras = TArray<UCameraComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorFL = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorFR = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorRL = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorRR = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool Trunk = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool Hood = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_TailLightsOn", meta = (MultiLine = "true"))
	bool TailLightsOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_BrakeLightsOn", meta = (MultiLine = "true"))
	bool BrakeLightsOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_ReverseLightsOn", meta = (MultiLine = "true"))
	bool ReverseLightsOn = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", Replicated, meta = (MultiLine = "true", UIMax = "3", ClampMax = "3", ClampMin = "0", UIMin = "0"))
	int32 SignalMode = 0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UAudioComponent*> TireRollingSounds = TArray<UAudioComponent*>();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UAudioComponent*> TireSlideSounds = TArray<UAudioComponent*>();
};
