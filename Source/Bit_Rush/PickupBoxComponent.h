// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"

#include "PickupBoxComponent.generated.h"

UENUM(BlueprintType)
enum EPickupType
{
	Time UMETA(DisplayName = "Time"),
	Ammo UMETA(DisplayName = "Ammo"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIT_RUSH_API UPickupBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupBoxComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//Variables
	
	//Functions
	APlayerCharacter* GetPlayer();
	void GiveToPlayer();

	//Debug utility
	void ScreenPrint(FString Message);
	void ScreenPrint(FString Message, FColor Color);
};
