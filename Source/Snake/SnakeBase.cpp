// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsDirectionJustChanged = false;
	GrowSize = 0;
	NeedToTeleportHead = false;
	ElementSize = 100.f;
	Speed = 10.f;
	LastMovementDirection = EDirection::Up;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(Speed);
	AddSnakeElement(3);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(const int Count)
{
	for (int i = 0; i < Count; i++)
	{
		const int Length = SnakeElements.Num();
		const bool IsHead = Length == 0;
		
		FVector LastLocation = GetActorLocation();
		FTransform NewTransform(LastLocation);

		if (!IsHead)
		{
			FVector ShiftVector(0, 0, 0);
			LastLocation = SnakeElements[Length - 1]->GetTransform().GetLocation();
			switch (LastMovementDirection)
			{
			case EDirection::Up:    ShiftVector = FVector(1, 0, 0);  break;
			case EDirection::Down:  ShiftVector = FVector(-1, 0, 0); break;
			case EDirection::Left:  ShiftVector = FVector(0, 1, 0);  break;
			case EDirection::Right: ShiftVector = FVector(0, -1, 0); break;
			}
			ShiftVector *= ElementSize;
			NewTransform = FTransform(LastLocation - ShiftVector);
		}
		
		auto SnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		SnakeElement->SnakeOwner = this;
		// FString str = FString::FromInt(NewTransform.GetLocation().X);
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, str);    
		SnakeElements.Add(SnakeElement);

		if (IsHead)
		{
			SnakeElement->SetHeadElementType();
		}
	}
}

void ASnakeBase::ChangeSnakeDirection(const EDirection NewDirection)
{
	if (!IsDirectionJustChanged)
	{
		LastMovementDirection = NewDirection;
		IsDirectionJustChanged = true;
	}
}

void ASnakeBase::Move()
{
	SnakeElements[0]->ToggleCollision();
	for(int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		const auto Element = SnakeElements[i];
		const auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		FVector CurrentLocation = Element->GetActorLocation();
		Element->SetActorLocation(PrevLocation);
		
		if (i == SnakeElements.Num() - 1 && GrowSize > 0)
		{
			auto SnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, FTransform(CurrentLocation));
			SnakeElement->SnakeOwner = this;
			SnakeElements.Add(SnakeElement);
			GrowSize--;
		}
	}

	if (NeedToTeleportHead)
	{
		SnakeElements[0]->SetActorLocation(TeleportLocation);
		NeedToTeleportHead = false;
	}
	else
	{
		const float MovementDelta = ElementSize;
		FVector MovementVector(0, 0, 0);

		switch (LastMovementDirection)
		{
		case EDirection::Up:
			MovementVector.X += MovementDelta;
			break;
		case EDirection::Down: 
			MovementVector.X -= MovementDelta;
			break;
		case EDirection::Left: 
			MovementVector.Y += MovementDelta;
			break;
		case EDirection::Right: 
			MovementVector.Y -= MovementDelta;
			break;
		}
		SnakeElements[0]->AddActorWorldOffset(MovementVector);
	}
	SnakeElements[0]->ToggleCollision();
	IsDirectionJustChanged = false;
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Actor)
{
	if(IsValid(OverlappedElement))
	{
		bool IsHead = OverlappedElement == SnakeElements[0];
		IInteractable* Interactable = Cast<IInteractable>(Actor);
		if (Interactable != nullptr)
		{
			Interactable->Interact(this);
		}
	}
}

void ASnakeBase::Grow()
{
	GrowSize++;
}

void ASnakeBase::TeleportHead(const FVector &NewLocation)
{
	NeedToTeleportHead = true;
	TeleportLocation = NewLocation;
}

void ASnakeBase::IncreaseSpeed(const float XFactor)
{
	Speed /= XFactor;
	SetActorTickInterval(Speed);
}

