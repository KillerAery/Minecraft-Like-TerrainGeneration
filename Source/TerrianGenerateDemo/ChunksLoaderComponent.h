// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chunk.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunksLoaderComponent.generated.h"

const size_t LoadRadius = 3;

const size_t ChunkSize = LoadRadius * 2 - 1;

const size_t Center = LoadRadius - 1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRIANGENERATEDEMO_API UChunksLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChunksLoaderComponent();

	UFUNCTION(BlueprintCallable, Category = "MyFunc")
	void SetPlayerLoactionEveryTick(FVector location);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector position3D;

	AChunk* Chunks[2][ChunkSize][ChunkSize];

	size_t ChunksIndex = 0;

	void UpdateChunks();

	bool NeedToLoadChunk(FVector2D position, FVector2D chunkPosition);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
