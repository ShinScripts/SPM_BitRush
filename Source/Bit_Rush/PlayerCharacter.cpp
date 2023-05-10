// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GrapplingFeedbackComponent.h"

//FMovementData
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

//FDashComponent

void FDashComponent::Initialize(APlayerCharacter* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}


void FDashComponent::Update(float DeltaTime)
{
	if(bIsDashing)
		Dash(DeltaTime);
	
	if(bDashIsOnCooldown)
	{
		DashCurrentCooldown-= DeltaTime;
	}
	
	if(DashCurrentCooldown <= 0.0f)
	{
		bDashIsOnCooldown = false;
		DashCurrentCooldown = DashCooldown;
	}
}

void FDashComponent::StartDash()
{
	if(bDashIsOnCooldown)
		return;

	bIsDashing = true;
	DashDistance = PlayerCharacter->GetActorRotation().RotateVector(DashDirection.GetSafeNormal()) * 20;
	DashDistance.Z = 0;
	CurrentDashTime = DashTime;
}

void FDashComponent::Dash(float DeltaTime)
{
	bIsDashing = true;
	
	const float DashSpeed = (PlayerCharacter->GetActorLocation() - DashDistance).Length()/DashTime;

	PlayerCharacter->SetActorLocation(FMath::VInterpConstantTo(PlayerCharacter->GetActorLocation(),DashDistance + PlayerCharacter->GetActorLocation(),PlayerCharacter->GetWorld()->DeltaTimeSeconds,DashSpeed));
	CurrentDashTime -= DeltaTime;
	UE_LOG(LogTemp,Warning,TEXT("%f"),CurrentDashTime);
	if(CurrentDashTime < 0)
	{
		bDashIsOnCooldown = true;
		bIsDashing = false;
	}
		
		
}

//APlayerCharacter
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
	
	MovementData.SetDefaultValues();
	DashComponent.Initialize(this);
	bCanMove = true;
	CameraComp = FindComponentByClass<UCameraComponent>();

	SetDeflectBoxVariable();

	CharacterHitBox = FindComponentByClass<UCapsuleComponent>();
	HitBoxDefaultValue = CharacterHitBox->GetScaledCapsuleHalfHeight();
	CrouchHitBoxValue = HitBoxDefaultValue/2;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	InvincibilityTimer -= DeltaTime;

	MovementData.SetCharacterMovement(CharacterMovement);
	DashComponent.Update(DeltaTime);
	
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
	
	/*if(bIsDashing)
	{
		Dash();
	}*/

	if(bIsGrappling)
	{
		Grapple();
	}
	else
	{
		ScanGrapple();
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
	PlayerInputComponent->BindAction(TEXT("Dash"),EInputEvent::IE_Pressed,this,&APlayerCharacter::ActionStartDash);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Pressed,this, &APlayerCharacter::EnterSlide);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Released,this,&APlayerCharacter::ExitSlide);
	PlayerInputComponent->BindAction(TEXT("Grapple"),EInputEvent::IE_Pressed,this,&APlayerCharacter::StartGrapple);
	PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Pressed,this,&APlayerCharacter::DeflectON);
	PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Released,this,&APlayerCharacter::DeflectOFF);
	//PlayerInputComponent->BindAction(TEXT("Shoot"),EInputEvent::IE_Pressed,this,&APlayerCharacter::Shoot);
}

void APlayerCharacter::DeflectON()
{
	if(DeflectorBox == nullptr)
	{
		return;
	}
	DeflectorBox->StartDeflect();
}
void APlayerCharacter::DeflectOFF()
{
	if(DeflectorBox == nullptr)
	{
		return;
	}
	DeflectorBox->StopDeflect();
}

void APlayerCharacter::MoveForward(const float AxisValue)
{
	if(bCanMove)
	{
		AddMovementInput(GetActorForwardVector() * AxisValue);
		DashComponent.DashDirection.X = AxisValue;
	}
}

void APlayerCharacter::MoveRight(const float AxisValue)
{
	if(bCanMove)
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
		DashComponent.DashDirection.Y = AxisValue;
	}
}

