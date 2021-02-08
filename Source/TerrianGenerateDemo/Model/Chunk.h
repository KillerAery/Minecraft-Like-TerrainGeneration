// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Block.h"
#include "Core/TerrianGenerateSetting.h"

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	Chunk(FVector2D chunkPosition);
	FVector2D ChunkPosition;
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
	BiomeType BlocksBiome[MaxBlocksWidth][MaxBlocksWidth];

	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];
};
