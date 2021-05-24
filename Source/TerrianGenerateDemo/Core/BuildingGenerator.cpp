// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGenerator.h"
#include <queue>
#include "GameFramework/Actor.h"
#include "Tool/NoiseTool.h"
#include "Tool/PathFinder.h"


TSet<uint64> BuildingGenerator::domains={};	

TSet<uint64> BuildingGenerator::roads={};

FVector2D BuildingGenerator::startPoint=FVector2D::ZeroVector;

TArray<FVector2D> BuildingGenerator::buildingPos={};

void BuildingGenerator::GenerateBuilding(Chunk& chunk,GlobalInfo& info){
    if(NoiseTool::rand(chunk.ChunkPosition)<0.98f)return;

    //domains.Reset();

    DevelopeDomains(chunk,info);
    PlaceBuildings(chunk,info);
    PlacePaths(chunk,info);
}

void BuildingGenerator::DevelopeDomains(Chunk& chunk,GlobalInfo& info){

    startPoint = FVector2D(chunk.ChunkPosition.X*16+7,chunk.ChunkPosition.Y*16+7);

    std::priority_queue<
        std::pair<float,FVector2D>,
        std::vector<std::pair<float,FVector2D>>,
        std::greater<std::pair<float,FVector2D>>
    > s;
    s.emplace(0,startPoint);

    int32 count = 0;
    const int32 dx[9] = {1,-1,0,0,1,-1,1,-1,0};
    const int32 dy[9] = {0,0,1,-1,1,-1,-1,1,0};
    
    UE_LOG(LogTemp, Warning, TEXT("develop begin"));

    while(!s.empty()){
        count++;
        float cost = s.top().first;
        FVector2D p = s.top().second;
        s.pop();

        for(int d = 0;d<9;++d)
        domains.Emplace(NoiseTool::Index(p.X+dx[d],p.Y+dy[d]));

        //设置最高发展度费用
        if(cost > 7)break;

        int32 centerHeight = info.GetHeight(p.X,p.Y);

        const int32 dx[4] = {1,-1,0,0};
        const int32 dy[4] = {0,0,1,-1};
        
        for(int d = 0;d<4;++d){
            int32 x = p.X+dx[d]*3;
            int32 y = p.Y+dy[d]*3;

            if(domains.Find(NoiseTool::Index(x,y)))continue;

            int32 height = info.GetHeight(x,y);
            if(height<=SeaLevel||height>1000){continue;}

            int32 deltaheight = FMath::Abs(centerHeight-height);

            s.emplace(cost+0.5f+deltaheight*0.5f,FVector2D(x,y));
        }
    }

	UE_LOG(LogTemp, Warning, TEXT("devlop %d"),count);
}

void BuildingGenerator::PlaceBuildings(Chunk& chunk,GlobalInfo& info){
    //[第几个建筑][长、宽]
    const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};
    const int32 dx[4] = {1,-1,0,0};
    const int32 dy[4] = {0,0,1,-1};

    int32 count = 0;

    std::queue<FVector2D> q;
    q.push(startPoint);

    while(!q.empty()){
        ++count;

        auto pos = q.front();
        q.pop();

        int32 index = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*107)%3;
        int32 rotate = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*17)%4;
        
        bool test = PlaceOneBuilding(info,pos.X,pos.Y,index,rotate) || PlaceOneBuilding(info,pos.X,pos.Y,index,(rotate+1)%4);
        if(!test){
            continue;
        }   

        int32 offset = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*67)%3+5;
        int32 offsetX = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,count)*61)%5-2;
        int32 offsetY = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(-count,count)*117)%5-2;
        
        for(int i = 0;i<4;++i){
            q.push(FVector2D
            (pos.X+dx[i]*(offset+buildingSize[index][0])+offsetX,
            pos.Y+dy[i]*(offset+buildingSize[index][1])+offsetY));
        }
    }
}


bool BuildingGenerator::PlaceOneBuilding(GlobalInfo& info,int32 x,int32 y,int32 index,int32 rotate){
    //[第几个建筑][长、宽]
    const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};

    int rotateIndex = rotate%2;
    int updown = buildingSize[index][rotateIndex]/2;
    int leftright = buildingSize[index][!rotateIndex]/2;

    float aver = 0;
    for(int i=-updown;i<updown;++i)
    for(int j=-leftright;j<leftright;++j)
    {   
        //若不在发展域，则无需生成建筑         
        if(!domains.Find(NoiseTool::Index(x+i,y+j))){return false;}
        int32 h = info.GetHeight(x+i,y+j);
        aver += h;
    }
    
    aver /= (buildingSize[index][0]*buildingSize[index][1]);
    aver = floor(aver+0.5f);

    //低于海平面，没必要生成房屋
    if(aver<=SeaLevel)return false;

    for(int i=-updown;i<updown;++i)
    for(int j=-leftright;j<leftright;++j)
    {
       info.SetHeight(x+i,y+j,aver);
       domains.Remove(NoiseTool::Index(x+i,y+j));
    }

    //地表插入空气，以免生成树木花草
    for(int i=-updown-1;i<updown+1;++i)
    for(int j=-leftright-1;j<leftright+1;++j)
    {   
        FVector pos = FVector(x+i,y+j,info.GetHeight(x+i,y+j)+1);
        info.AddBlock(pos,0);
    }

    info.AddBuilding(FVector(x,y,aver+1),index+1,rotate);
    domains.Emplace(NoiseTool::Index(x-updown,y-leftright));
    buildingPos.Push(FVector2D(x-updown,y-leftright));
    return true;
}

void BuildingGenerator::PlacePaths(Chunk& chunk,GlobalInfo& info){
    PathFinder::setConditionInBarrier(inBarrier);
    PathFinder::setWeightFormula(weightFormula);
    int n = buildingPos.Num();
    //两两寻路，进行道路连接
    for(int i = 0;i<n;++i)
    for(int j = i+1;j<n;++j)
    {
        auto path = PathFinder::findPath(buildingPos[i],buildingPos[j]);

	    UE_LOG(LogTemp, Warning, TEXT("path roads num = %d"),path.Num());

        for(FVector2D pos : path){
            roads.Emplace(NoiseTool::Index(pos.X,pos.Y));
            info.AlterBlock(FVector(pos.X,pos.Y,info.GetHeight(pos.X,pos.Y)),5);
        }
    }
    buildingPos.Reset();
}

bool BuildingGenerator::inBarrier(FVector2D pos){
    return !domains.Contains(NoiseTool::Index(pos.X,pos.Y));
}
	

TPair<float,float> BuildingGenerator::weightFormula(FVector2D pos,FVector2D endPos,float cost){
    if(roads.Contains(NoiseTool::Index(pos.X,pos.Y))){
        cost -= 0.5f;
    }

    FVector2D dist = (endPos-pos).GetAbs();
    float predict = (dist.X+dist.Y)*1.41f - FMath::Max(dist.X,dist.Y)*0.41f + cost;
    
    return TPair<float,float>(cost,predict);
}