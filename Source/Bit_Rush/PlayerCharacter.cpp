// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "AITypes.h"
#include "Camera/CameraComponent.h"
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
	CameraComp = FindComponentByClass<UCameraComponent>();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FloorHit = CharacterMovement->CurrentFloor.HitResult;
	SlideSurfNormal = GetSlideSurface(FloorHit.Normal);
	
	if(bShouldSlide && !CharacterMovement->IsFalling())
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

	bIsDashing = true;

	//CharacterMovement->SetMovementMode(MOVE_Flying);

	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), DashDistance + GetActorLocation(),GetWorld()->DeltaTimeSeconds,DashSpeed));
	
	//CanDash = false;
	
	FTimerHandle TimeHandler;
	GetWorldTimerManager().SetTimer(TimeHandler,this,&APlayerCharacter::StopDash,0.1,false);
	
}

void APlayerCharacter::StopDash()
{
	//CharacterMovement->BrakingFrictionFactor = 2;
	//CharacterMovement->GravityScale = 1;
	//CharacterMovement->FallingLateralFriction = 10;
	//CanMove = true;
	//CanDash = true;
	CharacterMovement->SetMovementMode(MOVE_Walking);
	bIsDashing = false;
}

void APlayerCharacter::StartDash()
{
	bIsDashing = true;
	DashDistance = CharacterMovement->Velocity.GetSafeNormal() * 2000;
	DashDistance.Z = 0;
}

void APlayerCharacter::EnterSlide()
{
	bShouldSlide = true;
	CharacterMovement->GroundFriction = 0;
	CharacterMovement->BrakingDecelerationWalking = 1000;
	CharacterMovement->FallingLateralFriction = 0;
}

void APlayerCharacter::ExitSlide()
{
	bShouldSlide = false;
	CanMove = true;
	CharacterMovement->GroundFriction = 20;
	CharacterMovement->BrakingDecelerationWalking = 2048;
	CharacterMovement->FallingLateralFriction = 10;
	CharacterMovement->BrakingFrictionFactor = 2;
}

void APlayerCharacter::PhysSlide(float DeltaTime)
{
	//if(SlideSurfNormal.Equals(FVector::Zero()))
	//{
		//CharacterMovement->AddImpulse(FVector(GetVelocity().X,GetVelocity().Y,0).GetSafeNormal() *  5000);
		//FTimerHandle TimerHandle;
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&APlayerCharacter::StopSlide,0.5f);
	//}
	//else
	//{
		CharacterMovement->AddForce(SlideSurfNormal);
	//}
	CanMove = false;
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
	else
	{
		bCanGrapple = false;
	}

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

	float GrapplingTime = GrappleHit.Distance/GrapplingSpeed;

	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.ImpactPoint, GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.Location).Length() < 50)
		StopGrapple();
}

FVector APlayerCharacter::GetSlideSurface(FVector FloorNormal)
{
		FVector crossVect = FVector::CrossProduct(FloorNormal,GetActorUpVector());
	    FVector CrossCrossVect = FVector::CrossProduct(FloorNormal,crossVect.GetSafeNormal());
		float Direction = 1 - FVector::DotProduct(FloorNormal,GetActorUpVector());
		FVector FloorInfluence = (FMath::Clamp(Direction,0,1) * SlideVelocity) *  CrossCrossVect.GetSafeNormal();
		return FloorInfluence;
}




