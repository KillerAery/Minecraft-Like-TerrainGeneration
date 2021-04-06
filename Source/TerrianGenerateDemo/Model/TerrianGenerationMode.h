// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/GlobalInfo.h"
#include "Model/Chunk.h"

#include "Model/Block.h"

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "TerrianGenerationMode.generated.h"

const int32 LoadRadius = 5;
const int32 ChunkSize = LoadRadius * 2 - 1;
const int32 Center = LoadRadius - 1;

const int32 DisplayRadius = LoadRadius-1;
const int32 DisplaySize = DisplayRadius * 2- 1;
const int32 DisplayCenter = DisplayRadius - 1;
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
	//全部加载Chunk
	TArray<Chunk> Chunks;
	//全部显示Chunk
	TSet<Chunk*> Chunks2Display;
	//全部Block
	TMap<uint64,ABlock*> Blocks;
	//全局信息
	GlobalInfo Info;
protected:
	//------------------------加载Chunk---------------------
	//是否需要加载chunk
	bool NeedLoadChunk(FVector2D chunkPosition);
	//加载地形方块ID
	void LoadTerrianBlocksID(Chunk& chunk);
	//生成建筑方块
	void GenerateBuildingBlocks();
	//载入Chunk
	void LoadChunk(Chunk& chunk);

	//------------------------显示Chunk---------------------
	//是否需要显示chunk
	Chunk* GetDisplayChunk(FVector2D chunkPosition);
	//显示Chunk
	void DisplayChunk(Chunk& chunk);

protected:
	//创建Block Actor
	bool CreateBlock(int32 id, FVector pos);
	
	//创建Building Actor
	bool CreateBuilding(int32 id,int32 rotate, FVector pos);

	//暴露方块
	//void Expose(GlobalInfo& info,int32 i,int32 j,int32 k);
};
