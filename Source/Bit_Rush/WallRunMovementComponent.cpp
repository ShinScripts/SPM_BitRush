// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRunMovementComponent.h"

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->bCanMove = true;
		
		// TODO: offwall function
		return;
	}

	const FVector RightSideStartTrace = PlayerCharacter->GetActorLocation();
	const FVector RightSideEndTrace = LineTraceLength * PlayerCharacter->GetActorRightVector() + PlayerCharacter->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(PlayerCharacter);

	GetWorld()->LineTraceSingleByChannel(HitResult, RightSideStartTrace, RightSideEndTrace, ECC_GameTraceChannel1, Params);
	DrawDebugLine(GetWorld(), RightSideStartTrace, RightSideEndTrace, FColor::Cyan, false, 2.f);


	if(HitResult.bBlockingHit && HitResult.GetComponent()->ComponentTags.Contains(WallRunTag))
	{
		PlayerCharacter->bCanMove = false;
		OnRightSide = true;

		if(!IsJumpingOffWall)
		{
			SetWallRunVelocity(HitResult.Normal);
			TiltCamera(PlayerCharacter->CameraComp, -RollDegrees, InterpolationSpeed);
		}
	}
}

