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
	
	// Is user is NOT pressing W or is NOT falling
	if(!UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsInputKeyDown(EKeys::W) || !UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMovementComponent()->IsFalling())
	{
		OnRightSide = false;
		OnLeftSide = false;
		PlayerCharacter->bCanMove = true;
		
		OffWall(PlayerCharacter, DeltaTime);
		return; 
	}
	
	const FVector StartTrace = PlayerCharacter->GetActorLocation();
	const FVector RightSideEndTrace = LineTraceLength * PlayerCharacter->GetActorRightVector() + PlayerCharacter->GetActorLocation();
	const FVector LeftSideEndTrace = LineTraceLength * (PlayerCharacter->GetActorRightVector() * -1) + PlayerCharacter->GetActorLocation();
	
	FHitResult HitResultRight;
	FHitResult HitResultLeft;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerCharacter);
	
	GetWorld()->LineTraceSingleByChannel(HitResultRight, StartTrace, RightSideEndTrace, ECC_GameTraceChannel1, Params);
	GetWorld()->LineTraceSingleByChannel(HitResultLeft, StartTrace, LeftSideEndTrace, ECC_GameTraceChannel1, Params);
	DrawDebugLine(GetWorld(), StartTrace, RightSideEndTrace, FColor::Cyan, false, 5.f);
	DrawDebugLine(GetWorld(), StartTrace, LeftSideEndTrace, FColor::Cyan, false, 5.f);
	
	// Right side
	if(HitResultRight.bBlockingHit && HitResultRight.GetComponent()->ComponentTags.Contains(WallRunTag))
	{
		PlayerCharacter->bCanMove = false;
		OnRightSide = true;	
		
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultRight.Normal, FVector(0.f, 0.f, -1.f)));
			TiltCamera(PlayerCharacter->CameraComp, -RollDegrees, InterpolationSpeed, DeltaTime);
		}
	}
	else
	{
		OnRightSide = false;
		
		if(!OnLeftSide)
			OffWall(PlayerCharacter, DeltaTime);
	}
	
	// Left side
	if(HitResultLeft.bBlockingHit && HitResultLeft.GetComponent()->ComponentTags.Contains(WallRunTag))
	{
		PlayerCharacter->bCanMove = false;
		OnLeftSide = true;
	
		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(PlayerCharacter, FVector::CrossProduct(HitResultLeft.Normal, FVector(0.f, 0.f, 1.f)));
			TiltCamera(PlayerCharacter->CameraComp, RollDegrees, InterpolationSpeed, DeltaTime);
		}
	}
	else
	{
		OnLeftSide = false;
	
		if(!OnRightSide)
			OffWall(PlayerCharacter, DeltaTime);
	}
}

void UWallRunMovementComponent::SetWallRunVelocity(APlayerCharacter* PlayerCharacter, const FVector& CrossProduct)
{
	FVector Velocity = PlayerCharacter->GetMovementComponent()->GetMaxSpeed() * CrossProduct;
	Velocity.Z = 0;
	
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
	PlayerCharacter->MovementData.SetDefaultValues();
	
	TiltCamera(PlayerCharacter->CameraComp, 0.f, InterpolationSpeed, DeltaTime);

	if(!OnLeftSide && !OnRightSide)
		PlayerCharacter->bCanMove = true;
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
