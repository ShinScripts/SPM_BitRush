// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlatformVInterpActorComponent.h"


// Sets default values for this component's properties
UMPlatformVInterpActorComponent::UMPlatformVInterpActorComponent()
{
 // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
 // off to improve performance if you don't need them.
 PrimaryComponentTick.bCanEverTick = true;
  //Parent = GetOwner();
 
 // ...
	
}

// Called when the game starts
void UMPlatformVInterpActorComponent::BeginPlay()
{
 Super::BeginPlay();

 // Do this:
	DefStartPos = GetPos();
	DefTargetPos = TargetActor->GetActorLocation();
	StartPosition = GetPos();
	TargetPosition = TargetActor->GetActorLocation();

	HideActor(TargetActor, ActorVisible, ActorPhysicsEnabled);
}

// Called every frame
void UMPlatformVInterpActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
 Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


 // Do this:
	//TargetPosition = TargetActor->GetActorLocation();
  MovePlatform(DeltaTime);
}



FVector UMPlatformVInterpActorComponent::GetPos() // A more convenient way to get this actors location;
{
 return GetOwner()->GetActorLocation();
}

void UMPlatformVInterpActorComponent::MovePlatform(float DeltaTime) // Moves object (ideally a platform) based on whether MovingXForward, MovingYForward and MovingZForward are true/false respectively.
{
  if(!TargetActor)
  {
    return;
  }
	if(!EaseIn)
	{
		if(ReciprocatingPlatform)
           {
         	ConstantReciprocatingMove();
          }
        	else
        	{
        		StartPosition = DefStartPos;
        		TargetPosition = DefTargetPos;
        	}
	}
	else
	{
		if(ReciprocatingPlatform)
		{
			EaseInReciprocatingMove();
		}
		else
		{
			StartPosition = DefStartPos;
			TargetPosition = DefTargetPos;
		}
	}
 
	GetOwner()->SetActorLocation(GetNewPos(DeltaTime));
}

FVector UMPlatformVInterpActorComponent::GetNewPos(float DeltaTime)
{
	FVector NewPos;

	if(EaseIn)
	{
		NewPos = FVector(FMath::VInterpTo(GetPos(), /*TargetActor->GetActorLocation()*/TargetPosition, DeltaTime, Speed)); 
	}
	else
	{
		NewPos = FVector(FMath::VInterpConstantTo(GetPos(), /*TargetActor->GetActorLocation()*/TargetPosition, DeltaTime, Speed*100)); // multiplied by a factor of 100 because UE uses cm as their main unit apparently so it was done to get speed in m/s.
	}

	return NewPos;
}

void UMPlatformVInterpActorComponent::ConstantReciprocatingMove()
{
	/*float MoveDistance = FVector::Dist(StartPosition, TargetPosition);
	 float DistanceMoved = FVector::Dist(StartPosition, GetPos());*/
	float DistanceToTarget = FVector::Dist(TargetPosition, GetPos());
	
	if(DistanceToTarget <= 0 && TargetPosition != DefStartPos)
	{
		StartPosition = DefTargetPos;
		TargetPosition = DefStartPos;
 		
		//Speed = -Speed;
		//TargetActor->GetOwner()->SetActorLocation(DefStartPos);
		//StartPosition = DefStartPos;
	}
	else if(DistanceToTarget <= 0 && TargetPosition != DefTargetPos)
	{
		StartPosition = DefStartPos;
		TargetPosition = DefTargetPos;
		//TargetActor->GetOwner()->SetActorLocation(DefTargetPos);
		//StartPosition = DefTargetPos;
	}
}

void UMPlatformVInterpActorComponent::EaseInReciprocatingMove()
{
	/*float MoveDistance = FVector::Dist(StartPosition, TargetPosition);
	 float DistanceMoved = FVector::Dist(StartPosition, GetPos());*/
	float DistanceToTarget = FVector::Dist(TargetPosition, GetPos());
	
	if(DistanceToTarget <= EaseInDistanceMargin && TargetPosition != DefStartPos)
	{
		StartPosition = DefTargetPos;
		TargetPosition = DefStartPos;
 		
		//Speed = -Speed;
		//TargetActor->GetOwner()->SetActorLocation(DefStartPos);
		//StartPosition = DefStartPos;
	}
	else if(DistanceToTarget <= EaseInDistanceMargin && TargetPosition != DefTargetPos)
	{
		StartPosition = DefStartPos;
		TargetPosition = DefTargetPos;
		//TargetActor->GetOwner()->SetActorLocation(DefTargetPos);
		//StartPosition = DefTargetPos;
	}
}

void UMPlatformVInterpActorComponent::HideActor(AActor* Actor ,bool Visible, bool PhysicsActive)
{
	if(!Actor)
	{
		return;
	}
	
	Actor->GetRootComponent()->SetVisibility(Visible);
	if(!PhysicsActive)
	{
		Actor->GetRootComponent()->DestroyPhysicsState();
	}
	
}



