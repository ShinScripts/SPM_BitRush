// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

void FMovementData::SetCharacterMovement(UCharacterMovementComponent* InCharacterMovementComponent) const
{
	InCharacterMovementComponent->GravityScale = GravityScale;
	InCharacterMovementComponent->BrakingFrictionFactor = BrakingFrictionFactor;
	InCharacterMovementComponent->FallingLateralFriction = FallingLateralFriction;
	InCharacterMovementComponent->AirControl = AirControl;
	InCharacterMovementComponent->GroundFriction = GroundFriction;
	InCharacterMovementComponent->BrakingDecelerationWalking = BrakingDecelerationWalking;
	InCharacterMovementComponent->JumpZVelocity = JumpForce;
	InCharacterMovementComponent->MaxAcceleration = MaxAcceleration;
}

void FMovementData::SetDefaultValues()
{
	JumpForce = 700;
	GravityScale = 2;
	BrakingFrictionFactor = 0.05;
	FallingLateralFriction = 8;
	AirControl = 0.4;
	GroundFriction = 10;
	BrakingDecelerationWalking = 5000;
	MaxAcceleration = 3200;
}

void FMovementData::SetGroundFriction(const float NewGroundFriction)
{
	GroundFriction = NewGroundFriction;
}

void FMovementData::SetGravityScale(const float NewGravityScale)
{
	GravityScale = NewGravityScale;
}

void FMovementData::SetBrakingDecelerationWalking(const float NewBrakingDecelerationWalking)
{
	BrakingDecelerationWalking = NewBrakingDecelerationWalking;
}

void FMovementData::SetFallingLateralFriction(const float NewFallingLateralFriction)
{
	FallingLateralFriction = NewFallingLateralFriction;
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CharacterMovement = GetCharacterMovement();
	MovementData.SetDefaultValues();

	// BoxLeft = CreateDefaultSubobject<UBoxComponent>("BoxLeft");
	// BoxLeft->SetupAttachment(RootComponent);
	//
	// BoxRight = CreateDefaultSubobject<UBoxComponent>("BoxRight");
	// BoxRight->SetupAttachment(RootComponent);
	//
	// BoxLeft->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
	// BoxRight->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//HitBoxDefaultValue = CharacterHitBox->GetScaledCapsuleHalfHeight();
	bCanMove = true;
	CameraComp = FindComponentByClass<UCameraComponent>();
	CharacterHitBox = FindComponentByClass<UCapsuleComponent>();
	HitBoxDefaultValue = CharacterHitBox->GetScaledCapsuleHalfHeight();
	CrouchHitBoxValue = HitBoxDefaultValue/2;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MovementData.SetCharacterMovement(CharacterMovement);
	FloorHit = CharacterMovement->CurrentFloor.HitResult;
	SlideSurfNormal = GetSlideSurface(FloorHit.Normal);
	
	if(bShouldSlide)
	{
		CharacterHitBox->SetCapsuleHalfHeight(FMath::FInterpTo(CharacterHitBox->GetScaledCapsuleHalfHeight(),CrouchHitBoxValue,DeltaTime,CrouchSpeed));
		PhysSlide(DeltaTime);
	}
	else
	{
		CharacterHitBox->SetCapsuleHalfHeight(FMath::FInterpTo(CharacterHitBox->GetScaledCapsuleHalfHeight(),HitBoxDefaultValue,DeltaTime,CrouchSpeed));
	}
	
	if(bIsDashing)
	{
		Dash();
	}

	if(bCanGrapple)
	{
		Grapple();
	}

	//Reduces players time left
	CurrentTime -= GetWorld()->GetDeltaSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("Time %f"), CurrentTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Binding Axis
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MousePitch"),this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MouseYaw"),this, &APawn::AddControllerYawInput);

	//Binding Action
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Dash"),EInputEvent::IE_Pressed,this, &APlayerCharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Pressed,this, &APlayerCharacter::EnterSlide);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Released,this,&APlayerCharacter::ExitSlide);
	PlayerInputComponent->BindAction(TEXT("Grapple"),EInputEvent::IE_Pressed,this,&APlayerCharacter::CanGrapple);
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	if(bCanMove)
	{
		AddMovementInput(GetActorForwardVector() * AxisValue);
		DashDirection.X = AxisValue;
	}
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	if(bCanMove)
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
		DashDirection.Y = AxisValue;
	}
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	
	if(bCanGrapple)
	{
		StopGrapple();
		LaunchCharacter(CameraComp->GetComponentRotation().Vector(),true,true);
	}
}

