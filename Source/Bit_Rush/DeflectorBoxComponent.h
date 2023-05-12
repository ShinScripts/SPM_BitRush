// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Containers/Array.h"

#include "DeflectorBoxComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIT_RUSH_API UDeflectorBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UDeflectorBoxComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame-------------------------------------------
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//FName GetDeflectInput();
	void StartDeflect();
	void StopDeflect();
	
	
	
private:
	UPROPERTY(EditAnywhere, Category="Deflector Component", BlueprintReadOnly,meta = (AllowPrivateAccess))
	bool IsDeflecting;
	
	//void BindInputs(UInputComponent* Component);
	
	void ScreenPrint(FString Message);
	void CheckOverlappingTags(TArray<FName> Tags);
	
	UPROPERTY(EditAnywhere, Category="Deflector Component")
	TArray<FName> DeflectableProjectileTags;

	float CurrentDeflectCooldown;
	float DeflectCooldown = 2;
	bool bIsDeflecting = false;

	/*UPROPERTY(EditAnywhere, Category="Deflector Component")
	APlayerCharacter* Player;*/
};
