// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveGenerator.h"
#include "Tool/NoiseTool.h"

void CaveGenerator::GenerateCave(Chunk& chunk,GlobalInfo& info){
    //晶格大小
	int32 m = 2;

	NoiseTool::prehandleSimplexNoise(FVector(chunk.ChunkPosition.X,chunk.ChunkPosition.Y,0),m,1);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
    for (int k = chunk.BlocksHeight[i][j]-10; k <= chunk.BlocksHeight[i][j]; ++k)
	{
		FVector pf = FVector(
                float(i) / MaxBlocksWidth / m, 
                float(j) / MaxBlocksWidth / m,
                float(k) / MaxBlocksWidth / m
         );

        //若高于一定阈值，挖空
        if(NoiseTool::simplexNoise(pf)>0.98f){
            uint64 index =
            NoiseTool::Index(
                chunk.ChunkPosition.X*16+i,
                chunk.ChunkPosition.Y*16+j,
                k);
            info.GolbalBlocksID.Emplace(index,0);
            chunk.BlocksID.Emplace(index,0);
        };
	}
}