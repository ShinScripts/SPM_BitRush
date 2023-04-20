// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "AITypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void FMovementData::SetCharacterMovement(UCharacterMovementComponent* InCharacterMovementComponent) const
{
	InCharacterMovementComponent->GravityScale = GravityScale;
	InCharacterMovementComponent->BrakingFrictionFactor = BrakingFrictionFactor;
	InCharacterMovementComponent->FallingLateralFriction = FallingLateralFriction;
	InCharacterMovementComponent->AirControl = AirControl;
	InCharacterMovementComponent->GroundFriction = GroundFriction;
	InCharacterMovementComponent->BrakingDecelerationWalking = BrakingDecelerationWalking;
	InCharacterMovementComponent->JumpZVelocity = JumpForce;
}

void FMovementData::SetDefaultValues()
{
	JumpForce = 700;
	GravityScale = 2;
	BrakingFrictionFactor = 0;
	FallingLateralFriction = 8;
	AirControl = 20;
	GroundFriction = 20;
	BrakingDecelerationWalking = 3000;
}

void FMovementData::SetGroundFriction(float NewGroundFriction)
{
	GroundFriction = NewGroundFriction;
}

void FMovementData::SetGravityScale(float NewGravityScale)
{
	GravityScale = NewGravityScale;
}

void FMovementData::SetBrakingDecelerationWalking(float NewBrakingDecelerationWalking)
{
	BrakingDecelerationWalking = NewBrakingDecelerationWalking;
}

void FMovementData::SetFallingLateralFriction(float NewFallingLateralFriction)
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
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	bCanMove = true;
	CameraComp = FindComponentByClass<UCameraComponent>();
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
		PhysSlide(DeltaTime);
	}

	if(bIsDashing)
	{
		Dash();
	}

	if(bCanGrapple)
	{
		Grapple();
	}
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

void APlayerCharacter::MoveForward(float AxisValue)
{
	if(bCanMove)
		AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if(bCanMove)
		AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::Dash()
{
	CanDash = false;
	bIsDashing = true;
	float DashSpeed = (GetActorLocation() - DashDistance).Length()/DashTime;
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
	CharacterMovement->Velocity.Z = 0;
	DashDistance = CharacterMovement->Velocity.GetSafeNormal() * 20;
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
}

void APlayerCharacter::ExitSlide()
{
	bShouldSlide = false;
	bCanMove = true;
	MovementData.SetDefaultValues();
}

void APlayerCharacter::PhysSlide(float DeltaTime)
{
	CharacterMovement->AddForce(SlideSurfNormal);
	bCanMove = false;
}

void APlayerCharacter::StopSlide()
{
	if(SlideSurfNormal.Equals(FVector::Zero()))
		ExitSlide();
}


void APlayerCharacter::CanGrapple()
{
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,0,0,5);
	GetWorld()->SweepSingleByChannel(GrappleHit,TraceStart,TraceEnd,FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(20),QueryParams);
	if(GrappleHit.Component != nullptr && GrappleHit.Component->ComponentHasTag("GrapplePoint"))
		bCanGrapple = true;
}

void APlayerCharacter::StopGrapple()
{
	bCanGrapple = false;
	CharacterMovement->SetMovementMode(MOVE_Walking);
}

void APlayerCharacter::Grapple()
{
	CharacterMovement->Velocity = FVector::Zero();
	CharacterMovement->SetMovementMode(MOVE_Flying);
	UE_LOG(LogTemp,Warning,TEXT("HIT"));
	
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.ImpactPoint, GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.Location).Length() < 50)
		StopGrapple();
}

FVector APlayerCharacter::GetSlideSurface(FVector FloorNormal)
{
	if(FloorNormal.Equals(GetActorUpVector()))
		return FVector::Zero();
	
		FVector crossVect = FVector::CrossProduct(FloorNormal,GetActorUpVector());
	    FVector CrossCrossVect = FVector::CrossProduct(FloorNormal,crossVect.GetSafeNormal());
		float Direction = 1 - FVector::DotProduct(FloorNormal,GetActorUpVector());
		FVector FloorInfluence = (FMath::Clamp(Direction,0,1) * SlideVelocity) *  CrossCrossVect.GetSafeNormal();
		return FloorInfluence;
}




