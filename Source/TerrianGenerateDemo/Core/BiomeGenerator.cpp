// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/BiomeGenerator.h"
#include "Core/NoiseTool.h"

BiomeGenerator::BiomeGenerator()
{
}

BiomeGenerator::~BiomeGenerator()
{
}


void BiomeGenerator::GenerateBiome(Chunk& chunk)
{
	int32 cystalSize = 64;
	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize,1);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
		FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
		float biomeValue = FMath::Clamp<float>(
				NoiseTool::simplexNoise(pf)
				,-1.0f,1.0f) + FMath::RandRange(-0.1f,0.1f);
		chunk.BlocksTemperature[i][j] = biomeValue;
	}
}
