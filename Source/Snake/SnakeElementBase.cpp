// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeElementBase.h"
#include "SnakeBase.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"


// Sets default values
ASnakeElementBase::ASnakeElementBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ASnakeElementBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakeElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeElementBase::SetHeadElementType_Implementation()
{
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeElementBase::HandleBeginOverlap);
}

void ASnakeElementBase::Interact(AActor* Actor)
{
	const auto Snake = Cast<ASnakeBase>(Actor);
	if (IsValid(Snake))
	{
		Snake->Destroy();
	}
}

void ASnakeElementBase::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									       AActor* OtherActor,
									       UPrimitiveComponent* OtherComponent,
									       int32, bool, const FHitResult&)
{
	if(IsValid(SnakeOwner))
	{
		SnakeOwner->SnakeElementOverlap(this, OtherActor);
	}
}

void ASnakeElementBase::ToggleCollision()
{
	if (StaticMeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

