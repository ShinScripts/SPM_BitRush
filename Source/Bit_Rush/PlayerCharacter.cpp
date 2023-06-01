// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
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
	JumpForce = 650;
	GravityScale = 2;
	BrakingFrictionFactor = 0.05;
	FallingLateralFriction = 8;
	AirControl = 0.4;
	GroundFriction = 10;
	BrakingDecelerationWalking = 5000;
	MaxAcceleration = 2500;
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

void FDashComponent::DashUpdate(float DeltaTime)
{
	if(bIsDashing)
		Dash(DeltaTime);
	
	if(bDashIsOnCooldown)
	{
		DashCurrentCooldown-= DeltaTime;
	}
	
	if(DashCurrentCooldown < 0.0f)
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
	// UE_LOG(LogTemp,Warning,TEXT("%f"),CurrentDashTime);
	if(CurrentDashTime < 0)
	{
		bDashIsOnCooldown = true;
		bIsDashing = false;
	}
}

///FGunComponent
void FGunComponent::Initialize(APlayerCharacter* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void FGunComponent::GunUpdate(float DeltaTime)
{
	if(bIsReloading)
		CurrentReloadTime -= PlayerCharacter->GetWorld()->GetDeltaSeconds();
	
	if(CurrentReloadTime < 0.0f)
	{
		CurrentReloadTime = ReloadTimer;
		bIsReloading = false;
		if(PlayerCharacter->UnlimitedAmmo == false)
		{
			if(PlayerCharacter->FillRestOnReload == false)
			{
				if(PlayerCharacter->StoredAmmo >= PlayerCharacter->AmmoMagCapacity)
				{
					PlayerCharacter->CurrentMagAmmo = PlayerCharacter->AmmoMagCapacity;
					PlayerCharacter->StoredAmmo -= PlayerCharacter->CurrentMagAmmo;
				}
				else if(PlayerCharacter->StoredAmmo < PlayerCharacter->AmmoMagCapacity)
				{
					PlayerCharacter->CurrentMagAmmo = PlayerCharacter->StoredAmmo;
					PlayerCharacter->StoredAmmo = 0;
				}
			}
			else
			{
				int AmmoRest = PlayerCharacter->CurrentMagAmmo;
				
				if(PlayerCharacter->StoredAmmo >= PlayerCharacter->AmmoMagCapacity)
				{
					if(PlayerCharacter->CurrentMagAmmo <= 0)
					{
						PlayerCharacter->CurrentMagAmmo = PlayerCharacter->AmmoMagCapacity;
						PlayerCharacter->StoredAmmo -= PlayerCharacter->CurrentMagAmmo;
					}
					else
					{
						PlayerCharacter->CurrentMagAmmo = PlayerCharacter->AmmoMagCapacity;
						PlayerCharacter->StoredAmmo -= ((PlayerCharacter->AmmoMagCapacity)-AmmoRest);
					}
					
				}
				else if(PlayerCharacter->StoredAmmo < PlayerCharacter->AmmoMagCapacity)
				{
					PlayerCharacter->CurrentMagAmmo += PlayerCharacter->StoredAmmo;
					PlayerCharacter->StoredAmmo -= AmmoRest;

					if(PlayerCharacter->StoredAmmo <= 0)
					{
						PlayerCharacter->StoredAmmo = 0;	
					}
				}
			}
		}
		else
		{
			if(PlayerCharacter->CurrentMagAmmo < PlayerCharacter->AmmoMagCapacity)
			{
				PlayerCharacter->CurrentMagAmmo = PlayerCharacter->AmmoMagCapacity;
			}
		}
	}
}


void FGunComponent::Reload()
{
	if(PlayerCharacter->CurrentMagAmmo < PlayerCharacter->AmmoMagCapacity)
	{
		bIsReloading = true;
	}
}

//FGrappleComponent

void FGrappleComponent::Initialize(APlayerCharacter* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void FGrappleComponent::GrappleUpdate()
{
	if(bIsGrappling)
	{
		Grapple();
	}
	else
	{
		ScanForGrapplePoint();
	}
}

void FGrappleComponent::ScanForGrapplePoint()
{
	if(bIsGrappling)
		return;
	
	const FVector TraceStart = PlayerCharacter->CameraComp->GetComponentLocation();
	const FVector TraceEnd = TraceStart + PlayerCharacter->CameraComp->GetForwardVector() * GrapplingHookRange;
	
	TArray<FHitResult> OutHits;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerCharacter);
	
	bool bIsHit = PlayerCharacter->GetWorld()->SweepMultiByChannel(OutHits,TraceStart,TraceEnd, FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeSphere(70),QueryParams);
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
				
				bCanGrapple = true;
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
	else if(GrappleHit.GetActor() != nullptr)
	{
		GrapplingFeedComp = Cast<UGrapplingFeedbackComponent>(GrappleHit.GetActor()->FindComponentByClass<UGrapplingFeedbackComponent>());
		GrapplingFeedComp->PlayerCanGrapple = false;
		GrappleHit.Reset();
		GrapplingFeedComp = nullptr;
		bCanGrapple = false;
	}
}

void FGrappleComponent::StartGrapple()
{
	if(bCanGrapple)
	{
		PlayerCharacter->CharacterMovement->Velocity = FVector::Zero();
		bIsGrappling = true;
	}
}

void FGrappleComponent::Grapple()
{
	PlayerCharacter->CharacterMovement->SetMovementMode(MOVE_Flying);
	PlayerCharacter->SetActorLocation(FMath::VInterpConstantTo(PlayerCharacter->GetActorLocation(),GrappleHit.GetComponent()->GetComponentLocation(), PlayerCharacter->GetWorld()->DeltaTimeSeconds, GrapplingSpeed));
	if((PlayerCharacter->GetActorLocation() - GrappleHit.GetComponent()->GetComponentLocation()).Length() < 70)
		StopGrapple();
}

void FGrappleComponent::StopGrapple()
{
	bCanGrapple = false;
	bIsGrappling = false;
	GrappleHit.Reset();
	PlayerCharacter->CharacterMovement->SetMovementMode(MOVE_Walking);
	PlayerCharacter->LaunchCharacter(PlayerCharacter->CameraComp->GetComponentRotation().Vector() * GrapplingLaunchSpeed,true,true);
	//RemoveNiagra
}

//FSlideComponent
void FSlideComponent::Initialize(FMovementData* InMovementData, APlayerCharacter* InPlayerCharacter)
{
	MovementData = InMovementData;
	PlayerCharacter = InPlayerCharacter;
}

void FSlideComponent::SlideBegin()
{
	HitBoxDefaultValue = PlayerCharacter->CharacterHitBox->GetScaledCapsuleHalfHeight();
	CrouchHitBoxValue = HitBoxDefaultValue/2;
}


void FSlideComponent::SlideUpdate(float DeltaTime)
{
	FloorHit = PlayerCharacter->CharacterMovement->CurrentFloor.HitResult;
	SlideSurfNormal = GetSlideSurface(FloorHit.Normal);
	
	if(bShouldSlide)
	{
		PlayerCharacter->CharacterHitBox->SetCapsuleHalfHeight(FMath::FInterpTo(PlayerCharacter->CharacterHitBox->GetScaledCapsuleHalfHeight(),CrouchHitBoxValue,DeltaTime,CrouchSpeed));
		Slide(DeltaTime);
	}
	else
	{
		PlayerCharacter->CharacterHitBox->SetCapsuleHalfHeight(FMath::FInterpTo(PlayerCharacter->CharacterHitBox->GetScaledCapsuleHalfHeight(),HitBoxDefaultValue,DeltaTime,CrouchSpeed));
	}
}

void FSlideComponent::EnterSlide()
{
	bShouldSlide = true;
	MovementData->SetGroundFriction(0);
	MovementData->SetGroundFriction(0);
	MovementData->SetBrakingDecelerationWalking(1500);
	MovementData->SetFallingLateralFriction(0);
	
	if(FloorHit.Normal.Equals(PlayerCharacter->GetActorUpVector()))
	{
		PlayerCharacter->CharacterMovement->AddImpulse(PlayerCharacter->GetVelocity().GetSafeNormal() * FlatSlideVelocity * PlayerCharacter->GetWorld()->DeltaRealTimeSeconds);
	}
}

void FSlideComponent::ExitSlide()
{
	bShouldSlide = false;
	PlayerCharacter->bCanMove = true;
	MovementData->SetDefaultValues();
}

void FSlideComponent::Slide(float DeltaTime)
{
	PlayerCharacter->bCanMove = false;
	
	MovementData->SetGroundFriction(0);

	if(SlideSurfNormal == FVector::Zero())
	{
		MovementData->SetBrakingDecelerationWalking(1000);
	}
	else
	{
		MovementData->SetBrakingDecelerationWalking(0);
	}
	MovementData->SetFallingLateralFriction(0);
	
	
	if(PlayerCharacter->CharacterMovement->Velocity.Length() > MaxSlideVelocity)
	{
		PlayerCharacter->CharacterMovement->Velocity.GetSafeNormal() *= MaxSlideVelocity * DeltaTime;
	}
	else
	{
		PlayerCharacter->CharacterMovement->AddForce(SlideSurfNormal * DeltaTime);
	}
}

void FSlideComponent::StopSlide()
{
	if(SlideSurfNormal.Equals(FVector::Zero()))
		ExitSlide();
}

FVector FSlideComponent::GetSlideSurface(const FVector &FloorNormal)
{
	if(FloorNormal.Equals(PlayerCharacter->GetActorUpVector()))
		return FVector::Zero();
	
	const FVector CrossVector = FVector::CrossProduct(FloorNormal,PlayerCharacter->GetActorUpVector());
	const FVector CrossCrossVector = FVector::CrossProduct(FloorNormal,CrossVector.GetSafeNormal());
	const float Direction = 1 - FVector::DotProduct(FloorNormal,PlayerCharacter->GetActorUpVector());
	const FVector FloorInfluence = FMath::Clamp(Direction,0,1) * SlideVelocity *  CrossCrossVector.GetSafeNormal();
	return FloorInfluence;
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

	CharacterHitBox = FindComponentByClass<UCapsuleComponent>();
	
	MovementData.SetDefaultValues();
	DashComponent.Initialize(this);
	GunComponent.Initialize(this);
	GrappleComponent.Initialize(this);
	SlideComponent.Initialize(&MovementData,this);
	SlideComponent.SlideBegin();
	
	bCanMove = true;
	CameraComp = FindComponentByClass<UCameraComponent>();

	SetDeflectBoxVariable();
	SetStartAmmo();

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InvincibilityTimer -= DeltaTime;
	MovementData.SetCharacterMovement(CharacterMovement);

	if(CharacterMovement->IsFalling())
	{
		CurrentCoyoteTime -= DeltaTime;
	}

	// UE_LOG(LogTemp,Warning,TEXT("%f"),CurrentCoyoteTime);
	if(!CharacterMovement->IsFalling())
	{
		CurrentCoyoteTime = CoyoteTime;
		bCanJump = true;
	}

	//Updates
	DashComponent.DashUpdate(DeltaTime);
	GunComponent.GunUpdate(DeltaTime);
	GrappleComponent.GrappleUpdate();
	SlideComponent.SlideUpdate(DeltaTime);
	
	//Reduces players time left
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("Time %f"), CurrentTime);
}
FVector APlayerCharacter::GetGrappleRotation() const
{

	return GrappleComponent.GrappleHit.GetActor()->GetActorLocation();
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GrappleComponent.GrappleNiagra, StartPosition, GrappleHookVector.Rotation(), FVector(GrappleHookVector.Size(), 1, 1 ));
	//UNiagaraFunctionLibrary::SpawnSystemAttached(this, GrappleComponent.GrappleNiagra, StartPosition, GrappleHookVector.Rotation(), FVector(GrappleHookVector.Size(), 1, 1));
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
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Pressed,this, &APlayerCharacter::ActionEnterSlide);
	PlayerInputComponent->BindAction(TEXT("Slide"),EInputEvent::IE_Released,this,&APlayerCharacter::ActionExitSlide);
	PlayerInputComponent->BindAction(TEXT("Grapple"),EInputEvent::IE_Pressed,this,&APlayerCharacter::ActionStartGrapple);
	PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Pressed,this,&APlayerCharacter::DeflectON);
	//PlayerInputComponent->BindAction(TEXT("Deflect"),EInputEvent::IE_Released,this,&APlayerCharacter::DeflectOFF);
	PlayerInputComponent->BindAction(TEXT("Reload"),EInputEvent::IE_Pressed,this,&APlayerCharacter::ActionReload);
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
	
	if(CurrentCoyoteTime > 0.0f && bCanJump && CharacterMovement->IsFalling())
	{
		LaunchCharacter(GetActorUpVector().GetSafeNormal() * 700,false,true);
	}

	bCanJump = false;
}

