﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Block.h"

const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	Chunk(FVector2D chunkPosition);

	//计算方块ID
	int32 CaculateBlockID(int32 i,int32 j,int32 k);

	//暴露方块
	//void Expose(GlobalInfo& info,int32 i,int32 j,int32 k);
public:
	//chunk中心位置
	FVector2D ChunkPosition;
	
	//chunk地形高度[0~256]
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
	
	//chunk地形温度[-1.0f,1.0f]
	float BlocksTemperature[MaxBlocksWidth][MaxBlocksWidth];
	
	//chunk地形湿度[0.0f,1.0f]
	float BlocksHumidity[MaxBlocksWidth][MaxBlocksWidth];

	//chunk生物群落属性[1,2,3,4...]
	int32 BlocksBiome[MaxBlocksWidth][MaxBlocksWidth];

	TArray<TPair<uint64,int32>> BlocksID;
};
