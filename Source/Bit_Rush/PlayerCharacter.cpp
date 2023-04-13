// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <string>

#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FloorHit = CharacterMovement->CurrentFloor.HitResult;
	if(FloorHit.Component != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),*FloorHit.Component->GetName());
		SlideSurfNormal = GetSlideSurface(FloorHit.Normal);
	
		if(ShouldSlide)
		{
			PhysSlide();
		}
	}

	UE_LOG(LogTemp,Warning,TEXT("VELOCITY: %s"),*CharacterMovement->Velocity.ToString())
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
		AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if(CanMove)
		AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::Dash()
{
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
}

void APlayerCharacter::EnterSlide()
{

	if(SlideSurfNormal.Equals(FVector::Zero()))
		ShouldLaunchSlide = true;
	Hit = FloorHit;
	ShouldSlide = true;
	CharacterMovement->GroundFriction = 0.5;
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
	CanMove = false;
	if(!CharacterMovement->IsFalling())
	{
		if(SlideSurfNormal.Equals(FVector::Zero()))
		{
			CharacterMovement->AddImpulse(GetActorForwardVector() * SlideVelocity);
			FTimerHandle TimeHandler;
			GetWorldTimerManager().SetTimer(TimeHandler,this,&APlayerCharacter::StopSlidingAfterSeconds,0.5f);
			//TEST
		}
		if(!SlideSurfNormal.Equals(FVector::Zero()))
		{
			CharacterMovement->AddImpulse(SlideSurfNormal.GetSafeNormal() * -SlideVelocity);
		}
			
	}

	UE_LOG(LogTemp,Warning,TEXT("SLIDE"));
}

void APlayerCharacter::StopSlidingAfterSeconds()
{
	UE_LOG(LogTemp,Warning,TEXT("STOPSTOPOST"));
	ShouldSlide = false;
	CanMove = true;
}

FVector APlayerCharacter::GetSlideSurface(FVector FloorNormal)
{
	if(FloorHit.Normal.Equals(GetActorUpVector()))
	{
		return FVector::Zero();
	}
		FVector crossVect = FVector::CrossProduct(FloorNormal,GetActorUpVector());
		return FVector::CrossProduct(crossVect,FloorNormal).GetSafeNormal();
}




