// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include <string>

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsulComp = Cast<UCapsuleComponent>(GetRootComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayerInputComponent->BindAction(TEXT("Dash"),EInputEvent::IE_Pressed,this,&APlayerCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Pressed,this,&APlayerCharacter::Slide);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Released, this, &APlayerCharacter::ResetSlide);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
		AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
		AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::Slide()
{
	
	if(FVector::DotProduct(GetVelocity(),GetActorForwardVector()) > 100)
	{
		UE_LOG(LogTemp,Warning,TEXT("RADIUS: %f HALFHEIGH: %f"),CapsulComp->GetScaledCapsuleRadius(), CapsulComp->GetScaledCapsuleHalfHeight());
		CapsulComp->SetCapsuleSize(CapsulComp->GetScaledCapsuleRadius()/2, CapsulComp->GetScaledCapsuleHalfHeight()/2);
		UE_LOG(LogTemp,Warning,TEXT("RADIUS: %f HALFHEIGH: %f"),CapsulComp->GetScaledCapsuleRadius(), CapsulComp->GetScaledCapsuleHalfHeight());
		LaunchCharacter(FVector(GetVelocity().X,GetVelocity().Y,0).GetSafeNormal() * SlideVelocity,false,true);
		HasSlided = true;
	}
}

void APlayerCharacter::Dash()
{
	LaunchCharacter(FVector(GetVelocity().X,GetVelocity().Y,0).GetSafeNormal() * DashVelocity,false,true);
}

void APlayerCharacter::ResetSlide()
{
	if(HasSlided)
	{
		CapsulComp->SetCapsuleSize(CapsulComp->GetScaledCapsuleRadius()*2, CapsulComp->GetScaledCapsuleHalfHeight()*2);
		HasSlided = false;
	}
	
}



