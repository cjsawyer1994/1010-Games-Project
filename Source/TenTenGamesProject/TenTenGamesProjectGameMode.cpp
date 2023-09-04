// Copyright Epic Games, Inc. All Rights Reserved.

#include "TenTenGamesProjectGameMode.h"
#include "TenTenGamesProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATenTenGamesProjectGameMode::ATenTenGamesProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
