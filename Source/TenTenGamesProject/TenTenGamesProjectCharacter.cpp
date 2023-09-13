// Copyright Epic Games, Inc. All Rights Reserved.

#include "TenTenGamesProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//includiing the "Weapon_Parent.h" class allows me access to the weapon parent class.

//////////////////////////////////////////////////////////////////////////
// ATenTenGamesProjectCharacter

ATenTenGamesProjectCharacter::ATenTenGamesProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATenTenGamesProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


}

//////////////////////////////////////////////////////////////////////////
// Input

void ATenTenGamesProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATenTenGamesProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATenTenGamesProjectCharacter::Look);

	}

}

void ATenTenGamesProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATenTenGamesProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


//void ATenTenGamesProjectCharacter::Lock()
//{

	// Tested running the linetrace via C++, realised that whilst technically it may be optimised better, for initial design & rapid prototyping
	// the system works better in BP. Would return later once figures/features are more fleshed out to convert.
	/*FHitResult OutHit;
	AActor* OutActor;
	FVector Start = GetActorLocation();
	Start.Z += 50.0f;
	Start.X += 200.0f;

	FVector ForwardVector = GetActorForwardVector(); //get the forward vector of the player character
	FVector End = ((ForwardVector * 500.0f) + Start); //whereever the start point is, go 500 units out from that, and that's the end point.
	FCollisionQueryParams CollisionParams;

	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 5);

	bool isHit = ActorLineTraceSingle(OutHit, Start, End, ECC_Pawn, CollisionParams);
	if (isHit)
	{
		OutActor = OutHit.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *OutActor->GetName());
	};
	}*/
//}


//simple attack function. Will only run if the player has enough Mana.
void ATenTenGamesProjectCharacter::Attack()
{
	if (bIsLockedOn == true && bMissileSpawned != true)
	{
		{
			/*if (AttackCost > CurrentMana) //For now I've commented this out and used a UMG widget in BP instead as it was quicker. Long term I'd use this to replace the BP system
			{
				//GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Not Enough Mana")); 
			};*/
			if (AttackCost <= CurrentMana && LockedTarget != nullptr)
			{
				SpawnProjectile();
				CurrentMana = CurrentMana - AttackCost;
			}
		};
		if (bMissileSpawned == true)
		{
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, TEXT("Cooldown Active"));
		};
	};
}

//Simple testing function for recharging Mana to full.
void ATenTenGamesProjectCharacter::RechargeMana()
{
	CurrentMana = MaxMana;
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Stamina Recharged!"));
}

void ATenTenGamesProjectCharacter::SpawnProjectile()
{
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnTransform);

	{
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnTransform);
		bMissileSpawned = true;

		if (ActorToSpawn == nullptr)
		{
			bMissileSpawned = false;			// if at any point the "ActorToSpawn" becomes invalid, set bMissileSpawned to false (allowing another missile to be spawned)
												// Design wise would likely change how this works in future, but for testing this is fine.
		}
	};
}









