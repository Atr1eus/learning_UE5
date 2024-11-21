// Copyright Epic Games, Inc. All Rights Reserved.

#include "week1GameMode.h"
#include "week1Character.h"
#include "UObject/ConstructorHelpers.h"

Aweek1GameMode::Aweek1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
