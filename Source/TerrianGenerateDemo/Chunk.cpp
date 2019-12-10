// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "NoiseTool.h"


Chunk::Chunk(FVector2D chunkPosition)
{
	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	for (int k = 0; k < MaxBlocksHeigth; ++k)
		Blocks[i][j][k] = nullptr;

	ChunkPosition = chunkPosition;
	GeneratePerlinNoise();
}

void Chunk::GeneratePerlinNoise()
{
	FVector2D pi = FVector2D(floor(ChunkPosition.X), floor(ChunkPosition.Y));
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1.0f,pi.Y},{pi.X,pi.Y + 1.0f},{pi.X + 1.0f,pi.Y + 1.0f} };

	FVector2D pf,w;
	//TODO
	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
			
			pf = FVector2D(i/float(MaxBlocksWidth),j/float(MaxBlocksWidth));
			w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

			BlocksHeight[i][j] =
				1.0f * FMath::Clamp<float>(FMath::Lerp(
				FMath::Lerp<float>(NoiseTool::hash21(vertex[0])%51,
					NoiseTool::hash21(vertex[1]) % 51,
					w.X),
				FMath::Lerp<float>(NoiseTool::hash21(vertex[2]) % 51,
					NoiseTool::hash21(vertex[3]) % 51,
					w.X),
				w.Y),0,51)
				+
				0.5f * (FMath::Clamp<float>(FMath::Lerp(
				FMath::Lerp(NoiseTool::grad(vertex[0], pf),
					NoiseTool::grad(vertex[1], pf - FVector2D(1, 0)),
					w.X),
				FMath::Lerp(NoiseTool::grad(vertex[2], pf - FVector2D(0, 1)),
					NoiseTool::grad(vertex[3], pf - FVector2D(1, 1)),
					w.X),
				w.Y), -1.0f, 1.0f) * 5 + 20);

			BlocksHeight[i][j] = FMath::Clamp(BlocksHeight[i][j], 0, 255);
	}
}
