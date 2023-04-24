// Fill out your copyright notice in the Description page of Project Settings.
//¯\_(ツ)_/¯

#pragma once




#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

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
  
  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  bool EaseIn = false;
  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  bool ReciprocatingPlatform = true;

  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  float EaseInDistanceMargin = 1.5f;
  
  UPROPERTY(EditAnywhere, Category="MovingPlatform")
  AActor* TargetActor = nullptr;

  FVector TargetPosition;
  FVector StartPosition;

  FVector DefStartPos;
  FVector DefTargetPos;

  /*FVector DefaultTargetPosition;
  FVector DefaultStartPosition;*/
  

  UPROPERTY(EditAnywhere)
  float Speed;

  

  //Methods
  void MovePlatform(float DeltaTime);
  void ConstantReciprocatingMove();
  void EaseInReciprocatingMove();
  void HideActor(AActor* Actor, bool Visible, bool PhysicsActive);
  
  FVector GetNewPos(float DeltaTime);
  
};
