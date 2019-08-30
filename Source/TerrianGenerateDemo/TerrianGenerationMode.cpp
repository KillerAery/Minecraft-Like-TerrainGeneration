// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrianGenerationMode.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "TerrianGenerateDemoHUD.h"
#include "TerrianGenerateDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Block.h"

ATerrianGenerationMode::ATerrianGenerationMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATerrianGenerateDemoHUD::StaticClass();

	ConstructorHelpers::FObjectFinder<UClass> block(TEXT("Blueprint'/Game/Blueprints/BP_Block.BP_Block'"));
	if (block.Succeeded()) {
		BlockClass = block.Object;
	}

}

void ATerrianGenerationMode::BeginPlay()
{
}

void ATerrianGenerationMode::Tick(float DeltaSeconds)
{
}

UObject* ATerrianGenerationMode::SpawnObject(UClass* ObjClass)
{
	UWorld* World = GEngine->GetWorldFromContextObject(this);
	UObject* tempObject = NewObject<UObject>(World, ObjClass);
	UE_LOG(LogTemp, Warning, TEXT("Spawn!"));
	return tempObject;
}

UClass * ATerrianGenerationMode::GetBlockClass() const
{
	return BlockClass;
}
