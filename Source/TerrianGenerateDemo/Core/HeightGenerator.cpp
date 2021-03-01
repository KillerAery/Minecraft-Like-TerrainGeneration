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
	//晶格大小
	int32 m[3] = {4,16,64};
	//权重
	float f[3] = {0.12f,0.2f,0.68f};
	//波幅
	float maxHeigh[3] = {100.0f,100.0f,100.0f};

	for (int d = 0; d < 3; ++d) {
		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,m[d],1);
		//NoiseTool::prehandlePerlinNoise(ChunkPosition,m,1);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = FVector2D((float)i / MaxBlocksWidth / m[d], float(j) / MaxBlocksWidth / m[d]);
			chunk.BlocksHeight[i][j] += 
			(	
				FMath::Clamp<float>(
				NoiseTool::simplexNoise(pf)
				//NoiseTool::perlinNoise(pf)
				,-1.0f,1.0f)*maxHeigh[d] + maxHeigh[d]
			)*f[d];
		}

	}
}
	