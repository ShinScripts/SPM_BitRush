// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectorBoxComponent.h"

void UDeflectorBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Test.")); //It works at least.
}

void UDeflectorBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//-----------------------------------------------------------------------------------------------------------------
	
	CheckOverlappingTags(ProjectileTags);
	//UE_LOG(LogTemp, Warning, TEXT("Vozmozshmno..."));
}

void UDeflectorBoxComponent::CheckOverlappingTags(TArray<FName> Tags) const
{
	
	TArray<AActor*> Actors;
	
	GetOverlappingActors(Actors);
	for(AActor* TargetActor : Actors)
	{
		for(FName CurrentTag : Tags)
		{
			if(Tags.Num() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is overlapping without tag"), *TargetActor->GetActorNameOrLabel());
				//return;
			}
			if (TargetActor->ActorHasTag(CurrentTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is overlapping with tag"), *TargetActor->GetActorNameOrLabel());
				//return TargetActor;
			}
		}
		
	}
	//return nullptr;
}

