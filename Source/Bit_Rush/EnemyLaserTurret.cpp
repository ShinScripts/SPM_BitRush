// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLaserTurret.h"

#include "PlayerCharacter.h"
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

	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(this, 0);
	FVector LaserStart = LaserSpawnPoint->GetComponentLocation();
	FVector LaserEnd = (LaserSpawnPoint->GetComponentLocation() + (GetActorRotation().Vector() * 2000));

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)));


	GetWorld()->SweepSingleByChannel(LaserHit, LaserStart, LaserEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(12.0f), Params);
	if(CanFire)
	{
		
		LaserBeam->SetWorldScale3D(FVector(	0.5, 0.5, (LaserHit.Location-LaserSpawnPoint->GetComponentLocation()).Size()/200));
	}

}

//Changes bool CanFire to true
void AEnemyLaserTurret::Recharge()
{
	CanFire = true;
}

//Activates the laser of the turret
void AEnemyLaserTurret::FireLaser()
{
	CanFire = false;
	FTimerHandle ChargeHandle;

	LaserBeam->SetWorldScale3D(FVector(	4, 4, (LaserHit.Location-LaserSpawnPoint->GetComponentLocation()).Size()/200));
	UGameplayStatics::ApplyDamage(LaserHit.GetActor(), Damage, GetInstigatorController(), this , UDamageType::StaticClass());
	GetWorldTimerManager().SetTimer(ChargeHandle, this, &AEnemyLaserTurret::Recharge, RechargeTimer, false);
	RotationSpeed = 150;
}

//Sets Rotationspeed of turret
void AEnemyLaserTurret::SetRotationSpeed()
{
	RotationSpeed = 1;
}


//Calls in blueprint when turret should shoot which then calls for FireLaser when laser should activate
void AEnemyLaserTurret::Shoot()
{


	FTimerHandle ChargeHandler;
	FTimerHandle RotationHandler;
	GetWorldTimerManager().SetTimer(RotationHandler, this, &AEnemyLaserTurret::SetRotationSpeed, 1.7, false);
	GetWorldTimerManager().SetTimer(ChargeHandler, this, &AEnemyLaserTurret::FireLaser, 2, false);

}

void AEnemyLaserTurret::Destroy()
{
}
