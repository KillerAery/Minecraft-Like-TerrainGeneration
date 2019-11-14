// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Block.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Chunk.generated.h"

const size_t BlockWidth = 100;
const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

UCLASS()
class TERRIANGENERATEDEMO_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	static void Initialize(FVector2D chunkPosition);

	UPROPERTY(VisibleAnywhere)
	FVector2D ChunkPosition = FVector2D(0, 0);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GeneratePerlinNoise();
	void GenerateBlocks();

	static FVector2D InitialChunkPosition;//TODO
private:

	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];

	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
