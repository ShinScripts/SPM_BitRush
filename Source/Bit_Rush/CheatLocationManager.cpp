// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatLocationManager.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ACheatLocationManager::ACheatLocationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACheatLocationManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
}

// Called every frame
void ACheatLocationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

