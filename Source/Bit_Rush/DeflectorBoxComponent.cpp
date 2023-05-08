// Fill out your copyright notice in the Description page of Project Settings.


#include "DeflectorBoxComponent.h"

#include "PlayerCharacter.h"
#include "Engine/GameEngine.h"

UDeflectorBoxComponent::UDeflectorBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDeflectorBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	//BindInputs(Player->);
	//UE_LOG(LogTemp, Warning, TEXT("can start.")); //It works at least.
}

void UDeflectorBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//-----------------------------------------------------------------------------------------------------------------
	//UE_LOG(LogTemp, Warning, TEXT("can tick."));
	CheckOverlappingTags(DeflectableProjectileTags);
	//UE_LOG(LogTemp, Warning, TEXT("Vozmozshmno..."));
	//ScreenPrint("Balls");
}

void UDeflectorBoxComponent::ScreenPrint(FString Message)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT(""+Message));
	}
}

void UDeflectorBoxComponent::CheckOverlappingTags(TArray<FName> Tags)
{
	
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	
	for(AActor* TargetActor : Actors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s is overlapping without tag"), *TargetActor->GetActorNameOrLabel());
		
			/*if(Tags.Num() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is overlapping without tag"), *TargetActor->GetActorNameOrLabel());
				//return;
			}*/
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
						//UE_LOG(LogTemp, Warning, TEXT("%s is overlapping with %s using the tag: %s"), *TargetActor->GetActorNameOrLabel(), *GetOwner()->GetActorNameOrLabel(), *CurrentTag.ToString());
						ScreenPrint(TargetActor->GetActorNameOrLabel() + " is overlapping with " + GetOwner()->GetActorNameOrLabel() + " using the tag: " + CurrentTag.ToString());
						if(IsDeflecting)
						{
							ScreenPrint(TargetActor->GetActorNameOrLabel() + " was deflected");
							TargetActor->Destroy();
						}
						//BindAxisToDeflect("DeflectActor", TargetActor);
					}
				}
			}
		
		
	}
	//return nullptr;
}

/*void UDeflectorBoxComponent::BindInputs(UInputComponent* Component)
{
	Component->BindAction(TEXT("Deflect"), IE_Pressed, this, &UDeflectorBoxComponent::StartDeflect);
	Component->BindAction(TEXT("Deflect"), IE_Released, this, &UDeflectorBoxComponent::StopDeflect);
}*/

void UDeflectorBoxComponent::StartDeflect()
{
	IsDeflecting = true;
}
void UDeflectorBoxComponent::StopDeflect()
{
	IsDeflecting = false;
}
