// Fill out your copyright notice in the Description page of Project Settings.


#include "HumidityGenerator.h"
#include "Tool/NoiseTool.h"


void HumidityGenerator::GenerateHumidity(Chunk& chunk){
	const int32 times = 3;
	int32 cystalSize  = 16;

    NoiseTool::setSeed(201);

	for (int d = 0; d < times; ++d) {

		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
		
			//湿度[0,1]
			float value = 
            	FMath::Clamp<float>(
            		FMath::Abs(NoiseTool::simplexNoise(pf))+ NoiseTool::rand(pf)*0.05f
            	,0.0f,1.0f);

			chunk.BlocksHumidity[i][j] += value / times;
		}
		
		cystalSize*=2;
	}
}