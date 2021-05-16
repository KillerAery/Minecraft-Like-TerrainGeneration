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
private:
	//全局方块列表
	TMap<uint64,int32> GlobalBlocksID;
	//待显示方块列表
	TArray<TPair<uint64,int32>> Blocks2Display;
	//待显示建筑列表
	TArray<TTuple<uint64,int32,int32>> Budildings2Display;
	//全局高度
	TMap<uint64,HeightMap> GlobalHeight;
public:
	GlobalInfo();

	~GlobalInfo();

	//高度若不存在则返还-1
	int32 GetHeight(int32 x,int32 y);

	//设置高度
	void SetChunkHeight(int32 x,int32 y,HeightMap heightmap);

	//设置高度	
	void SetHeight(int32 x,int32 y,int32 height);

	//添加方块
	void AddBlock(FVector pos,int32 BlockID);

	//添加非显示方块
	void AddBlockWithoutDisplay(FVector pos,int32 BlockID);

	//添加仅显示方块
	void AddBlockOnlyDisplay(FVector pos,int32 BlockID);

	//移除方块
	void RemoveBlock(FVector pos);

	//查找方块
	int32* FindBlock(FVector pos);

	//待显示方块列表
	TArray<TPair<uint64,int32>>& GetBlocks2Display();
	
	//添加建筑
	void AddBuilding(FVector pos,int32 BuildingID,int32 rotate);

	//移除建筑
	void RemoveBuilding(FVector pos);

	//待显示建筑列表
	TArray<TTuple<uint64,int32,int32>>& GetBuildings2Display();
};
