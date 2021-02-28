// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeGenerator.h"
#include "Tool/NoiseTool.h"

void TreeGenerator::GenerateTree(Chunk& chunk){
    int32 cystalSize = 16;
	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize,1);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);

        //-----------
        //--生成草
        float valueTemperature = chunk.BlocksTemperature[i][j];
		float possible = 
        (NoiseTool::rand(chunk.ChunkPosition*11 + pf*1011))
            - FMath::Abs(valueTemperature+0.1f)*0.22f;
        
        //若满足概率
        if(possible > 0.9f){
            valueTemperature += (NoiseTool::rand(chunk.ChunkPosition*17 + pf*10207)-0.5f)*0.2f;
            //方块ID 11为绿草,ID 12为黄草，ID 13为白草
            if(valueTemperature>0.3f){
                chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(i,j,chunk.BlocksHeight[i][j]+1),12);
            }
            else if(valueTemperature>-0.3f){
                chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(i,j,chunk.BlocksHeight[i][j]+1),11);
            }
            else{
                chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(i,j,chunk.BlocksHeight[i][j]+1),13);
            }
        }

        //--生成树
	}
}