// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Block.h"

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	Chunk(FVector2D chunkPosition);
	FVector2D ChunkPosition;
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];

	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];
};
