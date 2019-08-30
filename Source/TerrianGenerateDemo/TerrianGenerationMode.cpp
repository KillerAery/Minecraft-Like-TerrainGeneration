// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrianGenerationMode.h"
#include "TerrianGenerateDemoHUD.h"
#include "TerrianGenerateDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATerrianGenerationMode::ATerrianGenerationMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATerrianGenerateDemoHUD::StaticClass();


}

void ATerrianGenerationMode::BeginPlay()
{
}

void ATerrianGenerationMode::Tick(float DeltaSeconds)
{
}



