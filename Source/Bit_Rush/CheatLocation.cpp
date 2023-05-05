// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatLocation.h"
#include "CheatLocationManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheatLocation::ACheatLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}

// Called when the game starts or when spawned
void ACheatLocation::BeginPlay()
{
	Super::BeginPlay();
	CheatLocationManager = Cast<ACheatLocationManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ACheatLocationManager::StaticClass()));
	if(CheatLocationManager)
	{
		UE_LOG(LogTemp,Warning,TEXT("FOUND MANAGER"));
		CheatLocationManager->Locations.Add(GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("NOT FOUND MANAGER"));
	}

}

