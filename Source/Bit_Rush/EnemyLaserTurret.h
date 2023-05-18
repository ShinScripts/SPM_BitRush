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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Laser", meta = (AllowPrivateAccess))
	UStaticMeshComponent* LaserBeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	float RechargeTimer = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category ="Combat", meta = (AllowPrivateAccess))
	bool CanFire = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	float Damage = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	float RotationSpeed;

	
	UPROPERTY(VisibleDefaultsOnly, Category = "Rockets")
	TSubclassOf<class AActor> LaserClass;
	
	virtual void Tick(float DeltaSeconds) override;
	void Recharge();

	UFUNCTION(BlueprintCallable)
	virtual void Shoot() override;

	UFUNCTION(BlueprintCallable)
	void SetRotationSpeed(float NewRotationSpeed);
	
	void FireLaser();

	virtual void Destroy() override;
};