void APlayerCharacter::ActionReload()
{
	GunComponent.Reload();
}

void APlayerCharacter::ActionStartDash()
{
	DashComponent.StartDash();
}

void APlayerCharacter::ActionStartGrapple()
{
	GrappleComponent.StartGrapple();
}

void APlayerCharacter::ActionEnterSlide()
{
	SlideComponent.EnterSlide();
}

void APlayerCharacter::ActionExitSlide()
{
	SlideComponent.ExitSlide();
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
	CurrentTime += DamageAmount;
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
		}
	}
}

UDeflectorBoxComponent* APlayerCharacter::GetDeflectorBox()
{
	return DeflectorBox;
}


void APlayerCharacter::ChangeTime(bool AddOrTake, float Tribute)
{
	if(AddOrTake)
	{
		CurrentTime += Tribute;
	}
	else
	{
		CurrentTime -= Tribute;
		if(CurrentTime <= 0)
		{
			CurrentTime = 0;
		}
	}
}

void APlayerCharacter::ChangeAmmo(bool AddOrTake, bool MagOrStore, int Tribute)
{
	if(AddOrTake)
	{
		AddAmmo(MagOrStore, Tribute);
	}
	else
	{
		SubtractAmmo(MagOrStore, Tribute);
	}
}

void APlayerCharacter::SetStartAmmo()
{
	if(!UnlimitedAmmo)
	{
		if(FullMagAtStart)
		{
			CurrentMagAmmo = AmmoMagCapacity;
		}
		else
		{
			CurrentMagAmmo = MagAmmoAtStart;
		}
        
		if(FullAmmoStoreAtStart)
		{
			StoredAmmo = MaxAmmo;
		}
		else
		{
			StoredAmmo = StoredAmmoAtStart;
		}
	}
	else
	{
		if(FullMagAtStart)
		{
			CurrentMagAmmo = AmmoMagCapacity;
		}
		else
		{
			CurrentMagAmmo = MagAmmoAtStart;
		}

		StoredAmmo = AmmoMagCapacity;
	}
}

