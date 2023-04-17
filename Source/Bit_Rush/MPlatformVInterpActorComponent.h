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
  FVector GetPos();


private:
  //variables
  /*UPROPERTY(EditAnywhere)
  FVector TargetPosition;
  FVector StartPosition;*/
  
  AActor* Parent = nullptr;
  UPROPERTY(EditAnywhere)
  AActor* TargetActor = nullptr;


  UPROPERTY(EditAnywhere)
  float Speed;


  /*//UPROPERTY(EditAnywhere)
  float XVelocity;
  //UPROPERTY(EditAnywhere)
  float YVelocity;
  //UPROPERTY(EditAnywhere)
  float ZVelocity;

  UPROPERTY(EditAnywhere)
  bool MovingXForward;
  UPROPERTY(EditAnywhere)
  bool MovingYForward;
  UPROPERTY(EditAnywhere)
  bool MovingZForward;

  UPROPERTY(EditAnywhere)
  bool MovingXBackwards;
  UPROPERTY(EditAnywhere)
  bool MovingYBackwards;
  UPROPERTY(EditAnywhere)
  bool MovingZBackwards;*/

  //Methods
  void MovePlatform(float DeltaTime);
  /*void DefineVelocities();
  void DefineMovingForward();*/
};
