// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BuildingEnergyGameMode.h"
#include "BuildingEnergyHUD.h"
#include "BuildingEnergyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABuildingEnergyGameMode::ABuildingEnergyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABuildingEnergyHUD::StaticClass();
}
