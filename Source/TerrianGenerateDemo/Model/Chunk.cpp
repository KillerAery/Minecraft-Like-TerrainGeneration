// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Chunk.h"
#include "Tool/NoiseTool.h"

Chunk::Chunk(FVector2D chunkPosition):
	BlocksHeight{0},BlocksTemperature{0},BlocksHumidity{0},BlocksBiome{0}
{
	ChunkPosition = chunkPosition;
}

int32 Chunk::CaculateBlockID(int32 i,int32 j,int32 k){
    if(i<0||i>=16||j<0||j>=16){
        return 0;
    }

    int32 dk = BlocksHeight[i][j]-k;
    if(dk>=5){
        return 2;
    }
    //边缘高度选择 地表、地下
	else if(dk>=3){
		return 2;
	}
    else if(dk>=2){
        return 3;
    }
    else{			
        int32 targetBlockID;
        /* None = 0 雪地 Snow = 1 草地 Green = 2
        泥地 Dry = 3 石地 Stone = 4 沙漠 Desert = 5
        */
        switch(BlocksBiome[i][j])
		{
			case 1:targetBlockID = 10;break;
			case 2:targetBlockID = 1;break;
			case 3:targetBlockID = 3;break;
			case 4:targetBlockID = 5;break;
			case 5:targetBlockID = 4;break;
			default:targetBlockID = 0;break;
		};
        return targetBlockID;
    }
}

/*
void Chunk::Expose(GlobalInfo& info,int32 i,int32 j,int32 k){
    const int32 dx[6] = {1,-1,0,0,0,0};
    const int32 dy[6] = {0,0,1,-1,0,0};
    const int32 dz[6] = {0,0,0,0,-1,1};
    for(int d = 0;d<6;++d){
        uint64 index = NoiseTool::Index(
            ChunkPosition.X*16+i+dx[d],
            ChunkPosition.Y*16+j+dy[d],
            k+dz[d]
            );
        if(!info.GolbalBlocksID.Find(index)){
            info.GolbalBlocksID.Emplace(index,CaculateBlockID(i+dx[d],j+dy[d],k+dz[d]));
        }
    }
}
*/