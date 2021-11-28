// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "SnakeBase.h"
#include "Engine/Classes/Components/InputComponent.h"

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation((FRotator(-90, 0, 0)));
	CreateSnakeActor();
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawnBase::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawnBase::HandlePlayerHorizontalInput);

}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawnBase::HandlePlayerVerticalInput(const float Value)
{
	if (Value == 0)
	{
		return;
	}
	
	if (IsValid(SnakeActor))
	{
		SetMovementDirection(Value > 0 ? EDirection::Up : EDirection::Down);
	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(const float Value)
{
	if (Value == 0)
	{
		return;
	}
	
	if (IsValid(SnakeActor))
	{
		SetMovementDirection(Value > 0 ? EDirection::Left : EDirection::Right);
	}
}

void APlayerPawnBase::SetMovementDirection(const EDirection Direction)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));    
	
	const EDirection PrevDirection = SnakeActor->LastMovementDirection;
	switch (Direction)
	{
	case EDirection::Up:    if (PrevDirection == EDirection::Down)  return; break;
	case EDirection::Down:  if (PrevDirection == EDirection::Up)    return; break;
	case EDirection::Left:  if (PrevDirection == EDirection::Right) return; break;
	case EDirection::Right: if (PrevDirection == EDirection::Left)  return; break;
	}

	SnakeActor->LastMovementDirection = Direction;
}

