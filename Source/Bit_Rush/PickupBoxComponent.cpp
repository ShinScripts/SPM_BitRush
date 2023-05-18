// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBoxComponent.h"

// Sets default values for this component's properties
UPickupBoxComponent::UPickupBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickupBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPickupBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForPlayer();
	ObjectRotation();

	// ...
}

void UPickupBoxComponent::CheckForPlayer()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for(AActor* Actor : Actors)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if(!Player)
		{
			return;
		}
		PlayerChar = Player;
		TributeToPlayer();
	}
}

void UPickupBoxComponent::TributeToPlayer()
{
	if(!PlayerChar)
	{
		return;
	}

	switch (PickupType)
	{
	case EPickupType::Pickup_Time:
		PlayerChar->ChangeTime(AddOrSubtract, TimeTribute);
	
	case EPickupType::Pickup_Ammo:
		PlayerChar->ChangeAmmo(AddOrSubtract, MagOrStore,AmmoTribute);

		default: break;
	}
	
	GetOwner()->Destroy();
}

void UPickupBoxComponent::ObjectRotation()
{
	if(RotateObject)
	{
		AActor* Target = this->GetOwner();
        
		if(!Target)
		{
			return;
		}
		FQuat QuatRot = FQuat(FRotator(XRotation, YRotation, ZRotation));
        
		Target->AddActorLocalRotation(QuatRot, false, 0 , ETeleportType::None);
	}
}
