// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLaserTurret.h"

#include "PlayerCharacter.h"
#include "SAdvancedTransformInputBox.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyLaserTurret::AEnemyLaserTurret()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret base"));
	TurretBase->SetupAttachment(CapsuleComp);

	TurretTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret tower"));
	TurretTower->SetupAttachment(TurretBase);

	LaserSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile spawn point"));
	LaserSpawnPoint->SetupAttachment(TurretTower);

	LaserBeam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laserbeam"));
	LaserBeam->SetupAttachment(TurretTower);


}

void AEnemyLaserTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AEnemyLaserTurret::Recharge()
{
	CanFire = true;
}
void AEnemyLaserTurret::FireLaser()
{


	CanFire = false;
	FTimerHandle ChargeHandle;
	FVector LaserStart = LaserSpawnPoint->GetComponentLocation();
	FVector LaserEnd = (LaserSpawnPoint->GetComponentLocation() + (GetActorRotation().Vector() * 2000));

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)));


	GetWorld()->SweepSingleByChannel(Hit, LaserStart, LaserEnd, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(12.0f), Params);
	//DrawDebugLine(GetWorld(), LaserStart, Hit.Location, FColor::Red, false, RechargeTimer, 10, 15);
	GetWorldTimerManager().SetTimer(ChargeHandle, this, &AEnemyLaserTurret::Recharge, RechargeTimer, false);
}
//Turret creates an outliner for the laser that is about to activate
void AEnemyLaserTurret::Shoot()
{
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	FVector LaserStart = LaserSpawnPoint->GetComponentLocation();
	FVector LaserEnd = (LaserSpawnPoint->GetComponentLocation() + (GetActorRotation().Vector() * 2000));

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)));


	GetWorld()->SweepSingleByChannel(Hit, LaserStart, LaserEnd, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(12.0f), Params);
	
	
	//DrawDebugLine(GetWorld(), LaserStart, Hit.Location, FColor::Red, false, 0, 10, 5);
	FTimerHandle ChargeHandl;
	GetWorldTimerManager().SetTimer(ChargeHandl, this, &AEnemyLaserTurret::FireLaser, 2, false);

	LaserBeam->SetWorldScale3D(FVector(	1, 1, (Hit.Location-LaserStart).Size()/100));
}


void AEnemyLaserTurret::Destroy()
{
}
