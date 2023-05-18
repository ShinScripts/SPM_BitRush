// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPawn.h"
#include "EnemyRocketTurret.generated.h"


UCLASS()
class BIT_RUSH_API AEnemyRocketTurret : public AEnemyPawn
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	AEnemyRocketTurret();
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Combat", meta=(AllowPrivateAccess))
	float targetRange = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Combat", meta=(AllowPrivateAccess))
	float AttackFrequency;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretBase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretTower;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Rockets")
	TSubclassOf<class ARocketProjectile> RocketProjectileClass;
	
	virtual void Destroy() override;

	UFUNCTION(BlueprintCallable)
	virtual void Shoot() override;	
};
