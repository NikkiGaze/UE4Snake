// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Portal.generated.h"

UCLASS()
class SNAKE_API APortal : public AActor, public IInteractable
{
	GENERATED_BODY()
	bool IsInUse;
	
public:	
	// Sets default values for this actor's properties
	APortal();
	
	UPROPERTY(EditAnywhere)
	APortal * Another = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AActor* Actor) override;
};
