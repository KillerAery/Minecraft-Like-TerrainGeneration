// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HeightGenerator.h"
#include "Tool/NoiseTool.h"


void HeightGenerator::GenerateHeight(Chunk& chunk,GlobalInfo& info)
{
	//晶格大小
	int32 cystalSize[3] = {4,16,64};
	//权重
	float weight[3] = {0.12f,0.22f,0.66f};
	//波幅
	float maxHeigh[3] = {100.0f,100.0f,100.0f};
	
	for (int d = 0; d < 3; ++d) {

		////预处理perlin噪声
		//NoiseTool::prehandlePerlinNoise(chunk.ChunkPosition,m[d]);
		//预处理simplex噪声
		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize[d]);

		for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize[d], float(j) / MaxBlocksWidth / cystalSize[d]);
			
			//chunk.BlocksHeight[i][j] += (NoiseTool::perlinNoise(pf)*maxHeigh[d] + maxHeigh[d])*weight[d];
			chunk.BlocksHeight[i][j] += (NoiseTool::simplexNoise(pf)*maxHeigh[d] + maxHeigh[d])*weight[d];
		}
	}
	
	//更新全局信息中的高度
	info.SetChunkHeight(chunk.ChunkPosition.X,chunk.ChunkPosition.Y,chunk.BlocksHeight);
}
	