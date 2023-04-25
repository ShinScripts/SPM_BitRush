// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AppearingPlatforms.generated.h"

class USphereComponent;

UCLASS()
class BIT_RUSH_API AAppearingPlatforms : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAppearingPlatforms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Platform = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HitBox = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float HitBoxRadius = 7500.f;
	
	UFUNCTION(BlueprintCallable)
	void DisableActor(const bool ShouldDisable);	
};
