// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "WallRunMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIT_RUSH_API UWallRunMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallRunMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void SetWallRunVelocity(const FVector& CrossProduct);
	void TiltCamera(UCameraComponent* CameraComp, float RollDegrees, float InterpolationSpeed);

private:
	float LineTraceLength = 120.f;
	float JumpOffWallForce = 1.5f;
	float InterpolationSpeed = 10.f;
	float RollDegrees = 15.f;
	
	bool OnRightSide = false;
	bool OnLeftSide = false;
	bool IsJumpingOffWall = false;
	
	FString WallRunTag = "WallRunnable";
};
