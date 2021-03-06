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
        if(NoiseTool::simplexNoise(pf)>0.95f){
            info.SpecialBlocksID.Emplace(
                NoiseTool::Index(
                    chunk.ChunkPosition.X*16+i,
                    chunk.ChunkPosition.Y*16+j,
                    k)
                ,0);
                
            //暴露周围方块
            Expose(chunk,info,i,j,k);
        };
	}
}


void CaveGenerator::Expose(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 k){
    const int32 dx[6] = {1,-1,0,0,0,0};
    const int32 dy[6] = {0,0,1,-1,0,0};
    const int32 dz[6] = {0,0,0,0,-1,1};
    for(int d = 0;d<6;++d){
        uint64 index = NoiseTool::Index(
            chunk.ChunkPosition.X*16+i+dx[d],
            chunk.ChunkPosition.Y*16+j+dy[d],
            k+dz[d]
            );
        if(!info.SpecialBlocksID.Find(index)){
            info.SpecialBlocksID.Emplace(index,CaculateUnderBlock(chunk,i+dx[d],j+dy[d],k+dz[d]));
        }
    }
}

int32 CaveGenerator::CaculateUnderBlock(Chunk& chunk,int32 i,int32 j,int32 k){
    if(i<0||i>=16||j<0||j>=16){
        return 0;
    }
/*
	None = 0
	雪地 Snow = 1
	草地 Green = 2
	泥地 Dry = 3
    石地 Stone = 4
	沙漠 Desert = 5
*/
    int32 dk = chunk.BlocksHeight[i][j]-k;
    if(dk>=5){
        return 5;
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
        switch(chunk.BlocksBiome[i][j])
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