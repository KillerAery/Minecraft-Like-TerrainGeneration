// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGenerator.h"
#include "GameFramework/Actor.h"
#include <queue>
#include "Tool/NoiseTool.h"


TSet<uint64> BuildingGenerator::domains={};
FVector2D BuildingGenerator::startPoint=FVector2D::ZeroVector;

void BuildingGenerator::GenerateBuilding(Chunk& chunk,GlobalInfo& info){
    if(NoiseTool::rand(chunk.ChunkPosition)<0.8f)return;

    PlaceBuilding(info,chunk.ChunkPosition.X*16+7,chunk.ChunkPosition.Y*16+7,NoiseTool::randInt(chunk.ChunkPosition)%3,NoiseTool::randInt(chunk.ChunkPosition+FVector2D::UnitVector)%4);
    //GenerateDomains(chunk,info);
    //GenerateBuildings(chunk,info);
    //GeneratePaths(chunk,info);
}

void BuildingGenerator::GenerateDomains(Chunk& chunk,GlobalInfo& info){
    std::priority_queue<std::pair<float,uint64>> s;

    int32 maxGradient = INT_MAX;   
    for(int i =1;i<15;++i)
    for(int j =1;j<15;++j)
    {
        float aver = (
            chunk.BlocksHeight[i-1][j]+
            chunk.BlocksHeight[i+1][j]+
            chunk.BlocksHeight[i][j-1]+
            chunk.BlocksHeight[i][j+1]);
        float dt = FMath::Abs((chunk.BlocksHeight[i-1][j]-aver))+
         FMath::Abs((chunk.BlocksHeight[i+1][j]-aver))+
          FMath::Abs((chunk.BlocksHeight[i][j-1]-aver))+
           FMath::Abs((chunk.BlocksHeight[i][j+1]-aver));
        if(maxGradient<dt){
            maxGradient = dt;
            startPoint = FVector2D(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j);
        }
    }

    NoiseTool::setSeed(21854);
    s.emplace(0,NoiseTool::Index(startPoint.X,startPoint.Y));

    while(!s.empty()){
        domains.Emplace(s.top().second);
        FVector p = NoiseTool::UnIndex(s.top().second);
        float cost = s.top().first;

        //设置最高发展度费用
        if(cost > 6)return;
        s.pop();

        const int32 dx[4] = {1,-1,0,0};
        const int32 dy[4] = {0,0,1,-1};

        for(int d = 0;d<4;++d){
            int32 x = p.X+dx[d];
            int32 y = p.Y+dy[d];

            uint64 index = NoiseTool::Index(x,y);
            if(domains.Find(index))continue;

            int32 height = info.GetHeight(x,y);
            if(height<=SeaLevel)continue;

            int32 deltaheight = FMath::Abs(info.GetHeight(p.X,p.Y)-height);
            s.emplace(cost+deltaheight*deltaheight+0.15f,index);
        }
    }
}

void BuildingGenerator::GenerateBuildings(Chunk& chunk,GlobalInfo& info){
    //[第几个建筑][长、宽]
    const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};

    static int32 count = 1;
    int32 index = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*107)%3;
    ++count;

    for(int i=-buildingSize[index][0]/2;i<buildingSize[index][0]/2;++i)
    for(int j=-buildingSize[index][1]/2;j<buildingSize[index][1]/2;++j)
    {
        //if(!domains.Find()){
            
        //}
    }


}


void BuildingGenerator::PlaceBuilding(GlobalInfo& info,int32 x,int32 y,int32 index,int32 rotate){
    //[第几个建筑][长、宽]
    const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};

    int rotateIndex = rotate%2;
    int updown = buildingSize[index][rotateIndex]/2;
    int leftright = buildingSize[index][!rotateIndex]/2;

    float aver = 0;
    for(int i=-updown;i<updown;++i)
    for(int j=-leftright;j<leftright;++j)
    {
        int32 h = info.GetHeight(x+i,y+j);
        //auto result = info.GolbalBlocksID.Find(NoiseTool::Index(x+i,y+j,h));
        ////地面被挖空，不用生成房屋
        //if(result&&*result==0)return;
        aver += h;
    }
    
    aver/=(buildingSize[index][0]*buildingSize[index][1]);
    aver = floor(aver+0.5f);

    //低于海平面，没必要生成房屋
    if(aver<=SeaLevel)return;

    for(int i=-updown;i<updown;++i)
    for(int j=-leftright;j<leftright;++j)
    {
       info.SetHeight(x+i,y+j,aver);
    }

    //地表插入空气，以免生成树木花草
    for(int i=-updown-1;i<updown+1;++i)
    for(int j=-leftright-1;j<leftright+1;++j)
    {   
        info.GolbalBlocksID.Emplace(
           NoiseTool::Index(x+i,y+j,info.GetHeight(x+i,y+j)+1),
           0
       );
    }
    
    info.GolbalBudildings.Push(TTuple<uint64,int32,int32>(NoiseTool::Index(x,y,aver+1),index+1,rotate));
}

void BuildingGenerator::GeneratePaths(Chunk& chunk,GlobalInfo& info){

}
