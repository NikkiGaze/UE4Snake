// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"

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
	
	
	for(int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto Element = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		Element->SetActorLocation(PrevLocation);
	}
	
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
}

