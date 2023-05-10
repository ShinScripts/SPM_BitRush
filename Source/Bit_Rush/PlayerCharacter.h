// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeflectorBoxComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FSlideComponent
{
	GENERATED_USTRUCT_BODY()

	void Initialize(FMovementData* InMovementData, APlayerCharacter* InPlayerCharacter);
	void SlideBegin();
	void SlideUpdate(float DeltaTime);
	void EnterSlide();
	void ExitSlide();
	void Slide(float DeltaTime);
	void StopSlide();
	
	FVector GetSlideSurface(const FVector &FloorNormal);

	UPROPERTY(EditAnywhere)
	float SlideVelocity = 5000000;

	UPROPERTY(EditAnywhere)
	float FlatSlideVelocity = 7000000;
	
	bool bShouldSlide = false;
	
	bool ShouldLaunchSlide = false;

	float HitBoxDefaultValue;
	
	float CrouchHitBoxValue;

	float CrouchSpeed = 10;

	UPROPERTY(EditAnywhere)
	float MaxSlideVelocity = 3000;
	
	FVector SlideSurfNormal;
	
	FHitResult FloorHit;
	
	APlayerCharacter* PlayerCharacter;
	FMovementData* MovementData;
};

USTRUCT(BlueprintType)
struct FGrappleComponent
{
	GENERATED_USTRUCT_BODY()

	void Initialize(APlayerCharacter* InPlayerCharacter);
	void GrappleUpdate();
	void ScanForGrapplePoint();
	void StartGrapple();
	void StopGrapple();
	void Grapple();

	UPROPERTY(EditAnywhere)
	float GrapplingHookRange = 1500;

	UPROPERTY(EditAnywhere)
	float GrapplingSpeed = 3000;

	UPROPERTY(EditAnywhere)
	float GrapplingLaunchSpeed = 2000;

	UPROPERTY(BlueprintReadOnly)
	bool bCanGrapple;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGrappling;

	FHitResult GrappleHit;

	APlayerCharacter* PlayerCharacter;
};

USTRUCT(BlueprintType)
struct FGunComponent
{
	GENERATED_USTRUCT_BODY()

	void Initialize(APlayerCharacter* InPlayerCharacter);
	void GunUpdate(float DeltaTime);
	void Reload();

	UPROPERTY(EditDefaultsOnly)
	float ReloadTimer = 2;
	
	float CurrentReloadTime = 2;

	bool bIsReloading = false;
	
	APlayerCharacter* PlayerCharacter;
};

USTRUCT(BlueprintType)
struct FDashComponent
{
	GENERATED_USTRUCT_BODY()

	void Initialize(APlayerCharacter* PlayerCharacter);
	void DashUpdate(float DeltaTime);
	
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

	UPROPERTY(BlueprintReadOnly)
	bool bIsDashing = false;

	float CurrentDashTime;
	
	FVector DashDistance;
	FVector DashDirection;
	
	APlayerCharacter* PlayerCharacter;
};

USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_USTRUCT_BODY()

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	int Ammo = 8;
	
	//Deflect
	UFUNCTION(BlueprintCallable)
	UDeflectorBoxComponent* GetDeflectorBox();

	UPROPERTY(BlueprintReadWrite)
	FMovementData MovementData;
	UPROPERTY(BlueprintReadWrite)
	FDashComponent DashComponent;
	UPROPERTY(BlueprintReadWrite)
	FGunComponent GunComponent;
	UPROPERTY(BlueprintReadWrite)
	FGrappleComponent GrappleComponent;
	UPROPERTY(BlueprintReadWrite)
	FSlideComponent SlideComponent;
	
	UCharacterMovementComponent* CharacterMovement;
	UCapsuleComponent* CharacterHitBox;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float InvincibilityTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float CurrentTime;
	
	//Functions
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);
	void Jump();

	//ActionFunctions
	void ActionReload();
	void ActionStartDash();
	void ActionStartGrapple();
	void ActionEnterSlide();
	void ActionExitSlide();
	
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
