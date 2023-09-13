// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PrimitiveComponent.h"
#include "TenTenGamesProject/TenTenGamesProjectCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ProjectileBase.h"




// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	ATenTenGamesProjectCharacter* MissileOwner = Cast<ATenTenGamesProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); //Cast to the player character. (NOTE: This probably wouldn't work for multiplayer)
	if (MissileOwner != nullptr) //Only run this if the "MissileOner"(PlayerCharacter) is valid.
	{
		Target = MissileOwner->LockedTarget;		//Get the "LockedTarget" variable from the PlayerCharacter(MissileOwner) and set the TargetVariable as that value.
	};


		
	if (Target != nullptr) //only run this code if the target is valid
	{
		//start location will always be the location this actor spawns at
		StartLocation = GetActorLocation();

		Direction = Target->GetActorLocation() - StartLocation;		//Direction is == to the Target Actor location minus the Start location.

		TotalDistance = Direction.Size();							//Basically just the "size" of the Directional vector
		DespawnDistance = 10.0f;
		//Get Normal Direction
		Direction = Direction.GetSafeNormal();
		CurrentDistance = DespawnDistance * 1.5f;					// Current distance needs to be above Despawn distance, hence the x1.5.
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
											//for simplicity, the move to target function can be run on tick.
	MoveToTarget(DeltaTime);
}

void AProjectileBase::MoveToTarget(float DeltaTime)
{
	//only run this function if there's a valid target
	if (Target != nullptr)
	{
		//if the Current distance is less than the total distance, run the rest of the function
		if (CurrentDistance > DespawnDistance)
		{
			TotalDistance = Direction.Size();

			FVector TargetLocation = Target->GetActorLocation();		//As the target will be moving, the relevant locations will need updating each time the function is called
			FVector CurrentLocation = GetActorLocation();
			Direction = TargetLocation - CurrentLocation;
			Direction = Direction.GetSafeNormal(1);						// Set the SafeNormal as 1 to ensure the projectile doesn't slow down when approaching the target.


			CurrentLocation += Direction * Speed * DeltaTime;			//Get the current location of the actor, and add the Direction, Speed and DeltaTime to it. Then set the new location
			SetActorLocation(CurrentLocation);							// As this function will be called on tick, it will run regularly and update.
			CurrentDistance = (TargetLocation - CurrentLocation).Size();

			//If the CurrentDistance is greater than, or equal to the Total Distance, the target has been reached, and the projectile can be despawned.
		}

		else
		{
			
			Destroy();
		}
	}
}
