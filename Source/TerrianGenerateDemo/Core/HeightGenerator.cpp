// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HeightGenerator.h"
#include "Core/NoiseTool.h"

HeightGenerator::HeightGenerator()
{
}

HeightGenerator::~HeightGenerator()
{
}

float HeightGenerator::GetHeight(FVector2D BlockPosition)
{
	return 0.0f;
}

// #BUG1:仍存在边缘问题（0,0）
void HeightGenerator::GenerateHeight(FVector2D ChunkPosition, int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth])
{
	int32 m = 1;
	int32 maxHeigh = 64.0f;

	for (int d = 0; d < 5; ++d,m*=2) {
		NoiseTool::prehandlePerlinNoise(ChunkPosition,m,1);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = FVector2D((float)i / MaxBlocksWidth / m, float(j) / MaxBlocksWidth / m);
			BlocksHeight[i][j] += 
			(	
				FMath::Clamp<float>(
				NoiseTool::perlinNoise(pf)
				,-1.0f,1.0f)*maxHeigh + maxHeigh

			)*0.5f/m;
		}

	}
}
	