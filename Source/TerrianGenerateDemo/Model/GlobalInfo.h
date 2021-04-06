// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"


const int32 SeaLevel = 79;

using HeightMap = int32(*)[MaxBlocksWidth];
/**
 * 
 */
class TERRIANGENERATEDEMO_API GlobalInfo
{
public:
	//全局方块列表
	TMap<uint64,int32> GolbalBlocksID;
	//全局高度
	TMap<uint64,HeightMap> GolbalHeight;
	//全局建筑列表
	TArray<TTuple<uint64,int32,int32>> GolbalBudildings;
public:
	GlobalInfo();

	~GlobalInfo();
	
	//高度若不存在则返还-1
	int32 GetHeight(FVector2D pos);

	//高度若不存在则返还-1
	int32 GetHeight(int32 x,int32 y);

	//设置高度
	void SetChunkHeight(FVector2D chunkPosition,HeightMap heightmap);

	//设置高度
	void SetHeight(FVector2D pos,int32 height);

	//设置高度	
	void SetHeight(int32 x,int32 y,int32 height);

	//添加方块
	void AddBlock(FVector pos,int32 BlockID);

	//移除方块
	void RemoveBlock(FVector pos);
	
	//添加建筑
	void AddBuilding(FVector pos,int32 BlockID);

	//移除建筑
	void RemoveBuilding(FVector pos);
};
