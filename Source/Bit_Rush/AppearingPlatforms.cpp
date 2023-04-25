// Fill out your copyright notice in the Description page of Project Settings.


#include "AppearingPlatforms.h"

#include "Components/SphereComponent.h"


// Sets default values
AAppearingPlatforms::AAppearingPlatforms()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	Platform->SetupAttachment(RootComponent);

	HitBox = CreateDefaultSubobject<USphereComponent>("HitBox");
	HitBox->SetupAttachment(Platform);
	HitBox->SetSphereRadius(HitBoxRadius);
}

// Called when the game starts or when spawned
void AAppearingPlatforms::BeginPlay()
{
	Super::BeginPlay();

	HideActor(true);
}

// Called every frame
void AAppearingPlatforms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAppearingPlatforms::HideActor(const bool ShouldHide)
{
	// Hides the actor 
	SetActorHiddenInGame(ShouldHide);

	// TODO: Disable collision
	// SetActorEnableCollision(!ShouldHide);
	
	// Stop it from ticking
	SetActorTickEnabled(!ShouldHide);
}

