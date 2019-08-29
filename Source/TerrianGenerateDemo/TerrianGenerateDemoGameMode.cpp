// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TerrianGenerateDemoGameMode.h"
#include "TerrianGenerateDemoHUD.h"
#include "TerrianGenerateDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATerrianGenerateDemoGameMode::ATerrianGenerateDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATerrianGenerateDemoHUD::StaticClass();
}

void ATerrianGenerateDemoGameMode::BeginPlay()
{
}

void ATerrianGenerateDemoGameMode::Tick(float DeltaSeconds)
{
} 