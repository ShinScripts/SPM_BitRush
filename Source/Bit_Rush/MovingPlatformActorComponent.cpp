// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatformActorComponent.h"
#include "Math/UnitConversion.h"

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
  //DefineMovingForward();
  StartPosition = GetOwner()->GetActorLocation();
  DefineVelocities();
}

// Called every frame
void UMovingPlatformActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
 Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


 // Do this:
  DefineMovingForward();
  MovePlatform(DeltaTime);
}



FVector UMovingPlatformActorComponent::GetPos() // A more convenient way to get this actors location;
{
 return GetOwner()->GetActorLocation();
}

void UMovingPlatformActorComponent::MovePlatform(float DeltaTime) // Moves object (ideally a platform) based on whether MovingXForward, MovingYForward and MovingZForward are true/false respectively.
{
  //DefineVelocities();
	DefineMovingForward();
  FVector CurPos = GetPos();
  
	float NewX = 0;
	float NewY = 0;
	float NewZ = 0;
  
  	if(MovingXForward)
  	{
  	   NewX = CurPos.X + XVelocity*DeltaTime;
  	}
	else if(!MovingXForward)
	{
	  NewX = CurPos.X - XVelocity*DeltaTime;
	}

	if(MovingYForward)
  	{
      NewY = CurPos.Y + YVelocity*DeltaTime;
  	}
	else if(!MovingYForward)
	{
	  NewY = CurPos.Y - YVelocity*DeltaTime;
	}

	if(MovingZForward)
  	{
      NewZ = CurPos.Z + ZVelocity*DeltaTime;
  	}
	else if(!MovingZForward)
	{
	  NewZ = CurPos.Z - ZVelocity*DeltaTime;
	}
  

  FVector NewPos = FVector(NewX,NewY,NewZ);
 
	GetOwner()->SetActorLocation(NewPos);
}


void UMovingPlatformActorComponent::DefineVelocities() // Calculates velocities for XYZ (X-/Y-/ZVelocity) axis based on input forward vector speed (Speed), distance and estimated time of arrival (ETA) using basic physics and trigonoemtry formulas.
{
  //FVector CurPos = GetPos();
  
 FVector DistanceVector = TargetPosition-StartPosition;
 float Distance = sqrt((DistanceVector.X*DistanceVector.X) + (DistanceVector.Y*DistanceVector.Y) + (DistanceVector.Z*DistanceVector.Z));
 float ETA = Distance/Speed;
  
 XVelocity = /*100*Speed*(DistanceVector.X/Distance);*/100*(DistanceVector.X/ETA); // Since UE uses cm as its unit all velocities were * 100 to convert to m/s
 YVelocity = /*100*Speed*(DistanceVector.Y/Distance);*/100*(DistanceVector.Y/ETA);
 ZVelocity = /*100*Speed*(DistanceVector.Z/Distance);*/100*(DistanceVector.Z/ETA);

 if(TargetPosition.X < StartPosition.X) // In case of StartPosition being < TargetPosition, Velocities for all axis are inverted since it would otherwise not work with the MovePlatform() function causing the object to fly of in the opposite directions and not reaching its target destination.
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

void UMovingPlatformActorComponent::DefineMovingForward() // Sets bools MovingXForward, MovingYForward, MovingZForward to true/false when reaching TargetPosition/StartPosition for each respective axis when StartPosition.XYZ < TargetPosition.XYZ and inverse in the opposite case to  
{
  FVector CurPos = GetPos();
//-------------------------------------------------------
//X
  if(TargetPosition.X > StartPosition.X)
  {
    //UE_LOG(LogTemp, Warning, TEXT("these"));
    if(CurPos.X >= TargetPosition.X)
    {
      MovingXForward = false;
    }
    else if(CurPos.X < StartPosition.X)
    {
      MovingXForward = true;
    }
  }
  else if(TargetPosition.X < StartPosition.X)
  {
    //UE_LOG(LogTemp, Error, TEXT("NUTS"));
    if(CurPos.X <= TargetPosition.X)
    {
      MovingXForward = true;
    }
    else if(CurPos.X > StartPosition.X)
    {
      MovingXForward = false;
    }
    //UE_LOG(LogTemp, Error, TEXT("bool is %hs and velocity is %f"),MovingXForward?"true":"false", XVelocity);
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
      MovingZForward = false;
    }
    else if(CurPos.Z < StartPosition.Z)
    {
      MovingZForward = true;
    }
  }
  else if(TargetPosition.Z < StartPosition.Z)
  {
    //UE_LOG(LogTemp, Error, TEXT("NUTS"));
    if(CurPos.Z <= TargetPosition.Z)
    {
      MovingZForward = true;
    }
    else if(CurPos.Z > StartPosition.Z)
    {
      MovingZForward = false;
    }
    //UE_LOG(LogTemp, Error, TEXT("bool is %hs and velocity is %f"),MovingZForward?"true":"false", ZVelocity);
  }
//-----------------------------------------------------------
}