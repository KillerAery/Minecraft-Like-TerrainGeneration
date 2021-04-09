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

   auto height = GolbalHeight.Find(NoiseTool::Index(xi,yi));
   return height?(*height)[x-xi*16][y-yi*16]:-1;
}

void GlobalInfo::SetChunkHeight(int32 x,int32 y,HeightMap heightmap){
    GolbalHeight.Emplace(
        NoiseTool::Index(x,y),
        heightmap);
}
	
void GlobalInfo::SetHeight(int32 x,int32 y,int32 height){
   int32 xi = floor(x/16.0f);
   int32 yi = floor(y/16.0f);

   auto map = GolbalHeight.Find(NoiseTool::Index(xi,yi));
   if(map)
      (*map)[x-xi*16][y-yi*16] = height;
}

void GlobalInfo::AddBlock(FVector pos,int32 BlockID){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   GolbalBlocksID.Emplace(index,BlockID);
}

void GlobalInfo::RemoveBlock(FVector pos){
   uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
   GolbalBlocksID.Emplace(index,0);
}
	
void GlobalInfo::AddBuilding(FVector pos,int32 BlockID){

}

void GlobalInfo::RemoveBuilding(FVector pos){

}