void APlayerCharacter::Dash()
{
	CanDash = false;
	bIsDashing = true;
	const float DashSpeed = (GetActorLocation() - DashDistance).Length()/DashTime;
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), DashDistance + GetActorLocation(),GetWorld()->DeltaTimeSeconds,DashSpeed));
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&APlayerCharacter::StopDash,DashTime,false);
}

void APlayerCharacter::StopDash()
{
	bIsDashing = false;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,this,&APlayerCharacter::ResetDash,DashCooldown,false);
}

void APlayerCharacter::StartDash()
{
	if(!CanDash) return;
	
	bIsDashing = true;
	DashDistance = GetActorRotation().RotateVector(DashDirection.GetSafeNormal()) * 20;
	DashDistance.Z = 0;
}

void APlayerCharacter::ResetDash()
{
	CanDash = true;
}

void APlayerCharacter::EnterSlide()
{
	bShouldSlide = true;
	MovementData.SetGroundFriction(0);
	MovementData.SetGroundFriction(0);
	MovementData.SetBrakingDecelerationWalking(1000);
	MovementData.SetFallingLateralFriction(0);
	
	if(FloorHit.Normal.Equals(GetActorUpVector()))
	{
		CharacterMovement->AddImpulse(GetVelocity().GetSafeNormal() * FlatSlideVelocity * GetWorld()->DeltaTimeSeconds);
	}
		
}

void APlayerCharacter::ExitSlide()
{
	bShouldSlide = false;
	bCanMove = true;
	MovementData.SetDefaultValues();
}

void APlayerCharacter::PhysSlide(float DeltaTime)
{
	bCanMove = false;
	if(CharacterMovement->Velocity.Length() > MaxSlideVelocity)
	{
		CharacterMovement->Velocity.GetSafeNormal() *= MaxSlideVelocity;
	}
	else
	{
		CharacterMovement->AddForce(SlideSurfNormal);
	}
	
}

void APlayerCharacter::StopSlide()
{
	if(SlideSurfNormal.Equals(FVector::Zero()))
		ExitSlide();
}


void APlayerCharacter::CanGrapple()
{
	if(bCanGrapple)
		return;
	
	const FVector TraceStart = CameraComp->GetComponentLocation();
	const FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,0,0,5);
	GetWorld()->SweepSingleByChannel(GrappleHit,TraceStart,TraceEnd,FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(20),QueryParams);
	
	if(GrappleHit.Component != nullptr && GrappleHit.Component->ComponentHasTag("GrapplePoint"))
	{
		CharacterMovement->Velocity = FVector::Zero();
		bCanGrapple = true;
	}
}

void APlayerCharacter::StopGrapple()
{
	bCanGrapple = false;
	CharacterMovement->SetMovementMode(MOVE_Walking);
	LaunchCharacter(CameraComp->GetComponentRotation().Vector() * GrapplingLaunchSpeed,true,true);
}

void APlayerCharacter::Grapple()
{
	//FVector GrapplingDirection = GrappleHit.Location - GetActorLocation();
	CharacterMovement->SetMovementMode(MOVE_Flying);
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.ImpactPoint, GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.Location).Length() < 50)
		StopGrapple();
}

FVector APlayerCharacter::GetSlideSurface(const FVector& FloorNormal)
{
	if(FloorNormal.Equals(GetActorUpVector()))
		return FVector::Zero();
	
	const FVector CrossVector = FVector::CrossProduct(FloorNormal,GetActorUpVector());
	const FVector CrossCrossVector = FVector::CrossProduct(FloorNormal,CrossVector.GetSafeNormal());
	const float Direction = 1 - FVector::DotProduct(FloorNormal,GetActorUpVector());
	const FVector FloorInfluence = FMath::Clamp(Direction,0,1) * SlideVelocity *  CrossCrossVector.GetSafeNormal();
	
	return FloorInfluence;
}

//Overrides TakeDamage
float APlayerCharacter::TakeDamage
(
	float DamageAmount,
	struct FDamageEvent const & DamageEvent,
	class AController * EventInstigator,
	AActor * DamageCauser
)
{
	CurrentTime -= DamageAmount;
	return CurrentTime;
}

// Shahin
void APlayerCharacter::SetDefaultMovementDataValues()
{
	MovementData.SetDefaultValues();
}
