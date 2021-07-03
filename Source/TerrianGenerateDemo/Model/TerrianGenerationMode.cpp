// Fill out your copyright notice in the Description page of Project Settings.
#include "Model/TerrianGenerationMode.h"

#include "Core/HeightGenerator.h"
#include "Core/CaveGenerator.h"
#include "Core/TemperatureGenerator.h"
#include "Core/HumidityGenerator.h"
#include "Core/BiomeGenerator.h"
#include "Core/TreeGenerator.h"
#include "Core/RainGenerator.h"
#include "Core/BuildingGenerator.h"

#include "Tool/NoiseTool.h"
#include "TerrianGenerateDemoHUD.h"
#include "TerrianGenerateDemoCharacter.h"


ATerrianGenerationMode::ATerrianGenerationMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATerrianGenerateDemoHUD::StaticClass();
}

void ATerrianGenerationMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrianGenerationMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATerrianGenerationMode::SetCameraLoaction(FVector location)
{
	ChunksCenterPosition = FVector2D(
		floor(location.X / (MaxBlocksWidth * 100)-DisplayCenter),
		floor(location.Y / (MaxBlocksWidth * 100)-DisplayCenter));
}

void ATerrianGenerationMode::UpdateChunks()
{
	//在ChunkSize*ChunkSize的范围内加载Chunk
	for (int i = 0; i < ChunkSize; ++i)
	for (int j = 0; j < ChunkSize; ++j) 
	{
		FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i,
					  						ChunksCenterPosition.Y + j);
		uint64 index = NoiseTool::Index(chunkPosition.X,chunkPosition.Y);
		
		//若未加载该Chunk
		if (!Chunks.Contains(index)){
			//创建Chunk
			Chunks.Emplace(index,Chunk(chunkPosition));
			Chunk& chunk = Chunks[index];
			//加载chunk信息
			LoadChunk(chunk);
			//加载地形方块ID
			LoadTerrianBlocksID(chunk);
		}
	}	
	
	//在(ChunkSize-2)*(ChunkSize-2)的范围内显示Chunk
	for (int i = 2; i < ChunkSize-2; ++i)
	for (int j = 2; j < ChunkSize-2; ++j) 
	{
		FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i,
					  						ChunksCenterPosition.Y + j);
		uint64 index = NoiseTool::Index(chunkPosition.X,chunkPosition.Y);
		Chunk* chunkptr = &Chunks[index];
		
		//若未显示该Chunk
		if(!Chunks2Display.Contains(chunkptr)){
			//记录Chunk为已显示
			Chunks2Display.Add(chunkptr);

			Chunk& chunk = *chunkptr;
			//（禁用）生成雨水侵蚀现象
			//RainGenerator::GenerateRain(chunk,this->Info);	
			//生成建筑
			BuildingGenerator::GenerateBuilding(chunk,this->Info);
			//生成植被
			PlantGenerator::GeneratePlant(chunk,this->Info);
			//显示chunk
			DisplayChunk(chunk);
		}
	}
	
	//生成建筑
	GenerateBuildingBlocks();
}

void ATerrianGenerationMode::LoadChunk(Chunk& chunk)
{
	//生成高度
	HeightGenerator::GenerateHeight(chunk,this->Info);
	//生成洞穴
	CaveGenerator::GenerateCave(chunk,this->Info);
	//生成温度
	TemperatureGenerator::GenerateTemperature(chunk);
	//生成湿度
	HumidityGenerator::GenerateHumidity(chunk);
	//生成生物群落属性
	BiomeGenerator::GenerateBiome(chunk);
}

