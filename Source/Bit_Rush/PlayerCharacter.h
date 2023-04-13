// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BIT_RUSH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//Variable
	UPROPERTY(EditAnywhere)
	float SlideVelocity = 3000;

	UPROPERTY(EditAnywhere)
	float SlideLaunchVelocity = 500000;
	
	UPROPERTY(VisibleAnywhere)
	bool CanMove;

	UPROPERTY(EditAnywhere)
	float DashVelocity = 2000;

	UPROPERTY(EditAnywhere)
	FHitResult Hit;

	UPROPERTY(EditAnywhere)
	float DashLength = 0.15;

	UPROPERTY()
	class UCameraComponent* CameraComp;

	bool ShouldSlide = false;
	bool ShouldLaunchSlide = false;
	FVector SlideSurfNormal;
	class UCharacterMovementComponent* CharacterMovement;

	struct FHitResult FloorHit;
	//Functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void Dash();
	void StopVelocity();
	
	void EnterSlide();
	void ExitSlide();
	void PhysSlide();
	void StopSlidingAfterSeconds();
	FVector GetSlideSurface(FVector);
};
