// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBonus.h"
#include "SnakeBase.h"

// Sets default values
ASpeedBonus::ASpeedBonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	XFactor = FMath::RandRange(0.5, 2.f);
}

// Called when the game starts or when spawned
void ASpeedBonus::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpeedBonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpeedBonus::Interact(AActor* Actor)
{
	const auto Snake = Cast<ASnakeBase>(Actor);
	if (IsValid(Snake))
	{
		Snake->IncreaseSpeed(XFactor);
		this->Destroy();
	}
}

