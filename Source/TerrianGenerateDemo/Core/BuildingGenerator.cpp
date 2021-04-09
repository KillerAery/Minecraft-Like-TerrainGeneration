// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGenerator.h"
#include "GameFramework/Actor.h"
#include <queue>
#include "Tool/NoiseTool.h"


TSet<uint64> BuildingGenerator::domains={};
FVector2D BuildingGenerator::startPoint=FVector2D::ZeroVector;

void BuildingGenerator::GenerateBuilding(Chunk& chunk,GlobalInfo& info){
    if(NoiseTool::rand(chunk.ChunkPosition)<0.98f)return;

    //domains.Reset();

    GenerateDomains(chunk,info);
    GenerateBuildings(chunk,info);
    //PlaceBuilding(info,chunk.ChunkPosition.X*16+8,chunk.ChunkPosition.Y*16+8,0,0);
    //GeneratePaths(chunk,info);
}

void BuildingGenerator::GenerateDomains(Chunk& chunk,GlobalInfo& info){
    float maxGradient = 10000;   
    for(int i =1;i<15;++i)
    for(int j =1;j<15;++j)
    {
        float aver = (
            chunk.BlocksHeight[i-1][j]+
            chunk.BlocksHeight[i+1][j]+
            chunk.BlocksHeight[i][j-1]+
            chunk.BlocksHeight[i][j+1])/4.0f;
        float dt = FMath::Abs((chunk.BlocksHeight[i-1][j]-aver))+
         FMath::Abs((chunk.BlocksHeight[i+1][j]-aver))+
          FMath::Abs((chunk.BlocksHeight[i][j-1]-aver))+
           FMath::Abs((chunk.BlocksHeight[i][j+1]-aver));
        if(dt<maxGradient){
            maxGradient = dt;
            startPoint = FVector2D(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j);
        }
    }

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
        if(cost > 8)break;

        int32 centerHeight = info.GetHeight(p.X,p.Y);
        if(centerHeight<5||centerHeight>1000) 
        {
            continue;
        }

        const int32 dx[4] = {1,-1,0,0};
        const int32 dy[4] = {0,0,1,-1};
        
        for(int d = 0;d<4;++d){
            int32 x = p.X+dx[d]*3;
            int32 y = p.Y+dy[d]*3;

            if(domains.Find(NoiseTool::Index(x,y)))continue;

            int32 height = info.GetHeight(x,y);
            if(height<=SeaLevel||height>1000)continue;

            int32 deltaheight = FMath::Abs(centerHeight-height);

            s.emplace(cost+0.5f+deltaheight*0.5f,FVector2D(x,y));
        }
    }

	UE_LOG(LogTemp, Warning, TEXT("devlop %d"),count);
}

void BuildingGenerator::GenerateBuildings(Chunk& chunk,GlobalInfo& info){
    //[第几个建筑][长、宽]
    const int32 buildingSize[3][2]={{10,6},{8,6},{6,6}};
    const int32 dx[4] = {1,-1,0,0};
    const int32 dy[4] = {0,0,1,-1};

    int32 count = 0;

    std::queue<FVector2D> q;
    q.push(startPoint);

    UE_LOG(LogTemp, Warning, TEXT("buildings begin"));
    while(!q.empty()){
        ++count;

        auto pos = q.front();
        q.pop();

        int32 index = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*107)%3;
        int32 rotate = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*17)%4;
        
        bool test = PlaceBuilding(info,pos.X,pos.Y,index,rotate);
        if(!test) test = PlaceBuilding(info,pos.X,pos.Y,index,(rotate+1)%4);

        int32 offset = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,-count)*67)%3+5;
        int32 offsetX = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,count)*61)%3-1;
        int32 offsetY = NoiseTool::randInt(chunk.ChunkPosition+FVector2D(count,count)*117)%3-1;

        if(test){
            for(int i = 0;i<4;++i){
                q.push(FVector2D
                (pos.X+dx[i]*(offset+buildingSize[index][0])+offsetX,
                pos.Y+dy[i]*(offset+buildingSize[index][1])+offsetY));
            }
	        UE_LOG(LogTemp, Warning, TEXT("Your message 2134214"));
        }
        else{
	        UE_LOG(LogTemp, Warning, TEXT("Your message NNNNNNNNNNNNNNNNNNNNNNNN"));
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("buildings %d"),count);
}


bool BuildingGenerator::PlaceBuilding(GlobalInfo& info,int32 x,int32 y,int32 index,int32 rotate){
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
        //auto result = info.GolbalBlocksID.Find(NoiseTool::Index(x+i,y+j,h));
        ////地面被挖空，不用生成房屋
        //if(result&&*result==0)return false;
        aver += h;
    }
    
    aver/=(buildingSize[index][0]*buildingSize[index][1]);
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
        info.GolbalBlocksID.Emplace(NoiseTool::Index(x+i,y+j,info.GetHeight(x+i,y+j)+1),0);
    }
    
    info.GolbalBudildings.Push(TTuple<uint64,int32,int32>(NoiseTool::Index(x,y,aver+1),index+1,rotate));
    return true;
}

void BuildingGenerator::GeneratePaths(Chunk& chunk,GlobalInfo& info){

}