void APlayerCharacter::Jump()
{
	Super::Jump();
}

void APlayerCharacter::ActionStartDash()
{
	DashComponent.StartDash();
}

/*void APlayerCharacter::Dash()
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
*/

void APlayerCharacter::EnterSlide()
{
	bShouldSlide = true;
	MovementData.SetGroundFriction(0);
	MovementData.SetGroundFriction(0);
	MovementData.SetBrakingDecelerationWalking(1500);
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
	
    	MovementData.SetGroundFriction(0);
    	MovementData.SetBrakingDecelerationWalking(1500);
    	MovementData.SetFallingLateralFriction(0);
	
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

void APlayerCharacter::ScanGrapple()
{
	if(bIsGrappling)
		return;
	
	const FVector TraceStart = CameraComp->GetComponentLocation();
	const FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * GrapplingHookRange;
	
	TArray<FHitResult> OutHits;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	bool bIsHit = GetWorld()->SweepMultiByChannel(OutHits,TraceStart,TraceEnd, FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(70),QueryParams);
	UGrapplingFeedbackComponent* GrapplingFeedComp;
	if(bIsHit)
	{
		for(auto& Hit : OutHits)
		{
			if(Hit.GetComponent()->ComponentHasTag("GrapplePoint"))
			{
				GrappleHit = Hit;
				GrapplingFeedComp = Cast<UGrapplingFeedbackComponent>(GrappleHit.GetActor()->FindComponentByClass<UGrapplingFeedbackComponent>());
				GrapplingFeedComp->PlayerCanGrapple = true;
				
				UE_LOG(LogTemp,Warning,TEXT("%s"),*GrappleHit.GetActor()->GetName());
				bCanGrapple = true;
				//break;
			}
			else
			{
				if(GrappleHit.GetActor() != nullptr)
				{
					GrapplingFeedComp = Cast<UGrapplingFeedbackComponent>(GrappleHit.GetActor()->FindComponentByClass<UGrapplingFeedbackComponent>());
					GrapplingFeedComp->PlayerCanGrapple = false;
					GrappleHit.Reset();
					GrapplingFeedComp = nullptr;
				}
				bCanGrapple = false;
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
}

void APlayerCharacter::StopGrapple()
{
	bCanGrapple = false;
	bIsGrappling = false;
	GrappleHit.Reset();
	CharacterMovement->SetMovementMode(MOVE_Walking);
	LaunchCharacter(CameraComp->GetComponentRotation().Vector() * GrapplingLaunchSpeed,true,true);
}

void APlayerCharacter::Grapple()
{
	//FVector GrapplingDirection = GrappleHit.Location - GetActorLocation();
	CharacterMovement->SetMovementMode(MOVE_Flying);
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(),GrappleHit.GetComponent()->GetComponentLocation(), GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((GetActorLocation() - GrappleHit.GetComponent()->GetComponentLocation()).Length() < 70)
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
	if (InvincibilityTimer <= 0)
	{
	CurrentTime -= DamageAmount;
	InvincibilityTimer = 0.4;
	}
	return CurrentTime;
}

//Finding the DeflectorBoxComponent and setting a pointer to it
void APlayerCharacter::SetDeflectBoxVariable()
{
	TArray<USceneComponent*> ChildList;
	CameraComp->GetChildrenComponents(false, ChildList);
	for(USceneComponent* Child : ChildList)
	{
		UDeflectorBoxComponent* DefBox = Cast<UDeflectorBoxComponent>(Child);
		if(DefBox)
		{
			DeflectorBox = DefBox;
			//ScreenPrint("This loop object does match", FColor::Green);
		}
		else
		{
			//ScreenPrint("This loop object does not match", FColor::Red);
		}
	}
}

//Debug utility
void APlayerCharacter::ScreenPrint(FString Message)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT(""+Message));
	}
}

void APlayerCharacter::ScreenPrint(FString Message, FColor Color)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, Color, TEXT(""+Message));
	}
}

UDeflectorBoxComponent* APlayerCharacter::GetDeflectorBox()
{
	return DeflectorBox;
}

//ShootProjectile
/*void APlayerCharacter::Shoot()
{
}*/
