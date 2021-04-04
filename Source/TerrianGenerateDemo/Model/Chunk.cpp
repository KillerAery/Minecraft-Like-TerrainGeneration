// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Chunk.h"
#include "Tool/NoiseTool.h"

Chunk::Chunk(FVector2D chunkPosition):
	BlocksHeight{0},BlocksTemperature{0},BlocksHumidity{0},BlocksBiome{0}
{    
    ChunkPosition = chunkPosition;
}

int32 Chunk::CaculateBlockID(int32 i,int32 j,int32 k){
    if(i<0||i>=16||j<0||j>=16){return 0;}

    int32 dk = BlocksHeight[i][j]-k;
    /* None = 0 雪地 Snow = 1 草地 Green = 2
    泥地 Dry = 3 石地 Stone = 4 沙漠 Desert = 5
    */
    //地下石头
    if(dk>=3){return 2;}
    //地下泥土
    if(dk>=1){return 3;}
    //地表方块	
    switch(BlocksBiome[i][j])
	{
			case 1:return 10;break;
			case 2:return 1;break;
			case 3:return 3;break;
			case 4:return 5;break;
			case 5:return 4;break;
			default:return 0;break;
	};
}