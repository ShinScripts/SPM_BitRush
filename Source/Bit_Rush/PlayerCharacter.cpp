// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "AITypes.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CharacterMovement = GetCharacterMovement();
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CanMove = true;
	CharacterMovement->BrakingFrictionFactor = 2;
	CharacterMovement->GravityScale = 1;
	CharacterMovement->FallingLateralFriction = 10;
	CharacterMovement->AirControl = 20;
	CharacterMovement->GroundFriction = 20;
	CharacterMovement->BrakingDecelerationWalking = 2048;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FloorHit = CharacterMovement->CurrentFloor.HitResult;
	SlideSurfNormal = GetSlideSurface(FloorHit.Normal);
	
	if(ShouldSlide && !CharacterMovement->IsFalling())
	{
		PhysSlide();
	}

	//UE_LOG(LogTemp,Warning,TEXT("VELOCITY: %s"),*CharacterMovement->Velocity.ToString())
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
	PlayerInputComponent->BindAction(TEXT("Dash"),EInputEvent::IE_Pressed,this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Pressed,this, &APlayerCharacter::EnterSlide);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Released,this,&APlayerCharacter::ExitSlide);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if(CanMove)
		AddMovementInput(GetActorForwardVector() * AxisValue * CharacterSpeed );
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if(CanMove)
		AddMovementInput(GetActorRightVector() * AxisValue * CharacterSpeed);
}

void APlayerCharacter::Dash()
{
	if(!CanDash) return;
	
	CanMove = false;
	CharacterMovement->GravityScale = 0;
	
	if(GetCharacterMovement()->IsFalling())
	{
		CharacterMovement->FallingLateralFriction = 0;
		LaunchCharacter(FVector(GetVelocity().X,GetVelocity().Y,0).GetSafeNormal() * DashVelocity, false,true);
	}
	else
	{
		CharacterMovement->BrakingFrictionFactor = 0;
		LaunchCharacter(GetVelocity().GetSafeNormal() * DashVelocity, false,true);
	}
	
	CanDash = false;
	
	FTimerHandle TimeHandler;
	GetWorldTimerManager().SetTimer(TimeHandler,this,&APlayerCharacter::StopVelocity,DashLength,false);
	
}

void APlayerCharacter::StopVelocity()
{
	CharacterMovement->BrakingFrictionFactor = 2;
	CharacterMovement->GravityScale = 1;
	CharacterMovement->FallingLateralFriction = 10;
	CharacterMovement->Velocity.Set(0,0,0);
	CanMove = true;

	FTimerHandle TimeHandler;
	GetWorldTimerManager().SetTimer(TimeHandler,this, &APlayerCharacter::ResetDash,DashCooldown,false);
}

void APlayerCharacter::EnterSlide()
{
	ShouldSlide = true;
	CharacterMovement->GroundFriction = 0;
	CharacterMovement->BrakingDecelerationWalking = 1000;
}

void APlayerCharacter::ExitSlide()
{
	ShouldSlide = false;
	CanMove = true;
	CharacterMovement->GroundFriction = 20;
	CharacterMovement->BrakingDecelerationWalking = 2048;
}

void APlayerCharacter::PhysSlide()
{
	if(SlideSurfNormal.Equals(FVector::Zero()))
	{
		CharacterMovement->AddImpulse(FVector(GetVelocity().X,GetVelocity().Y,0).GetSafeNormal() *  5000);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&APlayerCharacter::StopSlide,0.5f);
	}
	else
	{
		CharacterMovement->AddForce(SlideSurfNormal * SlideVelocity);
	}
	CanMove = false;
}

void APlayerCharacter::StopSlide()
{
	if(!SlideSurfNormal.Equals(FVector::Zero()))
	{
		return;
	}
	else
	{
		ExitSlide();
	}
}

void APlayerCharacter::ResetDash()
{
	CanDash = true;
}

FVector APlayerCharacter::GetSlideSurface(FVector FloorNormal)
{
	if(FloorNormal.Equals(GetActorUpVector()))
	{
		return FVector::Zero();
	}
		FVector crossVect = FVector::CrossProduct(FloorNormal,GetActorUpVector());
	    FVector CrossCrossVect = FVector::CrossProduct(FloorNormal,crossVect.GetSafeNormal());
		float Direction = 1 - FVector::DotProduct(FloorNormal,GetActorUpVector());
		FVector FloorInfluence = (FMath::Clamp(Direction,0,1) * SlideVelocity) *  CrossCrossVect.GetSafeNormal();
		return FloorInfluence;
}




