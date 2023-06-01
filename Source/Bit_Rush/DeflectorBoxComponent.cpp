// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectorBoxComponent.h"

UDeflectorBoxComponent::UDeflectorBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDeflectorBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	IsDeflecting = false;
	CurrentDeflectCooldown = DeflectCooldown;
}

void UDeflectorBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//-----------------------------------------------------------------------------------------------------------------

	if(IsDeflecting)
	{
		if(DeflectCooldown-CurrentDeflectCooldown<0.4)
		{
			CheckOverlappingTags(DeflectableProjectileTags);
		}
		CurrentDeflectCooldown -= DeltaTime;
	}
	if (CurrentDeflectCooldown <= 0)
	{
		IsDeflecting = false;
		CurrentDeflectCooldown = DeflectCooldown;
	}

}

void UDeflectorBoxComponent::CheckOverlappingTags(TArray<FName> Tags)
{
	
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	
	for(AActor* TargetActor : Actors)
	{
			if (TargetActor == nullptr)
			{
				return;
			}
			if (TargetActor != nullptr)
			{
				for(FName CurrentTag : Tags)
				{
					if (TargetActor->ActorHasTag(CurrentTag))
					{
						if(IsDeflecting)
						{
							TargetActor->Destroy();
						}
					}
				}
			}
	}
}

void UDeflectorBoxComponent::StartDeflect()
{
	IsDeflecting = true;
	
}
void UDeflectorBoxComponent::StopDeflect()
{
	IsDeflecting = false;
	CurrentDeflectCooldown = DeflectCooldown;
}
