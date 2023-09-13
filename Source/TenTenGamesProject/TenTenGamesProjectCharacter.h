// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "TenTenGamesProjectCharacter.generated.h"


UCLASS(config=Game)
class ATenTenGamesProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	

public:
	ATenTenGamesProjectCharacter();
	//General Player Variables for editing by a designer

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAttributes)
		float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAttributes)
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAttributes)
		float CurrentMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAttributes)
		float MaxMana = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAttributes)
		float ManaRechargeRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetingData)
		AActor* LockedTarget;
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	
	
	//GeneralPlayerVariables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float AttackCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
		float MissileCooldown;

	//Actor to Spawn (currently used as the missile)
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> ActorToSpawn;

	// Targeting Data Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetingData)
		FTransform SpawnTransform;

	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetingData)
		bool bIsLockedOn;



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//UFUNCTION(BlueprintImplementableEvent)
		//void Lock();

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void RechargeMana();

	UFUNCTION(BlueprintCallable)
		void SpawnProjectile();

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TargetingData)
		bool bMissileSpawned;

};

