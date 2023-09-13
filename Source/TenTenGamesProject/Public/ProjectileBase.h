// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class TENTENGAMESPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Variable setup for establishing object movement in "Move To Target" function
	FVector Direction;
	float TotalDistance;
	float CurrentDistance;
	FVector StartLocation;
	float DespawnDistance;

public:

	virtual void Tick(float DeltaTime) override;


	void MoveToTarget(float DeltaTime);

};
