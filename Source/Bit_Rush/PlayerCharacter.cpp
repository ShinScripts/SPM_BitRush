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

	bCanMove = true;
	CharacterMovement->BrakingFrictionFactor = 0;
	CharacterMovement->GravityScale = 1;
	CharacterMovement->FallingLateralFriction = 10;
	CharacterMovement->AirControl = 20;
	CharacterMovement->GroundFriction = 40;
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
		Dash();

	

	if(bIsGrappling)
		Grapple();
<<<<<<< Updated upstream
	}
=======
	else
		ScanGrapple();

	//Reduces players time left
	CurrentTime -= GetWorld()->GetDeltaSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("Time %f"), CurrentTime);
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
	PlayerInputComponent->BindAction(TEXT("Grapple"),EInputEvent::IE_Pressed,this,&APlayerCharacter::CanGrapple);
=======
	PlayerInputComponent->BindAction(TEXT("Grapple"),EInputEvent::IE_Pressed,this,&APlayerCharacter::StartGrapple);
	PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Pressed,this,&APlayerCharacter::DeflectON);
	PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Released,this,&APlayerCharacter::DeflectOFF);
	//PlayerInputComponent->BindAction(TEXT("Shoot"),EInputEvent::IE_Pressed,this,&APlayerCharacter::Shoot);
>>>>>>> Stashed changes
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
	CharacterMovement->GroundFriction = 0;
	CharacterMovement->BrakingDecelerationWalking = 1000;
	CharacterMovement->FallingLateralFriction = 0;
}

void APlayerCharacter::ExitSlide()
{
	bShouldSlide = false;
	bCanMove = true;
	CharacterMovement->GroundFriction = 20;
	CharacterMovement->BrakingDecelerationWalking = 2048;
	CharacterMovement->FallingLateralFriction = 10;
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


void APlayerCharacter::ScanGrapple()
{
<<<<<<< Updated upstream
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
=======
	/*if(bCanGrapple)
		return;
	
	const FVector TraceStart = CameraComp->GetComponentLocation();
	const FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
	
>>>>>>> Stashed changes
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,0,0,5);
	GetWorld()->SweepSingleByChannel(GrappleHit,TraceStart,TraceEnd,FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(20),QueryParams);
	if(GrappleHit.Component != nullptr && GrappleHit.Component->ComponentHasTag("GrapplePoint"))
		bCanGrapple = true;
<<<<<<< Updated upstream
	//else
	//{
	//	bCanGrapple = false;
	//}

=======
	}
	*/

	if(bIsGrappling)
		return;
	
	const FVector TraceStart = CameraComp->GetComponentLocation();
	const FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
	
	TArray<FHitResult> OutHits;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false,0,0,5);
	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits,TraceStart,TraceEnd, FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(50),QueryParams);

	if(bIsHit)
	{
		for(auto& Hit : OutHits)
		{
			if(Hit.GetComponent()->ComponentHasTag("GrapplePoint"))
			{
				GrappleHit = Hit;
				UE_LOG(LogTemp,Warning,TEXT("%s"),*GrappleHit.GetActor()->GetName());
				bCanGrapple = true;
				break;
			}
			else
			{
				bCanGrapple = false;
				GrappleHit.Reset();
			}
		}
	}
}

void APlayerCharacter::StartGrapple()
{
	if(bCanGrapple)
	{
		CharacterMovement->Velocity = FVector::Zero();
		bIsGrappling = true;
	}
>>>>>>> Stashed changes
}

void APlayerCharacter::StopGrapple()
{
	bIsGrappling = false;
	bCanGrapple = false;
	GrappleHit.Reset();
	CharacterMovement->SetMovementMode(MOVE_Walking);
}

void APlayerCharacter::Grapple()
{
	CharacterMovement->Velocity = FVector::Zero();
	CharacterMovement->SetMovementMode(MOVE_Flying);
<<<<<<< Updated upstream
	UE_LOG(LogTemp,Warning,TEXT("HIT"));

	float GrapplingTime = GrappleHit.Distance/GrapplingSpeed;
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.ImpactPoint, GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.Location).Length() < 50)
=======
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.GetComponent()->GetComponentLocation(), GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.GetComponent()->GetComponentLocation()).Length() < 70)
>>>>>>> Stashed changes
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




