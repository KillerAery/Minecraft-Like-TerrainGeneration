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
	int32 frequency = 64.0f;

	for (int d = 0; d < 3; ++d,m*=2) {
		FVector2D pi = FVector2D(floor(ChunkPosition.X / m), floor(ChunkPosition.Y / m));
		FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };
		FVector2D vertexAfterHash22[4] = { NoiseTool::hash22(vertex[0]),NoiseTool::hash22(vertex[1]) ,NoiseTool::hash22(vertex[2]) ,NoiseTool::hash22(vertex[3])};

		//预设置全局顶点
		NoiseTool::setGlobalVertex(vertexAfterHash22);

		FVector2D pChunkf = (ChunkPosition-pi*m)/m;

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = pChunkf + FVector2D((float)i / MaxBlocksWidth / m, float(j) / MaxBlocksWidth / m);

			BlocksHeight[i][j] += (FMath::Clamp<float>(
				NoiseTool::perlinNoise(pf,1,1)
				,-1.0f,1.0f)*frequency + frequency)*0.5f/m;
		}

	}
}
