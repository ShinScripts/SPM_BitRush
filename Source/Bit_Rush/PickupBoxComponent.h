// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"

#include "PickupBoxComponent.generated.h"

//UENUM(BlueprintType)
enum EPickupType
{
	Pickup_Time UMETA(DisplayName = "Time"),
	Pickup_Ammo UMETA(DisplayName = "Ammo"),
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
	// - Variables
	//float - (EditAnywhere)
	UPROPERTY(EditAnywhere)
	float TributeCount = 0;
	//APlayerCharacter
	APlayerCharacter* PlayerChar;
	//TEnumAsByte
	TEnumAsByte<EPickupType> PickupType;
	
	// - Functions
	void GetPlayer();
	void GiveToPlayer();
};
