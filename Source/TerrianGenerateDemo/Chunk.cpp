// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "HeightGenerator.h"
#include "BiomeGenerator.h"

Chunk::Chunk(FVector2D chunkPosition)
{
	ChunkPosition = chunkPosition;

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	for (int k = 0; k < MaxBlocksHeigth; ++k)
		Blocks[i][j][k] = nullptr;

	HeightGenerator::GenerateHeight(ChunkPosition, BlocksHeight);
	BiomeGenerator::GenerateBiome(ChunkPosition, BlocksBiome);

}