// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

class ASnakeElementBase;

UENUM()
enum class EDirection
{
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class SNAKE_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	bool IsDirectionJustChanged;
	int GrowSize;
	bool NeedToTeleportHead;
	FVector TeleportLocation;
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY()
	TArray<ASnakeElementBase *> SnakeElements;

	UPROPERTY(EditDefaultsOnly)
	float ElementSize;

	UPROPERTY(EditDefaultsOnly)
	float Speed;
	
	UPROPERTY()
	EDirection LastMovementDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddSnakeElement(const int Count = 1);
	void ChangeSnakeDirection(const EDirection NewDirection);
	
	UFUNCTION(BlueprintCallable)
	void Move();

	UFUNCTION()
	void SnakeElementOverlap(ASnakeElementBase * OverlappedElement, AActor* Actor);
	void Grow();
	void TeleportHead(const FVector& NewLocation);
	void IncreaseSpeed(const float XFactor);
};
