// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheatLocation.generated.h"

UCLASS()
class BIT_RUSH_API ACheatLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheatLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ACheatLocationManager* CheatLocationManager;
};
