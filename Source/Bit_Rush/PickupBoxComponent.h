// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"

#include "PickupBoxComponent.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
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
	// - Variables
	
	// - Functions
	UPickupBoxComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// - Variables
	//bool - (EditAnywhere)
	UPROPERTY(EditAnywhere, Category="Pickup")
	bool AddOrSubtract;

	UPROPERTY(EditAnywhere, Category="Pickup")
	bool MagOrStore;
	
	//float - (EditAnywhere)
	UPROPERTY(EditAnywhere, Category="Pickup")
	float TributeCount = 0;
	
	//APlayerCharacter
	APlayerCharacter* PlayerChar;
	
	//TEnumAsByte - (EditAnywhere)
	UPROPERTY(EditAnywhere, Category="Pickup")
	EPickupType PickupType;
	
	// - Functions - void
	void CheckForPlayer();
	void TributeToPlayer();

};
