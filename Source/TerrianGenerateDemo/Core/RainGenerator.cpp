// Fill out your copyright notice in the Description page of Project Settings.


#include "RainGenerator.h"
#include "Tool/NoiseTool.h"

TSet<uint64> RainGenerator::waters = TSet<uint64>();

void RainGenerator::GenerateRain(Chunk& chunk,GlobalInfo& info){

    NoiseTool::setSeed(2183);
    waters.Reset();

    //初始化雨滴（随机生成雨滴）
    for(int n = 1;n>0;--n){
        float possible = NoiseTool::rand(chunk.ChunkPosition+FVector2D(13.51f,2.16f));
        if(possible <= 0.95f) continue;
        int32 x = NoiseTool::randInt(chunk.ChunkPosition)%15+1;
        int32 y = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(-1.512f,41.421f))%15+1;
        flow(chunk,info,60.0f,x,y,chunk.BlocksHeight[x][y]+1);
    }
}


void RainGenerator::flow(Chunk& chunk,GlobalInfo& info,float rain,int i,int j,int k){
    if(rain<0.5f || k<=SeaLevel)return;

    uint64 index = NoiseTool::Index(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
    waters.Emplace(index);

    //垂流：检测下方踏空
    index = NoiseTool::Index(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k-1);
    if(waters.Find(index)){return;}
    auto result = info.GolbalBlocksID.Find(index);
    if(!result||(*result)==0){
        flow(chunk,info,rain-0.3f,i,j,k-1);
        return;
    }

    const int32 dx[4] = {1,-1,0,0};
    const int32 dy[4] = {0,0,1,-1};

    for(int d = 0;d<4;++d){
        int32 x = i+dx[d];
        int32 y = j+dy[d];
        index = NoiseTool::Index(chunk.ChunkPosition.X*16+x,chunk.ChunkPosition.Y*16+y,k);
        if(waters.Find(index))continue;

        result = info.GolbalBlocksID.Find(index);
        //水平流向无障碍
        if(!result||(*result)==0){
            index = NoiseTool::Index(chunk.ChunkPosition.X*16+x,chunk.ChunkPosition.Y*16+y,k-1);
            if(waters.Find(index)){continue;}

            result = info.GolbalBlocksID.Find(index);
            if(!result||(*result)==0){
                flow(chunk,info,rain-1.0f,x,y,k-1);
            }
            else{
                flow(chunk,info,rain-1.0f,x,y,k);
            }
        }
    }

    for(uint64 index:waters){
        auto vec = NoiseTool::UnIndex(index);
        index = NoiseTool::Index(vec.X,vec.Y,vec.Z-1);
        info.GolbalBlocksID.Emplace(index,9);
        chunk.BlocksID.Emplace(index,9);
    }
}
