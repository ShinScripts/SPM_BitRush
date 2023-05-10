// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeflectorBoxComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FDashComponent
{
	GENERATED_BODY()

	void Initialize(APlayerCharacter* PlayerCharacter);
	void Update(float DeltaTime);
	
	void Dash(float DeltaTime);
	void StartDash();

	UPROPERTY(EditAnywhere)
	float DashCooldown = 1;
	
	float DashCurrentCooldown = DashCooldown;
	
	UPROPERTY(EditAnywhere)
	float DashVelocity = 2000;
	
	UPROPERTY(EditAnywhere)
	float DashTime = 0.15;

	bool bDashIsOnCooldown = false;

	bool bIsDashing = false;

	float CurrentDashTime;
	
	FVector DashDistance;
	FVector DashDirection;
	
	APlayerCharacter* PlayerCharacter;
};

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

	UPROPERTY(BlueprintReadWrite)
	float GravityScale;

	UPROPERTY(BlueprintReadWrite)
	float FallingLateralFriction;

	UPROPERTY(BlueprintReadWrite)
	float JumpForce;

	UPROPERTY(BlueprintReadWrite)
	float AirControl;
	
	float GroundFriction;
	float BrakingDecelerationWalking;
	float BrakingFrictionFactor;
	float MaxAcceleration;
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

	UPROPERTY()
	class UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanMove;

	//Deflect
	UFUNCTION(BlueprintCallable)
	UDeflectorBoxComponent* GetDeflectorBox();

	FMovementData MovementData;
	FDashComponent DashComponent;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxLeft;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxRight;
	
	//Variable
	UPROPERTY(EditAnywhere)
	float SlideVelocity = 5000000;

	UPROPERTY(EditAnywhere)
	float FlatSlideVelocity = 7000000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float CurrentTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	int MaxAmmo = 8;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	int Ammo = 8;

	// Dash
	UPROPERTY(EditAnywhere)
	float DashVelocity = 2000;
	
	UPROPERTY(EditAnywhere)
	float DashTime = 0.15;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool CanDash = true;

	bool bIsDashing = false;
	
	UPROPERTY(EditAnywhere)
	float DashCooldown = 1;

	UPROPERTY(EditAnywhere)
	float MaxSlideVelocity = 3000;

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

	UPROPERTY(EditAnywhere)
	float GrapplingLaunchSpeed = 2000;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bCanGrapple;
	bool bIsGrappling;

	float CrouchSpeed = 10;
	
	float HitBoxDefaultValue;
	float CrouchHitBoxValue;

	FVector DashDistance;
	FVector DashDirection;
	
	UCharacterMovementComponent* CharacterMovement;
	
	FHitResult FloorHit;
	FHitResult GrappleHit;

	UCapsuleComponent* CharacterHitBox;
	
	//Functions
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);
	void Jump();

	//Dash
	void ActionStartDash();
	/*void Dash();
	void StopDash();
	void StartDash();
	void ResetDash();
	*/
	//Slide
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float DeltaTime);
	void StopSlide();

	//Grapple
	void ScanGrapple();
	void StartGrapple();
	void StopGrapple();
	void Grapple();
	//void StopSlidingAfterSeconds();
	FVector GetSlideSurface(const FVector& FloorNormal);

	//Deflect
	// - variables
	UDeflectorBoxComponent* DeflectorBox;
	// - functions
	void DeflectON();
	void DeflectOFF();
	void SetDeflectBoxVariable();

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	//Debug utility
	void ScreenPrint(FString Message);
	void ScreenPrint(FString Message, FColor Color);

	//Shoot
	/*UFUNCTION(BlueprintCallable)
	void Shoot();*/
};
