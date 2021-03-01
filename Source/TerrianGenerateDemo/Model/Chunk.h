// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Block.h"

const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	Chunk(FVector2D chunkPosition);

public:
	//chunk中心位置
	FVector2D ChunkPosition;
	
	//chunk地形高度[0~256]
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
	
	//chunk地形温度[-1.0f,1.0f]
	float BlocksTemperature[MaxBlocksWidth][MaxBlocksWidth];
	
	//chunk地形湿度[0.0f,1.0f]
	float BlocksHumidity[MaxBlocksWidth][MaxBlocksWidth];

	//chunk特殊方块列表
	TArray<TPair<TTuple<int32,int32,int32>,int32>> BlocksID;

	//chunk所有显示用ABlock
	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];
};
