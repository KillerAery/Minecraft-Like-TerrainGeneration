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
	for (int d = 0; d < 1; ++d, m *= 2) {
		FVector2D pi = FVector2D((int32)ChunkPosition.X / m, (int32)ChunkPosition.Y / m) ;
		FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

		FVector2D pf, w;

		FVector2D vertexAfterHash22[4] = { NoiseTool::hash22(vertex[0]),NoiseTool::hash22(vertex[1]) ,NoiseTool::hash22(vertex[2]) ,NoiseTool::hash22(vertex[3])};
		
		for (int i = 0; i < MaxBlocksWidth; ++i)
			for (int j = 0; j < MaxBlocksWidth; ++j)
			{
				pf = FVector2D((float)i / (float)MaxBlocksWidth / m, float(j) / (float)MaxBlocksWidth / m);
				//w.X = pf.X * pf.X * (3.0f - 2.0f * pf.X);
				//w.Y = pf.Y * pf.Y * (3.0f - 2.0f * pf.Y);
				//w.X = pf.X*(2.0f-pf.X);
				//w.Y = pf.Y*(2.0f-pf.Y);
				w.X = pf.X;
				w.Y = pf.Y;

				BlocksHeight[i][j] =
				/*
					1.0f * FMath::Clamp<float>(FMath::Lerp(
						FMath::Lerp<float>(NoiseTool::hash21(vertex[0]) % 51,
							NoiseTool::hash21(vertex[1]) % 51,
							w.X),
						FMath::Lerp<float>(NoiseTool::hash21(vertex[2]) % 51,
							NoiseTool::hash21(vertex[3]) % 51,
							w.X),
						w.Y), 0, 51);
				*/	
					(FMath::Clamp<float>(
						FMath::Lerp(
							FMath::Lerp(
								NoiseTool::grad(vertexAfterHash22[0], pf),
								NoiseTool::grad(vertexAfterHash22[1], pf - FVector2D(1.0f, 0.0f)),
								w.X
							),
							FMath::Lerp(
								NoiseTool::grad(vertexAfterHash22[2], pf - FVector2D(0.0f, 1.0f)),
								NoiseTool::grad(vertexAfterHash22[3], pf - FVector2D(1.0f, 1.0f)),
								w.X
							),
							w.Y
						), -1.0f, 1.0f) * 32.0f * m) + 32.0f;
					
				BlocksHeight[i][j] = FMath::Clamp(BlocksHeight[i][j], 0, 64);
			}
	}
}
