// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRocketTurret.h"

#include "RocketProjectile.h"
#include "Components/CapsuleComponent.h"

AEnemyRocketTurret::AEnemyRocketTurret()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret base"));
	TurretBase->SetupAttachment(CapsuleComp);

	TurretTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret tower"));
	TurretTower->SetupAttachment(TurretBase);
	
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	ProjectileSpawnPoint->SetupAttachment(TurretTower);
}

void AEnemyRocketTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyRocketTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//TODO If player in sight, aim at player
	//Shoot on cooldown
}

//Turret fires a rocket that follows the player
void AEnemyRocketTurret::Shoot()
{
	const auto Rocket = GetWorld()->SpawnActor<ARocketProjectile>(RocketProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), TurretTower->GetComponentRotation());

	if (Rocket)
	{
		Rocket->SetOwner(this);
	}
}

void AEnemyRocketTurret::Destroy()
{
}