// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveGenerator.h"
#include "Tool/NoiseTool.h"

void CaveGenerator::GenerateCave(Chunk& chunk,GlobalInfo& info){
    //晶格大小
	int32 m = 4;

	NoiseTool::prehandleSimplexNoise(FVector(chunk.ChunkPosition.X,chunk.ChunkPosition.Y,0),m,1);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
    for (int k = 0; k < MaxBlocksWidth*MaxBlocksWidth; ++k)
	{
        if(k > chunk.BlocksHeight[i][j]){
            break;
        }

		FVector pf = FVector(
                float(i) / MaxBlocksWidth / m, 
                float(j) / MaxBlocksWidth / m,
                float(k) / MaxBlocksWidth / m
         );

        //若高于一定阈值，挖空
        if(NoiseTool::simplexNoise(pf)>-0.1f){
            info.SpecialBlocksID.Emplace(
                NoiseTool::Index(
                    chunk.ChunkPosition.X*16+i,
                    chunk.ChunkPosition.Y*16+j,
                    k)
                ,0);
        };
	}
	

}