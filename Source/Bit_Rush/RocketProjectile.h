// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketProjectile.generated.h"

UCLASS()
class BIT_RUSH_API ARocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	float ProjectileDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta=(AllowPrivateAccess))
	class UNiagaraSystem* HitParticles;
	
	UFUNCTION(BlueprintCallable)
	void SpawnHitParticles(UNiagaraSystem* ParticlesToSpawn, FVector ScaleOfSystem) const;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Combat", meta=(AllowPrivateAccess))
	class UNiagaraSystem* HitParticlesPlayer;
	

	
};
