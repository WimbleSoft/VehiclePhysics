// Fill out your copyright notice in the Description page of Project Settings.


#include "Wheel.h"

// Sets default values
AWheel::AWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetupAttachment(RootComponent);

	Caliper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Caliper"));
	Caliper->SetupAttachment(DefaultSceneRoot);

	Rim = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rim"));
	Rim->SetupAttachment(DefaultSceneRoot);

	Disc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disc"));
	Disc->SetupAttachment(Rim);

	Tire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tire"));
	Tire->SetupAttachment(Rim);
}

// Called when the game starts or when spawned
void AWheel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

