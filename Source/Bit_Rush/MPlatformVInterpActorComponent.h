// Fill out your copyright notice in the Description page of Project Settings.
//¯\_(ツ)_/¯

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"

#include "MPlatformVInterpActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BIT_RUSH_API UMPlatformVInterpActorComponent : public UActorComponent
{
  GENERATED_BODY()




public:  
  // Sets default values for this component's properties
  UMPlatformVInterpActorComponent();




protected:
  // Called when the game starts
  virtual void BeginPlay() override;




public:  
  // Called every frame
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  //public Methods
  FVector GetPos();

private:
  //variables
  UPROPERTY(EditAnywhere, Category="TargetActor")
  bool ActorVisible = false;
  UPROPERTY(EditAnywhere, Category="TargetActor")
  bool ActorPhysicsEnabled = false;

  UPROPERTY(EditAnywhere, Category="MovingPlatform"/*, meta=(EditCondition = "!ReciprocatingPlatform")*/)
  bool ReturnToStartOnExitOverlap = false;
  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  bool MoveActorOnOverlap = false;

  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  bool EaseIn = false;
  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  bool ReciprocatingPlatform = true;
  bool OverlappingPlayer = false;

  UPROPERTY(EditAnywhere, Category="MovingPlatform", meta=(EditCondition = "EaseIn"))
  float EaseInDistanceMargin = 1.5f;
  
  UPROPERTY(EditAnywhere, Category="TargetActor")
  AActor* TargetActor = nullptr;

  UPROPERTY(VisibleAnywhere)
  UBoxComponent* BoxComponent;

  /*UPROPERTY(VisibleAnywhere)
  USceneComponent* Root;*/

  //APlayerCharacter* OverlappingPlayer;

  UPROPERTY(EditAnywhere, Category="Overlap")
  FVector BoxExtent = FVector(100,100,100);

  UPROPERTY(EditAnywhere, Category="Overlap")
  FVector BoxPosition = FVector(0,0,0);

  FVector TargetPosition;
  FVector StartPosition;

  FVector DefStartPos;
  FVector DefTargetPos;

  /*FVector DefaultTargetPosition;
  FVector DefaultStartPosition;*/
  

  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  float Speed;
  float DefSpeed;

  

  //Methods
  void MovePlatform(float DeltaTime, bool MoveOnOverlap, bool Overlapping, bool ReturnToStartOnExit);
  void AdjustTargetPos();
  void ReturnToStart(bool IsOverlapping);
  void ConstantReciprocatingMove();
  void EaseInReciprocatingMove();
  void HideActor(AActor* Actor, bool Visible, bool PhysicsActive);
  void SetBox();
  
  FVector GetNewPos(float DeltaTime);

  bool IsOverlappingPlayer();
};