// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/TemperatureGenerator.h"
#include "Tool/NoiseTool.h"

void TemperatureGenerator::GenerateTemperature(Chunk& chunk)
{
	const int32 times = 3;
	int32 cystalSize  = 16;

    NoiseTool::setSeed(143+NoiseTool::hash21(chunk.ChunkPosition));

	for (int d = 0; d < times; ++d,cystalSize*=2) {

	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
		FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
		//温度[-1,1]
		float value = 
			FMath::Clamp<float>(
			NoiseTool::simplexNoise(pf)
			+ (NoiseTool::rand(pf)-0.5f)*0.05f
			,-1.0f,1.0f);

		chunk.BlocksTemperature[i][j] += value / times;
	}

	}
}
