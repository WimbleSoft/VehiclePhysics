#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wheel.generated.h"

/** Please add a class description */
UCLASS(Blueprintable, BlueprintType)
class VEHICLEPHYSICS_API AWheel : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWheel();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> Caliper;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> Disc;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> Rim;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> Tire;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
