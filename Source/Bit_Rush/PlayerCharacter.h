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
	float SlideVelocity = 70000000;

	UPROPERTY(EditAnywhere)
	float FlatSlideVelocity = 6000000;
	
	UPROPERTY(BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Grapple Component", meta=(AllowPrivateAccess))
	class UNiagaraSystem* GrappleNiagra;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReloadTimer = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CurrentReloadTime = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DashCooldown = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

USTRUCT(BlueprintType)
struct FWeaponData
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

	//Ammo
	UPROPERTY(EditAnywhere, Category = "Ammo", BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool UnlimitedAmmo = false;
	UPROPERTY(EditAnywhere, Category = "Ammo", BlueprintReadWrite, meta = (AllowPrivateAccess, EditCondition = "!UnlimitedAmmo"))
	int MaxAmmo = 32;
	UPROPERTY(EditAnywhere, Category = "Ammo", BlueprintReadWrite, meta = (AllowPrivateAccess, EditCondition = "!UnlimitedAmmo"))
	int StoredAmmo ;
	UPROPERTY(EditAnywhere, Category = "Ammo", BlueprintReadWrite, meta = (AllowPrivateAccess))
	int AmmoMagCapacity = 8;
	UPROPERTY(VisibleAnywhere, Category = "Ammo", BlueprintReadWrite, meta = (AllowPrivateAccess))
	int CurrentMagAmmo;
	
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

	//Tribute resource
	void ChangeTime(bool AddOrTake, float Tribute); //AddOrTake = true -> add / = false -> subtract
	void ChangeAmmo(bool AddOrTake, bool MagOrStore, int Tribute); //MagOrStore = true -> Mag / false -> Storage.

	UFUNCTION(BlueprintCallable)
	FVector GetGrappleRotation() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float InvincibilityTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float CurrentTime;

	UPROPERTY(EditAnywhere)
	float CoyoteTime = 0.2;

	float CurrentCoyoteTime = CoyoteTime;

	bool bCanJump = true;
	//Ammo
	UPROPERTY(EditAnywhere, Category = "Ammo")
	bool FullMagAtStart = true;
	UPROPERTY(EditAnywhere, Category = "Ammo")
	bool FullAmmoStoreAtStart = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ammo", meta = (AllowPrivateAccess, EditCondition = "!FullAmmoStoreAtStart"))
	int StoredAmmoAtStart;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ammo", meta = (AllowPrivateAccess, EditCondition = "!FullMagAtStart"))
	int MagAmmoAtStart;

	/*UPROPERTY(EditAnywhere, Category = "MyActor", meta = (EditCondition = "bMyVariable"))
	bool bMyOtherVariable;*/
	//
	
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

	//Ammo
	void SetStartAmmo();
	void SubtractAmmoWhileUnlimited(bool MagOrStore, int Tribute);
	void AddAmmoWhileUnlimited(bool MagOrStore, int Tribute);
};
