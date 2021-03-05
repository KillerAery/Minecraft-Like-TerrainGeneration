// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeGenerator.h"
#include "Tool/NoiseTool.h"

void TreeGenerator::GenerateTree(Chunk& chunk,GlobalInfo& info){
    const int32 cystalSize = 16;
    int32 seedOffset=NoiseTool::hash21(chunk.ChunkPosition);

	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize,1);
    NoiseTool::setSeed(1317+seedOffset);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
        /*
	    None = 0
	    雪地 Snow = 1
	    草地 Green = 2
	    泥地 Dry = 3
        石地 Stone = 4
	    沙漠 Desert = 5
        */
        if(chunk.BlocksBiome[i][j]==0||chunk.BlocksBiome[i][j]==4||chunk.BlocksBiome[i][j]==5)continue;
        //被挖空就无法生成
        if(info.SpecialBlocksID.Find(NoiseTool::Index(
            chunk.ChunkPosition.X*16+i,
            chunk.ChunkPosition.Y*16+j,
            chunk.BlocksHeight[i][j]))
        )continue;
        //------------------
        //----生成树
        if(GenerateTree(chunk,info,i,j,cystalSize))continue;
        //-----------------
        //----生成草
        if(GenerateFlower(chunk,info,i,j,cystalSize))continue;
	}
}

bool TreeGenerator::GenerateFlower(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize){
        if(chunk.BlocksHeight[i][j] <=79)return false;
        
		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);

        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];
		float possible = NoiseTool::rand(pf) - FMath::Abs(temperature+0.1f)*0.4f + humidity*0.4f;
        int32 targetID = 0;
        //若满足概率（花类）：20黄花、21蓝花、22蘑菇、23蘑菇
        if(possible > 1.0f){
            targetID = 20+NoiseTool::randInt(FVector2D(i+j*21,j-i^2))%4;
        }
        //若满足概率（草类）：11绿草 12黄草，13白草
        else if(possible > 0.85f){
            if(temperature>0.3f){targetID = 12;}
            else if(temperature>-0.3f){targetID = 11;}
            else{targetID = 13;}
        }

        if(targetID==0)return false;
        
        AddBlockWithIndex(chunk,info,i,j,chunk.BlocksHeight[i][j]+1,targetID);

        return true;
}

bool TreeGenerator::GenerateTree(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize){
        if(chunk.BlocksHeight[i][j]<=79-1)return false;
        
		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];
		float possible = ((NoiseTool::simplexNoise(pf)+1.0f)/2.0f)*0.15f - FMath::Abs(temperature+0.1f)*0.10f + humidity*0.15f + NoiseTool::rand(-pf)*0.9f;
            
        //若满足概率
        if(possible < 0.985f)return false;
        //树干高度
        int32 treeHeight = (NoiseTool::randInt(FVector2D::UnitVector-pf)%3)+4;

        //根据温度选择树类型
        temperature += (NoiseTool::rand(FVector2D::UnitVector+pf)-0.5f)*0.2f;
        //方块ID：
        //14温带木 15热带木，16寒带木
        //17温带树叶 18寒带树叶 19热带树叶
        int32 targetWoodID;
        int32 targetLeafID;
        if(temperature>0.3f){targetWoodID = 15;targetLeafID = 19;}
        else if(temperature>-0.3f){targetWoodID = 14;targetLeafID = 17;}
        else{targetWoodID = 16;targetLeafID = 18;}

                
        for(int k = 0;k<treeHeight;++k){
            //生成树干
            AddBlockWithIndex(chunk,info,i,j,chunk.BlocksHeight[i][j]+1+k,targetWoodID);
        }

        int32 t1 = NoiseTool::rand(17*pf)*4+1.5f+int32(treeHeight>=5);
        int32 t2 = NoiseTool::rand(11*pf)*4+1.5f+int32(treeHeight>=5);
        int32 leafHeight = treeHeight+1+t1%3;
        int32 initLeafHeight = 2+t2%2;
        for(int k = leafHeight-1;k >= initLeafHeight ;--k){
            //生成树叶
            GenerateLeaves(chunk,info,i,j,chunk.BlocksHeight[i][j]+1+k,
                //贝塞尔曲线计算树叶
                NoiseTool::bezier(
                    FVector2D(0,0)
                    ,FVector2D(0.33f,t1)
                    ,FVector2D(0.66f,t2)
                    ,FVector2D(1,0)
                    ,float(k-initLeafHeight)/(leafHeight-1-initLeafHeight)).Y
            ,targetLeafID);
        }

    return true;
}

void TreeGenerator::GenerateLeaves(Chunk& chunk,GlobalInfo& info,int32 x,int32 y,int32 height,int32 radius,int32 targetLeafID){
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
        if(!leavesTemplate[radius][i][j])
            continue;
        
        int32 dx = x+i-2;
        int32 dy = y+j-2;
        int32 gx = x+i-2+chunk.ChunkPosition.X;
        int32 gy = y+j-2+chunk.ChunkPosition.Y;

        if(info.SpecialBlocksID.Find(NoiseTool::Index(gx,gy,height)))
            continue;

        AddBlockWithIndex(chunk,info,dx,dy,height,targetLeafID);

        //树顶雪
        if(chunk.BlocksBiome[x][y]==1 && NoiseTool::rand(FVector2D(dx+height*11,dy*17+radius*23))>0.13f){
            AddBlockWithIndex(chunk,info,dx,dy,height+1,24);
        }
    }
}


void TreeGenerator::AddBlockWithIndex(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 height,int32 targetID){
    uint64 index = NoiseTool::Index(
        chunk.ChunkPosition.X*16+i,
        chunk.ChunkPosition.Y*16+j,
        height);

    if(!info.SpecialBlocksID.Find(index))
        info.SpecialBlocksID.Emplace(index,targetID);
}