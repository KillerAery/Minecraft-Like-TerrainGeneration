// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeGenerator.h"
#include "Tool/NoiseTool.h"

void TreeGenerator::GenerateTree(Chunk& chunk){
    const int32 cystalSize = 16;
    int32 seedOffset=NoiseTool::hash21(chunk.ChunkPosition);

	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize,1);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
        int32 height = chunk.BlocksHeight[i][j];
        //-----------------
        //----生成草
        if(height >79){
            NoiseTool::setSeed(117+seedOffset);
            float valueTemperature = chunk.BlocksTemperature[i][j];
		    float possible = NoiseTool::rand(pf) - FMath::Abs(valueTemperature+0.1f)*0.22f;
        
            //若满足概率
            if(possible > 0.93f){
                valueTemperature += (NoiseTool::rand(FVector2D::UnitVector+pf)-0.5f)*0.2f;
                //方块ID：
                //11绿草 12黄草，13白草
                int32 targetID;
                if(valueTemperature>0.3f){targetID = 12;}
                else if(valueTemperature>-0.3f){targetID = 11;}
                else{targetID = 13;}

                chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(i,j,chunk.BlocksHeight[i][j]+1),targetID);
            }
        }
        //------------------
        //----生成树
        if(height>79-1){
            NoiseTool::setSeed(117-seedOffset);

            float valueTemperature = chunk.BlocksTemperature[i][j];
            float valueHumidity = chunk.BlocksHumidity[i][j];
		    float possible = ((NoiseTool::simplexNoise(pf)+1.0f)/2.0f)*0.15f - FMath::Abs(valueTemperature+0.1f)*0.10f + valueHumidity*0.15f + NoiseTool::rand(pf)*0.9f;
            
            //若满足概率
            if(possible > 0.985f){
                //树干高度
                int32 treeHeight = (NoiseTool::randInt(FVector2D::UnitVector-pf)%3)+4;

                //根据温度选择树类型
                valueTemperature += (NoiseTool::rand(FVector2D::UnitVector+pf)-0.5f)*0.2f;
                //方块ID：
                //14温带木 15热带木，16寒带木
                //17温带树叶 18寒带树叶 19热带树叶
                int32 targetWoodID;
                int32 targetLeafID;
                if(valueTemperature>0.3f){targetWoodID = 15;targetLeafID = 19;}
                else if(valueTemperature>-0.3f){targetWoodID = 14;targetLeafID = 17;}
                else{targetWoodID = 16;targetLeafID = 18;}

                
                for(int k = 0;k<treeHeight;++k){
                    //生成树干
                    chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(i,j,chunk.BlocksHeight[i][j]+1+k),targetWoodID);
                }

                int32 t1 = NoiseTool::randInt(17*pf)%6+int32(treeHeight>=5);
                int32 t2 = NoiseTool::randInt(11*pf)%6+int32(treeHeight>=5);
                int32 leafHeight = treeHeight+1+t1%3;
                int32 initLeafHeight = 2+t2%2;
                for(int k = initLeafHeight;k < leafHeight;++k){
                    //生成树叶
                    GenerateLeaves(chunk,i,j,chunk.BlocksHeight[i][j]+1+k,
                        NoiseTool::bezier(
                        FVector2D(0,0)
                        ,FVector2D(0.33f,t1)
                        ,FVector2D(0.66f,t2)
                        ,FVector2D(1,0)
                        ,float(k-initLeafHeight)/(leafHeight-1-initLeafHeight)).Y
                    ,targetLeafID);
                }
            }
        }
	}
}



void TreeGenerator::GenerateLeaves(Chunk& chunk,int32 x,int32 y,int32 height,int32 radius,int32 targetLeafID){
    const bool leavesTemplate[4][5][5] = {
        {
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,1,1,1,0},
            {0,0,1,0,0},
            {0,0,0,0,0}
        },
        {
            {0,0,0,0,0},
            {0,1,1,1,0},
            {0,1,1,1,0},
            {0,1,1,1,0},
            {0,0,0,0,0}
        },
        {
            {0,1,1,1,0},
            {1,1,1,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1},
            {0,1,1,1,0}
        },
        {
            {1,1,1,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1}
        }
    };

    radius = FMath::Clamp(radius,0,3);

    for(int i =0;i<5;++i)
    for(int j =0;j<5;++j){
        if(!leavesTemplate[radius][i][j])continue;
        int32 dx = x+i-2;
        int32 dy = y+j-2;

        if(chunk.BlocksID.ContainsByPredicate(
            [dx,dy,height](TPair<TTuple<int32,int32,int32>,int32>& p){
                auto& t = p.Get<0>();
                return t.Get<0>() == dx && t.Get<1>() == dy && t.Get<2>() == height;
            })
        )
        {
            continue;
        }

        chunk.BlocksID.Emplace(TTuple<int32,int32,int32>(dx,dy,height),targetLeafID);
    }
}