// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformActorComponent.h"




// Sets default values for this component's properties
UMovingPlatformActorComponent::UMovingPlatformActorComponent()
{
 // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
 // off to improve performance if you don't need them.
 PrimaryComponentTick.bCanEverTick = true;




 // ...
}








// Called when the game starts
void UMovingPlatformActorComponent::BeginPlay()
{
 Super::BeginPlay();




 // Do this:
  //MovingForward = true;
  DefineMovingForward();
  StartPosition = GetOwner()->GetActorLocation();
  DefineVelocities();
}








// Called every frame
void UMovingPlatformActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
 Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


 // Do this:
  //DefineVelocities();
  DefineMovingForward();
  MovePlatform(/*TargetPosition*/);
}




FVector UMovingPlatformActorComponent::GetPos()
{
 return GetOwner()->GetActorLocation();
}




void UMovingPlatformActorComponent::MovePlatform(/*FVector TargetPos, float Speed*/)
{
	DefineMovingForward();
  FVector CurPos = GetPos();
  
	float NewX = 0;
	float NewY = 0;
	float NewZ = 0;
  
  	if(MovingXForward)
  	{
  	   NewX = CurPos.X + XVelocity;
  	}
	else if(!MovingXForward)
	{
	  NewX = CurPos.X - XVelocity;
	}

	if(MovingYForward)
  	{
      NewY = CurPos.Y + YVelocity;
  	}
	else if(!MovingYForward)
	{
	  NewY = CurPos.Y - YVelocity;
	}

	if(MovingZForward)
  	{
      NewZ = CurPos.Z + ZVelocity;
  	}
	else if(!MovingZForward)
	{
	  NewZ = CurPos.Z - ZVelocity;
	}

  /*if(CurPos.X > TargetPos.X)
 {
   NewX = CurPos.X - XVelocity;
 }
 else if(CurPos.X < TargetPos.X)
 {
   NewX = CurPos.X + XVelocity;
 }


 if(CurPos.Y > TargetPos.Y)
 {
   NewY = CurPos.Y - YVelocity;
 }
 else if(CurPos.Y < TargetPos.Y)
 {
   NewY = CurPos.Y + YVelocity;
 }


 if(CurPos.Z > TargetPos.Z)
 {
   NewZ = CurPos.Z - ZVelocity;
 }
 else if
 (CurPos.Z < TargetPos.Z)
 {
   NewZ = CurPos.Z + ZVelocity;
 }*/


  FVector NewPos = FVector(NewX,NewY,NewZ);
 
	GetOwner()->SetActorLocation(NewPos);
}


void UMovingPlatformActorComponent::DefineVelocities()
{
  //FVector CurPos = GetPos();
  
 FVector DistanceVector = TargetPosition-StartPosition;
 float Distance = sqrt((DistanceVector.X*DistanceVector.X) + (DistanceVector.Y*DistanceVector.Y) + (DistanceVector.Z*DistanceVector.Z));
 float ETA = Distance/Speed;
  
 XVelocity = DistanceVector.X/ETA;
 YVelocity = DistanceVector.Y/ETA;
 ZVelocity = DistanceVector.Z/ETA;

 if(TargetPosition.X < StartPosition.X)
 {
   XVelocity *= -1;
 }
 if(TargetPosition.Y < StartPosition.Y)
 {
   YVelocity *= -1;
 }
 if(TargetPosition.Z < StartPosition.Z)
 {
   ZVelocity *= -1;
 }
}

void UMovingPlatformActorComponent::DefineMovingForward()
{
  FVector CurPos = GetPos();
//-------------------------------------------------------
//X
  if(TargetPosition.X > StartPosition.X)
  {
    //UE_LOG(LogTemp, Warning, TEXT("these"));
    if(CurPos.X >= TargetPosition.X)
    {
      MovingYForward = false;
    }
    else if(CurPos.X < StartPosition.X)
    {
      MovingYForward = true;
    }
  }
  else if(TargetPosition.X < StartPosition.X)
  {
    //UE_LOG(LogTemp, Error, TEXT("NUTS"));
    if(CurPos.X <= TargetPosition.X)
    {
      MovingYForward = true;
    }
    else if(CurPos.X > StartPosition.X)
    {
      MovingYForward = false;
    }
    //UE_LOG(LogTemp, Error, TEXT("bool is %hs and velocity is %f"),MovingYForward?"true":"false", YVelocity);
  }
//--------------------------------------------------
//Y
//UE_LOG(LogTemp, Warning, TEXT("TPY:%f SPY:%f"),*TargetPosition.Y,*StartPosition.Y);
  if(TargetPosition.Y > StartPosition.Y)
  {
    //UE_LOG(LogTemp, Warning, TEXT("these"));
	  if(CurPos.Y >= TargetPosition.Y)
    {
      MovingYForward = false;
    }
    else if(CurPos.Y < StartPosition.Y)
    {
      MovingYForward = true;
    }
  }
  else if(TargetPosition.Y < StartPosition.Y)
  {
    //UE_LOG(LogTemp, Error, TEXT("NUTS"));
	  if(CurPos.Y <= TargetPosition.Y)
    {
      MovingYForward = true;
    }
      else if(CurPos.Y > StartPosition.Y)
    {
      MovingYForward = false;
    }
    //UE_LOG(LogTemp, Error, TEXT("bool is %hs and velocity is %f"),MovingYForward?"true":"false", YVelocity);
  }
//-------------------------------------------------------
//Z
  if(TargetPosition.Z > StartPosition.Z)
  {
    //UE_LOG(LogTemp, Warning, TEXT("these"));
    if(CurPos.Z >= TargetPosition.Z)
    {
      MovingYForward = false;
    }
    else if(CurPos.Z < StartPosition.Z)
    {
      MovingYForward = true;
    }
  }
  else if(TargetPosition.Z < StartPosition.Z)
  {
    //UE_LOG(LogTemp, Error, TEXT("NUTS"));
    if(CurPos.Z <= TargetPosition.Z)
    {
      MovingYForward = true;
    }
    else if(CurPos.Z > StartPosition.Z)
    {
      MovingYForward = false;
    }
    //UE_LOG(LogTemp, Error, TEXT("bool is %hs and velocity is %f"),MovingYForward?"true":"false", YVelocity);
  }
//-----------------------------------------------------------
}