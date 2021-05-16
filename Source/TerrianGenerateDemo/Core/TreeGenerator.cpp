// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeGenerator.h"
#include "Tool/NoiseTool.h"
#include "Model/Biome.h"

void PlantGenerator::GeneratePlant(Chunk& chunk,GlobalInfo& info){
    const int32 cystalSize = 16;
    int32 seedOffset = NoiseTool::hash21(chunk.ChunkPosition);

	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition,cystalSize);
    NoiseTool::setSeed(1317+seedOffset);

	for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
        //不允许在空地、石头、沙漠上产生植被
        if( chunk.BlocksBiome[i][j]==static_cast<int32>(BiomeType::None)||
            chunk.BlocksBiome[i][j]==static_cast<int32>(BiomeType::Stone)||
            chunk.BlocksBiome[i][j]==static_cast<int32>(BiomeType::Desert))
        {
            continue;
        }
        
        int32* result;
        //查询地板方块ID
        result = info.FindBlock(FVector(
                chunk.ChunkPosition.X*16+i,
                chunk.ChunkPosition.Y*16+j,
                chunk.BlocksHeight[i][j])
        );
        //被挖空（方块ID为0）或者方块ID为9（方块为水）就无法生成
        if(result && (*result == 0 || *result == 9)){
            continue;
        }
        
        //查询地板上一格的方块ID
        result = info.FindBlock(FVector(
            chunk.ChunkPosition.X*16+i,
            chunk.ChunkPosition.Y*16+j,
            chunk.BlocksHeight[i][j]+1));
        //如果存在方块，则无法生成
        if(result){
            continue;
        }

        //生成树
        if(GenerateTree(chunk,info,i,j,cystalSize))continue;
        
        //生成草
        if(GenerateFlower(chunk,info,i,j,cystalSize))continue;
	}
}

bool PlantGenerator::GenerateFlower(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize){
        //如果地板方块在水平面以下，则不允许生成花朵
        if(chunk.BlocksHeight[i][j] <= SeaLevel)
            return false;
        
        //获得柱块的温度值、湿度值
        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];

		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
        //计算概率值
		float possible = NoiseTool::rand(pf) - FMath::Abs(temperature+0.1f)*0.4f + humidity*0.4f;

        int32 targetID = 0;
        //若满足生成花的概率
        if(possible > 1.0f){
            //方块ID：20=黄花、21=蓝花、22=蘑菇、23=蘑菇
            targetID = 20 + NoiseTool::randInt(FVector2D(i+j*21,j-i^2))%4;
        }
        //若满足生成草的概率
        else if(possible > 0.85f){
            //方块ID：11=绿草 12=黄草，13=白草
            if(temperature>0.3f){targetID = 12;}
            else if(temperature>-0.3f){targetID = 11;}
            else{targetID = 13;}
        }
        if(targetID==0)
            return false;
        
        //添加目标方块（花或草）    
        info.AddBlock(FVector(
            chunk.ChunkPosition.X*16+i,
            chunk.ChunkPosition.Y*16+j,
            chunk.BlocksHeight[i][j]+1),targetID);

        return true;
}

bool PlantGenerator::GenerateTree(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize){
        //如果地板方块在水平面下一格的以下，则不允许生成花朵
        if(chunk.BlocksHeight[i][j] <= SeaLevel-1)return false;
        
		FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];
		float possible = (NoiseTool::simplexNoise(pf)+1.0f)/2.0f*0.15f - FMath::Abs(temperature+0.1f)*0.10f + humidity*0.15f + NoiseTool::rand(-pf)*0.9f;
            
        //若满足概率
        if(possible < 0.985f)return false;
        //树干高度
        int32 treeHeight = (NoiseTool::randInt(FVector2D::UnitVector-pf)%3)+4;

        //根据温度选择树类型
        temperature += (NoiseTool::rand(FVector2D::UnitVector+pf)-0.5f)*0.2f;

        int32 targetWoodID;
        int32 targetLeafID;
        //方块ID：14=温带木 15=热带木，16=寒带木，17=温带树叶 18=寒带树叶，19=热带树叶
        if(temperature>0.3f){targetWoodID = 15;targetLeafID = 19;}
        else if(temperature>-0.3f){targetWoodID = 14;targetLeafID = 17;}
        else{targetWoodID = 16;targetLeafID = 18;}
         
        for(int k = 0;k<treeHeight;++k){
            //生成目标方块（树干）    
            info.AddBlock(FVector(
                chunk.ChunkPosition.X*16+i,
                chunk.ChunkPosition.Y*16+j,
                chunk.BlocksHeight[i][j]+1+k),targetWoodID);
        }

        int32 t1 = NoiseTool::rand(17*pf)*4+1.5f+int32(treeHeight>=5);
        int32 t2 = NoiseTool::rand(11*pf)*4+1.5f+int32(treeHeight>=5);
        int32 leafHeight = treeHeight+1+t1%3;
        int32 initLeafHeight = 2+t2%2;
        for(int k = leafHeight-1;k >= initLeafHeight;--k){
            //贝塞尔曲线计算树叶半径
            float leafRadius = 
                NoiseTool::bezier(
                    FVector2D(0,0)
                    ,FVector2D(0.33f,t1)
                    ,FVector2D(0.66f,t2)
                    ,FVector2D(1,0)
                    ,float(k-initLeafHeight)/(leafHeight-1-initLeafHeight)).Y;

            //生成树叶
            GenerateLeaves(chunk,info,i,j,chunk.BlocksHeight[i][j]+1+k,leafRadius,targetLeafID);
        }

    return true;
}

void PlantGenerator::GenerateLeaves(Chunk& chunk,GlobalInfo& info,int32 x,int32 y,int32 height,int32 radius,int32 targetLeafID){
    radius = FMath::Clamp(radius,0,3);

    for(int i =0;i<5;++i)
    for(int j =0;j<5;++j){
        if(!leavesTemplate[radius][i][j])
            continue;
        
        int32 dx = x+i-2;
        int32 dy = y+j-2;
        int32 gx = dx+chunk.ChunkPosition.X*16;
        int32 gy = dy+chunk.ChunkPosition.Y*16;

        if(info.FindBlock(FVector(gx,gy,height)))
            continue;

        info.AddBlock(FVector(gx,gy,height),targetLeafID);

        //树顶雪
        if(chunk.BlocksBiome[x][y]==1 && NoiseTool::rand(FVector2D(gx+height*11,gy*17+radius*23))>0.13f){
            info.AddBlock(FVector(gx,gy,height+1),24);
        }
    }
}

bool PlantGenerator::leavesTemplate[4][5][5] = {
{   {0,0,0,0,0},
    {0,0,1,0,0},
    {0,1,1,1,0},
    {0,0,1,0,0},
    {0,0,0,0,0}},

{   {0,0,0,0,0},
    {0,1,1,1,0},
    {0,1,1,1,0},
    {0,1,1,1,0},
    {0,0,0,0,0}},

{   {0,1,1,1,0},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {0,1,1,1,0}},

{   {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1},
    {1,1,1,1,1}}
};