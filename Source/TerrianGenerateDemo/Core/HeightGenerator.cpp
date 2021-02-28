// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HeightGenerator.h"
#include "Tool/NoiseTool.h"

float HeightGenerator::GetHeight(FVector2D BlockPosition)
{
	return 0.0f;
}

// #BUG1:仍存在边缘问题（0,0）
void HeightGenerator::GenerateHeight(Chunk& chunk)
{
	int32 m = 3;
	int32 maxHeigh = 100.0f;

	for (int d = 0; d < 3; ++d,m*=2) {
		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,m,1);
		//NoiseTool::prehandlePerlinNoise(ChunkPosition,m,1);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = FVector2D((float)i / MaxBlocksWidth / m, float(j) / MaxBlocksWidth / m);
			chunk.BlocksHeight[i][j] += 
			(	
				FMath::Clamp<float>(
				NoiseTool::simplexNoise(pf)
				//NoiseTool::perlinNoise(pf)
				,-1.0f,1.0f)*maxHeigh + maxHeigh
			)/3/3;
		}

	}
}
	