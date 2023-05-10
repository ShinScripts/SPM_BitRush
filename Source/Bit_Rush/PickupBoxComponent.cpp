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

	// ...
}

APlayerCharacter* UPickupBoxComponent::GetPlayer()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for(AActor* Actor : Actors)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if(!Player)
		{
			ScreenPrint("player null");
			return nullptr;
		}
		ScreenPrint("is player");
		return Player;
	}
}

void UPickupBoxComponent::GiveToPlayer()
{
	APlayerCharacter* Player = GetPlayer();

	if(!Player)
	{
		return;
	}
	
	switch (EPickupType)
	{
	case "Time":
		//Give time to player
		break;
		
	case "Ammo":
		//Give ammo to player
		break;

	default: break;
	}
	
}

//Debug utility
void APlayerCharacter::ScreenPrint(FString Message)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT(""+Message));
	}
}

void APlayerCharacter::ScreenPrint(FString Message, FColor Color)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, Color, TEXT(""+Message));
	}
}
