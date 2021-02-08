// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Chunk.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "TerrianGenerationMode.generated.h"

const int32 LoadRadius = 2;
const int32 ChunkSize = LoadRadius * 2 - 1;
const int32 Center = LoadRadius - 1;
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

	//设置摄像机（玩家）中心位置
	UFUNCTION(BlueprintCallable, Category = "Func")
	void SetCameraLoaction(FVector location);

	//更新周围Chunk
	UFUNCTION(BlueprintCallable, Category = "Func")
	void UpdateChunks();

	//中心位置
	UPROPERTY(VisibleAnywhere)
	FVector2D ChunksCenterPosition;
protected:
	//全部Chunk
	Chunk* Chunks[2][ChunkSize][ChunkSize];
	//当前缓冲区索引（使用双重缓冲）
	size_t ChunksIndex = 0;
	bool NeedChunk(FVector2D chunkPosition);
	Chunk* GenerateChunk(FVector2D chunkPosition);
	ABlock* CreateBlock(int32 id, FVector location);
};
