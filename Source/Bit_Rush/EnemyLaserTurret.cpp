// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLaserTurret.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyLaserTurret::AEnemyLaserTurret()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret base"));
	TurretBase->SetupAttachment(CapsuleComp);

	WeakSpot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weakspot"));
	WeakSpot->SetupAttachment(CapsuleComp);

	TurretTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret tower"));
	TurretTower->SetupAttachment(TurretBase);

	LaserSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	LaserSpawnPoint->SetupAttachment(TurretTower);
}

void AEnemyLaserTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Shoot();
	//TODO if player in sight track enemy and aim with a lead
	//shoot on cooldown
}

//Turret creates an outliner for the laser that is about to activate
void AEnemyLaserTurret::Shoot()
{
	//TODO Render a line
	//stop turret movement
	//generate damaging laser

	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	FVector LaserStart = LaserSpawnPoint->GetComponentLocation();
	FVector LaserEnd = (LaserSpawnPoint->GetComponentLocation() + (GetActorRotation().Vector() * 2000));

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// TODO add all enemies? Params.AddIgnoredActor()
	GetWorld()->SweepSingleByChannel(Hit, LaserStart, LaserEnd, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(12.0f), Params);
	DrawDebugLine(GetWorld(), LaserStart, Hit.Location, FColor::Red, false, 1.f, 0, 10);

	//	FColor(255,0,0),true , 0, 0, 1); 
}

void AEnemyLaserTurret::Destroy()
{
}
