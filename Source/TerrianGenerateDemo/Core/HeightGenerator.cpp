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

void HeightGenerator::GenerateHeight(FVector2D ChunkPosition, int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth])
{
	int32 m = 1;
	for (int d = 0; d < 3; ++d,m*=2) {
		FVector2D pi = FVector2D((int32)ChunkPosition.X / m, (int32)ChunkPosition.Y / m) ;
		FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };
		FVector2D vertexAfterHash22[4] = { NoiseTool::hash22(vertex[0]),NoiseTool::hash22(vertex[1]) ,NoiseTool::hash22(vertex[2]) ,NoiseTool::hash22(vertex[3])};

		//预设置全局顶点
		NoiseTool::setGlobalVertex(vertexAfterHash22);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = (ChunkPosition-pi*m)/m + FVector2D((float)i / MaxBlocksWidth / m, float(j) / MaxBlocksWidth / m);

			BlocksHeight[i][j] += (FMath::Clamp<float>(
				NoiseTool::perlinNoise(pf,1,1)
				,-1.0f,1.0f)*32.0f+ 32.0f)*0.5f/m;
		}

	}
}
