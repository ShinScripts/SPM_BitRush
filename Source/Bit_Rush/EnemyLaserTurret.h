// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyPawn.h"
#include "EnemyLaserTurret.generated.h"


UCLASS()
class BIT_RUSH_API AEnemyLaserTurret : public AEnemyPawn
{
	GENERATED_BODY()
	FHitResult HitResult;
	
public: 
	AEnemyLaserTurret();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* TurretTower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	USceneComponent* LaserSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Body", meta = (AllowPrivateAccess))
	UStaticMeshComponent* WeakSpot;
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void Shoot() override;
	virtual void Destroy() override;
};
