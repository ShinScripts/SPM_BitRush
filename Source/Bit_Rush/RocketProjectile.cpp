// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketProjectile.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ARocketProjectile::ARocketProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARocketProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARocketProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocketProjectile::SpawnHitParticles(UNiagaraSystem* ParticlesToSpawn, FVector ScaleOfSystem) const
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ParticlesToSpawn, GetActorLocation(), GetActorRotation(), ScaleOfSystem);
}
