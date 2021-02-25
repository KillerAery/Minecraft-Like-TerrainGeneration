// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Block.h"

const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	Chunk(FVector2D chunkPosition);
	FVector2D ChunkPosition;
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
	float BlocksTemperature[MaxBlocksWidth][MaxBlocksWidth];

	TArray<TPair<TTuple<int32,int32,int32>,int32>> BlocksID;
	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];
};
