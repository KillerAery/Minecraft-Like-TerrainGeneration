// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/BiomeGenerator.h"
#include "Core/NoiseTool.h"

void BiomeGenerator::GenerateBiome(Chunk& chunk)
{
	int32 cystalSize  = 16;

	for (int d = 0; d < 3; ++d,cystalSize*=2) {
	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize,1);
	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
		FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
		//温度[-1,1]
		float value = FMath::Clamp<float>(
				NoiseTool::simplexNoise(pf)
				,-1.0f,1.0f) + ((NoiseTool::hash21(chunk.ChunkPosition*16 + pf*1024))-512.0f)/512.0f*0.05f;
		chunk.BlocksTemperature[i][j] += value/3;
	}
	}
}
