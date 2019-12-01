// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chunk.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChunksLoaderComponent.generated.h"

const int32 LoadRadius = 3;

const int32 ChunkSize = LoadRadius * 2 - 1;

const int32 Center = LoadRadius - 1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRIANGENERATEDEMO_API UChunksLoaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChunksLoaderComponent();

	UFUNCTION(BlueprintCallable, Category = "MyFunc")
	void SetPlayerLoactionEveryTick(FVector location);
	
	UPROPERTY(VisibleAnywhere)
	FVector2D ChunksCenterPosition;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	Chunk* Chunks[2][ChunkSize][ChunkSize];

	size_t ChunksIndex = 0;

	void UpdateChunks();

	bool NeedChunk(FVector2D chunkPosition);

	Chunk* GenerateChunk(FVector2D chunkPosition);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
