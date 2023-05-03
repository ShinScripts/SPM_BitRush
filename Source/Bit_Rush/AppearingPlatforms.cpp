// Fill out your copyright notice in the Description page of Project Settings.


#include "AppearingPlatforms.h"


// Sets default values
AAppearingPlatforms::AAppearingPlatforms()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	Platform->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAppearingPlatforms::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAppearingPlatforms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

