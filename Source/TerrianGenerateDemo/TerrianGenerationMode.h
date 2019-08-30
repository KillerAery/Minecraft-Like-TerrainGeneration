// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "TerrianGenerationMode.generated.h"

/**
 * 
 */
UCLASS()
class TERRIANGENERATEDEMO_API ATerrianGenerationMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATerrianGenerationMode();

	void BeginPlay()override;

	void Tick(float DeltaSeconds)override;

	UFUNCTION(BlueprintCallable)
	UObject* SpawnObject(UClass* ObjClass);

	UFUNCTION(BlueprintCallable)
	UClass* GetBlockClass()const;

	UPROPERTY(BlueprintReadOnly, Category = "Watch")
	FVector CenterChunkPosition;

	UPROPERTY(EditAnywhere, Category = "Watch")
	UClass* BlockClass;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int ChunksLoadRadius = 2;
};