void ATerrianGenerationMode::LoadTerrianBlocksID(Chunk& chunk){
	//载入地形方块
	for (int i = 0; i < 16; ++i)
	for (int j = 0; j < 16; ++j) 
	{	
		for (int k = chunk.BlocksHeight[i][j]; k > chunk.BlocksHeight[i][j]-20; --k)
		{
			int32 targetBlockID = CaculateBlockID(chunk,i,j,k);
			//随机泥土
			if (rand() % 255 >= 250){
				targetBlockID = 3;
			}

			FVector pos = FVector(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
			if(!Info.FindBlock(pos)){
				Info.AddBlockWithoutDisplay(pos,targetBlockID);
			}
		}

		//垫底方块（非显示）
		for (int k = chunk.BlocksHeight[i][j]-20; k > chunk.BlocksHeight[i][j]-23; --k)
		{
			FVector pos = FVector(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
			Info.AddBlockWithoutDisplay(pos,1);
		}
	}
}


void ATerrianGenerationMode::GenerateBuildingBlocks(){
	//根据待显示建筑列表，进行多个ABuilding的创建
	auto& buildings2Display = Info.GetBuildings2Display();
	for(auto &tuple: buildings2Display){
		uint64 posIndex = tuple.Get<0>();
		int32 buildingIndex = tuple.Get<1>();
		int32 rotate = tuple.Get<2>();
		CreateBuilding(buildingIndex,rotate,NoiseTool::UnIndex(posIndex));
	} 

	//显示完建筑就可以清理列表了
	buildings2Display.Reset();
}

int32 ATerrianGenerationMode::CaculateBlockID(Chunk& chunk,int32 i,int32 j,int32 k){
    if(i<0||i>=16||j<0||j>=16){return 0;}
    int32 dk = chunk.BlocksHeight[i][j]-k;
    //地下石头
    if(dk>=3){return 2;}
    //地下泥土
    if(dk>=1){return 3;}
    //地表方块	
    switch((BiomeType)chunk.BlocksBiome[i][j])
	{
			case BiomeType::Snow:	return 10;break;
			case BiomeType::Green:	return 1;break;
			case BiomeType::Dry:	return 3;break;
			case BiomeType::Stone:	return 5;break;
			case BiomeType::Desert:	return 4;break;
			default:				return 0;break;
	};
}


void ATerrianGenerationMode::DisplayChunk(Chunk& chunk){
	//添加地形方块进显示方块列表
	for (int i = 0; i < 16; ++i)
	for (int j = 0; j < 16; ++j) 
	{	
		for (int k = chunk.BlocksHeight[i][j]; k > chunk.BlocksHeight[i][j]-20; --k)
		{
			FVector pos = FVector(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
			int32* blockID = Info.FindBlock(pos);
			if(blockID){
				Info.AddBlock(pos,*blockID);
			}
		}
	}
	//根据待显示方块列表，进行多个ABlock的创建
	auto& blocks2Display = Info.GetBlocks2Display();
	for(auto& itr : blocks2Display){
		FVector BlockPosition = NoiseTool::UnIndex(itr.Key);
		const int32 dx[6] = {1,-1,0,0,0,0};
    	const int32 dy[6] = {0,0,1,-1,0,0};
    	const int32 dz[6] = {0,0,0,0,-1,1};
		for(int d=0;d<6;++d)
		{
			FVector pos = FVector(BlockPosition.X+dx[d],BlockPosition.Y+dy[d],BlockPosition.Z+dz[d]);
			auto result = Info.FindBlock(pos);
			
			bool needCreate = false;
			if(!result){
				needCreate = true;
			}
			else switch(*result){
				case 0:case 9:case 11:case 12:case 13:case 20:case 21:case 22:case 23:case 24:
				needCreate = true;
				break;
				}
			
			if(needCreate){
				CreateBlock(itr.Value,BlockPosition);
				break;
			}
		}
	}
	
	//显示完方块就可以清理列表了
	blocks2Display.Reset();
}

void ATerrianGenerationMode::UpdateBlocks(){
	TArray<TPair<uint64,int32>>& alterBlocks = Info.GetBlocks2Alter();
	for(auto& pair: alterBlocks){
		FVector pos = NoiseTool::UnIndex(pair.Get<0>());
		RemoveBlock(pos);
		CreateBlock(pair.Get<1>(),pos);
	}
	
	alterBlocks.Reset();
}

bool ATerrianGenerationMode::CreateBlock(int32 id, FVector pos)
{
	if (id < 0 || id > MAX_BLOCKS_NUM) {return false;}
	uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
	auto result = ABlocks.Find(index);
	//已存在方块，失败
	if(result)return false;
	
	//雪是特殊方块，处理特殊高度
	if(id==24)pos.Z-=0.5f;
	//挖空方块，处理特殊空气方块
	if(id==0){return true;}

	//创建方块Actor
	ABlock* block = GetWorld()->SpawnActor<ABlock>(pos*100, FRotator::ZeroRotator);
	block->InitByBlockID(id);
	ABlocks.Add(index,block);

	return true;
}


void ATerrianGenerationMode::RemoveBlock(FVector pos){
	uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
	auto result = ABlocks.Find(index);
	if(!result)return;
	(*result)->Destroy();
	*result = nullptr;
}

bool ATerrianGenerationMode::CreateBuilding(int32 id,int32 rotate, FVector pos){
	FString str = TEXT("Blueprint'/Game/Blueprints/BP_Building") + FString::FromInt(id)+TEXT(".BP_Building")+ FString::FromInt(id)+ TEXT("_C'");
	UClass* BPClass = LoadClass<AActor>(nullptr, *str);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(BPClass,pos*100+FVector(-50,-50,0), FRotator(0,rotate*90,0));

	return spawnActor!=nullptr;
}