void APlayerCharacter::SubtractAmmo(bool MagOrStore, int Tribute)
{
	if(!UnlimitedAmmo)
	{
		if(MagOrStore)
		{
			CurrentMagAmmo -= Tribute;
			if(CurrentMagAmmo <= 0)
			{
				CurrentMagAmmo = 0;
			}
		}
		else
		{
			StoredAmmo -= Tribute;
			if(StoredAmmo <= 0)
			{
				StoredAmmo = 0;
			}
		}
	}
	else
	{
		if(MagOrStore)
		{
			CurrentMagAmmo -= Tribute;
			if(CurrentMagAmmo <= 0)
				{
					CurrentMagAmmo = 0;
				}
		}
	}
}

void APlayerCharacter::AddAmmo(bool MagOrStore, int Tribute)
{
	if(!UnlimitedAmmo)
	{
		if(MagOrStore)
		{
			CurrentMagAmmo += Tribute;
			if(CurrentMagAmmo >= AmmoMagCapacity)
			{
				CurrentMagAmmo = AmmoMagCapacity;
			}
		}
		else
		{
			StoredAmmo += Tribute;
			if(StoredAmmo >= MaxAmmo)
			{
				StoredAmmo = MaxAmmo;
			}
		}
	}
	else
	{
		CurrentMagAmmo += Tribute;
		if(CurrentMagAmmo >= AmmoMagCapacity)
		{
			CurrentMagAmmo = AmmoMagCapacity;
		}
	}
}

