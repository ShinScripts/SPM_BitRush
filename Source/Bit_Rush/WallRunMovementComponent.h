// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallRunMovementComponent.generated.h"


class UCameraComponent;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIT_RUSH_API UWallRunMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallRunMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SetWallRunVelocity(APlayerCharacter* PlayerCharacter, const FVector& CrossProduct);
	void TiltCamera(UCameraComponent* CameraComp, float Degrees, float InterpSpeed, float DeltaTime);
	void OffWall(APlayerCharacter* PlayerCharacter, float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void JumpOffWall(APlayerCharacter* PlayerCharacter);

	bool IsLookingAtWallNormal(APlayerCharacter* PlayerCharacter, const FHitResult& HitResult, const float DeltaTime);
	bool ContainsTag(FHitResult HitResult);
	
	UPROPERTY(EditAnywhere)
	float LineTraceLength = 110.f;

	UPROPERTY(EditAnywhere)
	float InterpolationSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float RollDegrees = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	float JumpOffWallVelocityMultiplier = 1.5f;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool OnRightSide = false;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool OnLeftSide = false;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool IsJumpingOffWall = false;

	float InitialVelocity = 0.f;

	UPROPERTY(EditAnywhere)
	float MinVelocity = 700.f;

	UPROPERTY(EditAnywhere)
	float MaxVelocity = 1500.f;

	UPROPERTY(EditAnywhere)
	float OffWallThreshold = 0.85f;

	UPROPERTY(EditAnywhere)
	FName WallRunTag = "WallRunnable";
};
