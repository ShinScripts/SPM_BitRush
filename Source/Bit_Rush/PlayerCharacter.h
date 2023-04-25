// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_BODY()

	void SetCharacterMovement(UCharacterMovementComponent* InCharacterMovementComponent) const;
	void SetCharacterHitBox(UCapsuleComponent* InCharacterMovementComponent) const;
	void SetDefaultValues();
	void SetGroundFriction(const float NewGroundFriction);
	void SetGravityScale(const float NewGravityScale);
	void SetBrakingDecelerationWalking(const float NewBrakingDecelerationWalking);
	void SetFallingLateralFriction(const float NewFallingLateralFriction);

	UPROPERTY(BlueprintReadOnly)
	float GravityScale;

	float BrakingFrictionFactor;

	UPROPERTY(BlueprintReadOnly)
	float FallingLateralFriction;

	float AirControl;
	float GroundFriction;
	float BrakingDecelerationWalking;

	UPROPERTY(BlueprintReadOnly)
	float JumpForce;
};

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
	float SlideVelocity = 5000000;

	UPROPERTY(EditAnywhere)
	float FlatSlideVelocity = 7000000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float CurrentTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bCanMove;

	// Dash
	UPROPERTY(EditAnywhere)
	float DashVelocity = 2000;
	
	UPROPERTY(EditAnywhere)
	float DashTime = 0.15;

	bool CanDash = true;

	bool bIsDashing = false;
	UPROPERTY(EditAnywhere)
	float DashCooldown = 1;

	UPROPERTY()
	class UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess))
	bool bShouldSlide = false;
	
	bool ShouldLaunchSlide = false;

	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess))
	FVector SlideSurfNormal;

	UPROPERTY(EditAnywhere)
	float CharacterSpeed = 1;

	UPROPERTY(EditAnywhere)
	float GrapplingHookRange = 1500;

	UPROPERTY(EditAnywhere)
	float GrapplingSpeed = 3000;

	bool bCanGrapple;

	float CrouchSpeed = 10;
	
	float HitBoxDefaultValue;
	float CrouchHitBoxValue;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FMovementData MovementData;

	FVector DashDistance;
	
	UCharacterMovementComponent* CharacterMovement;
	
	FHitResult FloorHit;
	FHitResult GrappleHit;

	UCapsuleComponent* CharacterHitBox;
	//Functions
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);

	//Dash
	void Dash();
	void StopDash();
	void StartDash();
	void ResetDash();

	//Slide
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime);
	void StopSlide();

	//Grapple
	void CanGrapple();
	void StopGrapple();
	void Grapple();
	//void StopSlidingAfterSeconds();
	FVector GetSlideSurface(const FVector& FloorNormal);

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
};
