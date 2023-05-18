// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlatformVInterpActorComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"


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
	//Root = GetOwner()->GetRootComponent();
	DefStartPos = GetPos();
	DefTargetPos = TargetActor->GetActorLocation();
	DefSpeed = Speed;
	StartPosition = GetPos();
	TargetPosition = TargetActor->GetActorLocation();

	HideActor(TargetActor, ActorVisible, ActorPhysicsEnabled);
	SetBox();
}

// Called every frame
void UMPlatformVInterpActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Do this:
	//CheckForPlayer();
	MovePlatform(DeltaTime, MoveActorOnOverlap, IsOverlappingPlayer(), ReturnToStartOnExitOverlap);
	
}

FVector UMPlatformVInterpActorComponent::GetPos() // A more convenient way to get this actors location;
{
 return GetOwner()->GetActorLocation();
}

void UMPlatformVInterpActorComponent::MovePlatform(float DeltaTime, bool MoveOnOverlap, bool Overlapping, bool ReturnToStartOnExit) // Moves object (ideally a platform) based on whether MovingXForward, MovingYForward and MovingZForward are true/false respectively.
{
	if(!MoveOnOverlap)
	{
		AdjustTargetPos();
	}
	else
	{
		if(Overlapping)
		{
			AdjustTargetPos();
			Speed = DefSpeed;
		}
		if(!Overlapping)
		{
			if(ReturnToStartOnExit)
			{
				ReturnToStart(Overlapping);
			}
			else
			{
				Speed = 0;
			}
		}
	}
	
	GetOwner()->SetActorLocation(GetNewPos(DeltaTime));
}

void UMPlatformVInterpActorComponent::ReturnToStart(bool IsOverlapping)
{
	float DistanceToDefStart = FVector::Dist(DefStartPos, GetPos());
	//float DistanceToDefTarget = FVector::Dist(DefTargetPos, GetPos());
	
	if(DistanceToDefStart <= 0)
	{
		if(!IsOverlapping)
		{
			Speed = 0;
			//TargetPosition = DefStartPos;
		}
		else
		{
			Speed = DefSpeed;
		}
	}
	else if(DistanceToDefStart > 0)
	{
		if(!IsOverlapping)
		{
			//Speed = DefSpeed;
			TargetPosition = DefStartPos;
		}
		else
		{
			//Speed = DefSpeed;
			TargetPosition = DefTargetPos;
		}
	}
}


void UMPlatformVInterpActorComponent::AdjustTargetPos()
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
	float DistanceToTarget = FVector::Dist(TargetPosition, GetPos());
	
	if(DistanceToTarget <= 0 && TargetPosition != DefStartPos)
	{
		StartPosition = DefTargetPos;
		TargetPosition = DefStartPos;
	}
	else if(DistanceToTarget <= 0 && TargetPosition != DefTargetPos)
	{
		StartPosition = DefStartPos;
		TargetPosition = DefTargetPos;
	}
}

void UMPlatformVInterpActorComponent::EaseInReciprocatingMove()
{
	float DistanceToTarget = FVector::Dist(TargetPosition, GetPos());
	
	if(DistanceToTarget <= EaseInDistanceMargin && TargetPosition != DefStartPos)
	{
		StartPosition = DefTargetPos;
		TargetPosition = DefStartPos;
	}
	else if(DistanceToTarget <= EaseInDistanceMargin && TargetPosition != DefTargetPos)
	{
		StartPosition = DefStartPos;
		TargetPosition = DefTargetPos;
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

void UMPlatformVInterpActorComponent::SetBox()
{
	BoxComponent = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("PlatformTrigger"));
	if(!BoxComponent)
	{
		return;
	}
	BoxComponent->RegisterComponent();
	BoxComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // Might need this line, might not.
	BoxComponent->InitBoxExtent(BoxExtent);
	BoxComponent->SetLineThickness(1);
	BoxComponent->SetRelativeLocation(BoxPosition);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetActiveFlag(true);
}

bool UMPlatformVInterpActorComponent::IsOverlappingPlayer()
{
	//UE_LOG(LogTemp, Warning, TEXT("is Root: %s"),*RootMesh->GetOwner()->GetActorNameOrLabel());
	TArray<AActor*> Actors;
	BoxComponent->GetOverlappingActors(Actors);
	bool ReturnValue = false;
	
	for(AActor* Actor : Actors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("is Overlapping: %s"),*Actor->GetActorNameOrLabel());
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);

		if(!Player)
		{
			ReturnValue = false;
		}
		else
		{
			TArray<UPrimitiveComponent*> OverlapComponents;
			BoxComponent->GetOverlappingComponents(OverlapComponents);

			for(UPrimitiveComponent* OverlapComp : OverlapComponents)
			{
				UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OverlapComp);
				if(!Capsule)
				{
					ReturnValue = false;
				}
				else
				{
					ReturnValue = true;
				}
			}
		}
	}
	return ReturnValue;
}