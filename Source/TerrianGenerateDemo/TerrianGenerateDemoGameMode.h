// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TerrianGenerateDemoGameMode.generated.h"

UCLASS(minimalapi)
class ATerrianGenerateDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATerrianGenerateDemoGameMode();

	void BeginPlay()override;

	void Tick(float DeltaSeconds)override;

	//UPROPERTY(BlueprintReadOnly, Category = "Watch")
	//TArray<Chunk> ChunksLoaded;

	UPROPERTY(BlueprintReadOnly,Category = "Watch")
	FVector CenterChunkPosition;

	UPROPERTY(EditAnywhere,Category = "Setup")
	int ChunksLoadRadius = 2;
};



