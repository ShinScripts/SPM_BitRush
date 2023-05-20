// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRunMovementComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UWallRunMovementComponent::UWallRunMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWallRunMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UWallRunMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	// If is NOT falling
	if(!UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMovementComponent()->IsFalling())
	{
		OnRightSide = false;
		OnLeftSide = false;
		
		OffWall(PlayerCharacter, DeltaTime);
		return; 
	}
	
	const FVector StartTrace = PlayerCharacter->GetActorLocation();
	const FVector RightSideEndTrace = LineTraceLength * PlayerCharacter->GetActorRightVector() + PlayerCharacter->GetActorLocation();
	const FVector LeftSideEndTrace = LineTraceLength * (PlayerCharacter->GetActorRightVector() * -1) + PlayerCharacter->GetActorLocation();

	const FVector RightSideEndAngleTrace = LineTraceLength * (PlayerCharacter->GetActorRightVector() - PlayerCharacter->GetActorForwardVector()) + PlayerCharacter->GetActorLocation();
	const FVector LeftSideEndAngleTrace = LineTraceLength * (PlayerCharacter->GetActorRightVector() * -1.f - PlayerCharacter->GetActorForwardVector()) + PlayerCharacter->GetActorLocation();
	
	FHitResult HitResultRight;
	FHitResult HitResultLeft;
	FHitResult HitResultRightAngle;
	FHitResult HitResultLeftAngle;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);
	
	GetWorld()->LineTraceSingleByChannel(HitResultRight, StartTrace, RightSideEndTrace, ECC_GameTraceChannel1, Params);
	GetWorld()->LineTraceSingleByChannel(HitResultLeft, StartTrace, LeftSideEndTrace, ECC_GameTraceChannel1, Params);
	GetWorld()->LineTraceSingleByChannel(HitResultRightAngle, StartTrace, RightSideEndAngleTrace, ECC_GameTraceChannel1, Params);
	GetWorld()->LineTraceSingleByChannel(HitResultLeftAngle, StartTrace, LeftSideEndAngleTrace, ECC_GameTraceChannel1, Params);
	
	// Right side
	if(HitResultRight.bBlockingHit && ContainsTag(HitResultRight))
	{
		PlayerCharacter->bCanMove = false;
		OnRightSide = true;	

		if(IsLookingAtWallNormal(PlayerCharacter, HitResultRight))
		{
			OffWall(PlayerCharacter, DeltaTime);
			return;
		}
		
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultRight.Normal, FVector(0.f, 0.f, -1.f)));
			TiltCamera(PlayerCharacter->CameraComp, -RollDegrees, InterpolationSpeed, DeltaTime);
			return;
		}
	}
	else if (HitResultRightAngle.bBlockingHit && ContainsTag(HitResultRightAngle))
	{
		PlayerCharacter->bCanMove = false;
		OnRightSide = true;	

		if(IsLookingAtWallNormal(PlayerCharacter, HitResultRightAngle))
		{
			OffWall(PlayerCharacter, DeltaTime);
			return;
		}
		
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultRightAngle.Normal, FVector(0.f, 0.f, -1.f)));
			TiltCamera(PlayerCharacter->CameraComp, -RollDegrees, InterpolationSpeed, DeltaTime);
			return;
		}
	}
	else
	{
		if(!OnLeftSide)
			OffWall(PlayerCharacter, DeltaTime);
	}
	
	
	// Left side
	if(HitResultLeft.bBlockingHit && ContainsTag(HitResultLeft))
	{
		PlayerCharacter->bCanMove = false;
		OnLeftSide = true;

		if(IsLookingAtWallNormal(PlayerCharacter, HitResultLeft))
		{
			OffWall(PlayerCharacter, DeltaTime);
			return;
		}
		
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultLeft.Normal, FVector(0.f, 0.f, 1.f)));
			TiltCamera(PlayerCharacter->CameraComp, RollDegrees, InterpolationSpeed, DeltaTime);
		}
	}
	else if (HitResultLeftAngle.bBlockingHit && ContainsTag(HitResultLeftAngle))
	{
		PlayerCharacter->bCanMove = false;
		OnLeftSide = true;

		if(IsLookingAtWallNormal(PlayerCharacter, HitResultLeftAngle))
		{
			OffWall(PlayerCharacter, DeltaTime);
			return;
		}
		
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultLeftAngle.Normal, FVector(0.f, 0.f, 1.f)));
			TiltCamera(PlayerCharacter->CameraComp, RollDegrees, InterpolationSpeed, DeltaTime);
		}
	}
	else
	{
		if(!OnRightSide)
			OffWall(PlayerCharacter, DeltaTime);
	}
}

void UWallRunMovementComponent::SetWallRunVelocity(APlayerCharacter* PlayerCharacter, const FVector& CrossProduct)
{
	if(InitialVelocity == 0.f)
		InitialVelocity = PlayerCharacter->GetMovementComponent()->Velocity.Size();
	
	const FVector Velocity = (InitialVelocity > MaxVelocity ? MaxVelocity : InitialVelocity < MinVelocity ? MinVelocity : InitialVelocity) * CrossProduct;
	
	PlayerCharacter->LaunchCharacter(Velocity, true, true);
	PlayerCharacter->MovementData.SetGravityScale(0.f);
}

void UWallRunMovementComponent::TiltCamera(UCameraComponent* CameraComp, float Degrees, float InterpSpeed, float DeltaTime)
{
	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator ControlRotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();

	CameraComp->SetWorldRotation(FMath::RInterpTo(
		FRotator(ControlRotation.Pitch, ControlRotation.Yaw, CameraRotation.Roll),
		FRotator(ControlRotation.Pitch, ControlRotation.Yaw, Degrees),
		DeltaTime,
		InterpSpeed
	));
}

void UWallRunMovementComponent::OffWall(APlayerCharacter* PlayerCharacter, float DeltaTime)
{
	if(OnLeftSide || OnRightSide)
	{
		PlayerCharacter->bCanMove = true;
		PlayerCharacter->MovementData.SetDefaultValues();
		
		OnRightSide = false;
		OnLeftSide = false;
	}
	
	InitialVelocity = 0.f;
	TiltCamera(PlayerCharacter->CameraComp, 0.f, InterpolationSpeed, DeltaTime);
}

void UWallRunMovementComponent::JumpOffWall(APlayerCharacter* PlayerCharacter)
{
	IsJumpingOffWall = true;
	PlayerCharacter->bCanMove = true;

	PlayerCharacter->MovementData.FallingLateralFriction = 0.f;
	PlayerCharacter->MovementData.AirControl = 1.f;

	const float JumpForce = PlayerCharacter->MovementData.JumpForce;
	
	FVector Velocity = JumpForce * JumpOffWallVelocityMultiplier * (PlayerCharacter->GetActorRightVector() * (OnRightSide ? -1.f : 1.f));
	Velocity.Z = JumpForce;
	
	PlayerCharacter->LaunchCharacter(Velocity, false, true);
}

bool UWallRunMovementComponent::IsLookingAtWallNormal(const APlayerCharacter* PlayerCharacter, const FHitResult& HitResult)
{
	if(FVector::DotProduct(HitResult.Normal, PlayerCharacter->GetActorForwardVector()) > OffWallThreshold)
		return true;
	
	return false;
}

bool UWallRunMovementComponent::ContainsTag(FHitResult HitResult)
{
	return HitResult.Component->ComponentTags.Contains(WallRunTag);
}
