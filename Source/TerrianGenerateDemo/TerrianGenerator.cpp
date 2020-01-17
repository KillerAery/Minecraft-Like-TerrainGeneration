// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrianGenerator.h"
#include "NoiseTool.h"

TerrianGenerator::TerrianGenerator()
{
}

TerrianGenerator::~TerrianGenerator()
{
}

float TerrianGenerator::GetHeight(FVector2D BlockPosition)
{
	return 0.0f;
}

void TerrianGenerator::GenerateHeight(FVector2D ChunkPosition, int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth])
{
	FVector2D pi = FVector2D(floor(ChunkPosition.X), floor(ChunkPosition.Y));

	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1.0f,pi.Y},{pi.X,pi.Y + 1.0f},{pi.X + 1.0f,pi.Y + 1.0f} };

	FVector2D pf, w;

	FVector2D vertexAfterHash22[4] = { NoiseTool::hash22(vertex[0]),NoiseTool::hash22(vertex[1]) ,NoiseTool::hash22(vertex[2]) ,NoiseTool::hash22(vertex[3])};
	//TODO
	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{

			pf = FVector2D(i / float(MaxBlocksWidth), j / float(MaxBlocksWidth));
			w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

			BlocksHeight[i][j] =
				//1.0f * FMath::Clamp<float>(FMath::Lerp(
				//	FMath::Lerp<float>(NoiseTool::hash21(vertex[0]) % 51,
				//		NoiseTool::hash21(vertex[1]) % 51,
				//		w.X),
				//	FMath::Lerp<float>(NoiseTool::hash21(vertex[2]) % 51,
				//		NoiseTool::hash21(vertex[3]) % 51,
				//		w.X),
				//	w.Y), 0, 51)
				//+
				0.5f * (FMath::Clamp<float>(FMath::Lerp(
					FMath::Lerp(NoiseTool::grad(vertexAfterHash22[0], pf),
						NoiseTool::grad(vertexAfterHash22[1], pf - FVector2D(1, 0)),
						w.X),
					FMath::Lerp(NoiseTool::grad(vertexAfterHash22[2], pf - FVector2D(0, 1)),
						NoiseTool::grad(vertexAfterHash22[3], pf - FVector2D(1, 1)),
						w.X),
					w.Y), -10.0f, 10.0f)*10.0f+20);

			BlocksHeight[i][j] = FMath::Clamp(BlocksHeight[i][j], 0, 255);
		}
}
