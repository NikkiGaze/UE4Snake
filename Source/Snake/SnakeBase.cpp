// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	Speed = 10.f;
	LastMovementDirection = EDirection::Up;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(Speed);
	AddSnakeElement(4);
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
		FVector ShiftVector(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform = FTransform(GetActorLocation() - ShiftVector);
		auto SnakeElement = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		SnakeElement->SnakeOwner = this;
		int32 index = SnakeElements.Add(SnakeElement);

		if (index == 0)
		{
			SnakeElement->SetHeadElementType();
		}
	}
}

void ASnakeBase::Move()
{
	const float MovementDelta = ElementSize;
	FVector MovementVector;

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
	
	SnakeElements[0]->ToggleCollision();
	for(int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		const auto Element = SnakeElements[i];
		const auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		Element->SetActorLocation(PrevLocation);
	}
	
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
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

