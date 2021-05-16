// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalInfo.h"
#include "Tool/NoiseTool.h"

GlobalInfo::GlobalInfo()
{
}

GlobalInfo::~GlobalInfo()
{
}

int32 GlobalInfo::GetHeight(int32 x,int32 y){
   int32 xi = floor(x/16.0f);
   int32 yi = floor(y/16.0f);

   auto height = GlobalHeight.Find(NoiseTool::Index(xi,yi));
   return height?(*height)[x-xi*16][y-yi*16]:-1;
}

void GlobalInfo::SetChunkHeight(int32 x,int32 y,HeightMap heightmap){
    GlobalHeight.Emplace(NoiseTool::Index(x,y),heightmap);
}
	
void GlobalInfo::SetHeight(int32 x,int32 y,int32 height){
   int32 xi = floor(x/16.0f);
   int32 yi = floor(y/16.0f);

   auto map = GlobalHeight.Find(NoiseTool::Index(xi,yi));
   if(map)
      (*map)[x-xi*16][y-yi*16] = height;
}

void GlobalInfo::AddBlock(FVector pos,int32 BlockID){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   GlobalBlocksID.Emplace(index,BlockID);
   Blocks2Display.Emplace(index,BlockID);
}


void GlobalInfo::AddBlockWithoutDisplay(FVector pos,int32 BlockID){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   GlobalBlocksID.Emplace(index,BlockID);
}

//添加仅显示方块
void GlobalInfo::AddBlockOnlyDisplay(FVector pos,int32 BlockID){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   Blocks2Display.Emplace(index,BlockID);
}

void GlobalInfo::RemoveBlock(FVector pos){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   GlobalBlocksID.Emplace(index,0);
}

int32* GlobalInfo::FindBlock(FVector pos){
   int32* result = GlobalBlocksID.Find(NoiseTool::Index(
            pos.X,
            pos.Y,
            pos.Z));
   return result;
}

TArray<TPair<uint64,int32>>& GlobalInfo::GetBlocks2Display(){
   return Blocks2Display;
}

void GlobalInfo::AddBuilding(FVector pos,int32 BuildingID,int32 rotate){
   Budildings2Display.Emplace(TTuple<uint64,int32,int32>(NoiseTool::Index(pos.X,pos.Y,pos.Z),BuildingID,rotate));
}

void GlobalInfo::RemoveBuilding(FVector pos){
   //TODO
}

TArray<TTuple<uint64,int32,int32>>& GlobalInfo::GetBuildings2Display(){
   return Budildings2Display;
}