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
	void CreateSuspension(bool IsLeftP, USCAxis* AxisP, double WheelTorqueRatio, USCWheel*& SuspensionP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "SC Creation")
	USCAxis* CreateAxis(FSAxis AxisDataP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleCameraLookUp(double DeltaRotationPitchP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Camera Handling")
	void HandleCameraLookRight(double DeltaRotationYawP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateLights();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateExhaustSounds();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Particle Handling")
	void TriggerBackfires();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Lights Handling")
	void SetHeadAndTailLights();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Lights Handling")
	void HandeHeadAndTailLights();

	/** Get world temperature in Celsius unit. */
	UFUNCTION(BlueprintPure, Category = "Default")
	void GetWorldTemperature(double& TempP);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ConstructGearBox();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_HeadLightsFarOn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_HeadLightsCloseOn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_SignalLOn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_SignalROn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_TailLightsOn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_BrakeLightsOn();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category = "Replication")
	void OnRep_ReverseLightsOn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Materials")
	void CreateDynamicMaterials();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Materials")
	void SetDynamicMaterialsToMesh();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateCameras();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateExhaustParticles();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateMirrors();

	/** Scene Component Creation Functions */
	UFUNCTION(BlueprintCallable, Category = "SC Creation")
	void CreateWheelMesh(USCWheel* SuspensionP, USkeletalMeshComponent*& WheelMeshP);

	/** Scene Component Creation Functions */
	UFUNCTION(BlueprintCallable, Category = "Component Construction")
	void CreateTireSounds();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category = "Particle Handling")
	void TriggerNitrous(bool EnableP);
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> ReverseLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	TArray<UCameraComponent*> Cameras;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	int32 LastActiveCamIndex;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> CloseHeadLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> FarHeadLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UFMODAudioComponent*> ExhaustSounds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UFMODAudioComponent*> BackfireSounds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> BrakeLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> TailLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> LeftSignalLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Components", meta = (MultiLine = "true"))
	TArray<USpotLightComponent*> RightSignalLights;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	bool CameraReleased;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
	FName RPMTextForEngineAudio;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Light Data", Replicated, meta = (MultiLine = "true", UIMax = "3", ClampMax = "3", ClampMin = "0", UIMin = "0"))
	int32 LightMode;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_HeadLightsFarOn", meta = (MultiLine = "true"))
	bool HeadLightsFarOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_HeadLightsCloseOn", meta = (MultiLine = "true"))
	bool HeadLightsCloseOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_SignalLOn", meta = (MultiLine = "true"))
	bool SignalLOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_SignalROn", meta = (MultiLine = "true"))
	bool SignalROn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TArray<UMaterialInstance*> MI_Array;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	bool InvertMouseLook;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> FarHeadLightsDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> CloseHeadLightsDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> TailLightsDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> SignalLeftDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> SignalRightDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> BrakeLightsDMI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TObjectPtr<UMaterialInstanceDynamic> ReverseLightsDMI;

	/** Camera Socket Names to fit in SkeletalMesh Socket positions. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera Handling", meta = (MultiLine = "true"))
	TArray<FName> CameraSocketNames;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle Components", meta = (MultiLine = "true"))
	TArray<UParticleSystemComponent*> BackfireParticles;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle Components", meta = (MultiLine = "true"))
	TArray<UParticleSystemComponent*> NitrousParticles;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material Handling", meta = (MultiLine = "true"))
	TArray<UCameraComponent*> MirrorCameras;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorFL;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorFR;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorRL;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool DoorRR;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool Trunk;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation Handling", meta = (MultiLine = "true"))
	bool Hood;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_TailLightsOn", meta = (MultiLine = "true"))
	bool TailLightsOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_BrakeLightsOn", meta = (MultiLine = "true"))
	bool BrakeLightsOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", ReplicatedUsing = "OnRep_ReverseLightsOn", meta = (MultiLine = "true"))
	bool ReverseLightsOn;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Light Data", Replicated, meta = (MultiLine = "true", UIMax = "3", ClampMax = "3", ClampMin = "0", UIMin = "0"))
	int32 SignalMode;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UAudioComponent*> TireRollingSounds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sound Components", meta = (MultiLine = "true"))
	TArray<UAudioComponent*> TireSlideSounds;
};
