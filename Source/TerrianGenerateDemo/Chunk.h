// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Block.h"

const size_t BlockWidth = 100;
const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

class TERRIANGENERATEDEMO_API Chunk
{
public:	
	// Sets default values for this actor's properties
	Chunk(FVector2D chunkPosition);
	FVector2D ChunkPosition;
	ABlock* Blocks[MaxBlocksWidth][MaxBlocksWidth][MaxBlocksHeigth];
	int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth];
protected:
	void GeneratePerlinNoise();
};
