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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretTower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* WeakSpot;

	UPROPERTY(EditDefaultsOnly, Category = "Rockets")
	TSubclassOf<class ARocketProjectile> RocketProjectileClass;
	
	virtual void Destroy() override;

	UFUNCTION(BlueprintCallable)
	virtual void Shoot() override;	
};
