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
	//加载Chunk
	for (int i = 0; i < ChunkSize; ++i)
	for (int j = 0; j < ChunkSize; ++j) 
	{
		FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i,
					  						ChunksCenterPosition.Y + j);

		if (NeedLoadChunk(chunkPosition)){
			int32 index = Chunks.Add(Chunk(chunkPosition));
			Chunk& chunk = Chunks[index];
			//加载chunk信息
			LoadChunk(chunk);
		}
	}	
	
	/* 关闭雨水侵蚀
	for (int i = 0; i < DisplaySize; ++i)
	for (int j = 0; j < DisplaySize; ++j) 
	{
		FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i +1,
					  						ChunksCenterPosition.Y + j +1);
		Chunk* chunk = Chunks.FindByPredicate(
		[chunkPosition](Chunk& chunk){return FVector2D::DistSquared(chunkPosition,chunk.ChunkPosition)<0.0001f;});

		if(chunk){
			//生成雨水侵蚀现象
			RainGenerator::GenerateRain(*chunk,this->Info);
		}
	}
	*/
	
	for (int i = 1; i < ChunkSize-1; ++i)
	for (int j = 1; j < ChunkSize-1; ++j) 
	{
		FVector2D position = FVector2D(ChunksCenterPosition.X + i,
					  				   ChunksCenterPosition.Y + j);

		Chunk* chunk = Chunks.FindByPredicate(
		[position](Chunk& chunk){
			return FVector2D::DistSquared(position,chunk.ChunkPosition)<0.0001f;
		});

		if(!Chunk2Build.Find(chunk)){
			//生成建筑
			BuildingGenerator::GenerateBuilding(*chunk,this->Info);
			//加载地形方块ID
			LoadTerrianBlocksID(*chunk);
			Chunk2Build.Add(chunk);
		}
	}
	
	for (int i = 2; i < ChunkSize-2; ++i)
	for (int j = 2; j < ChunkSize-2; ++j) 
	{
		FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i,
					  						ChunksCenterPosition.Y + j);
		Chunk* chunk = GetDisplayChunk(chunkPosition);
		if(chunk){			
			//生成植被
			TreeGenerator::GenerateTree(*chunk,this->Info);
			//显示chunk
			DisplayChunk(*chunk);
		}
	}

	//生成建筑
	GenerateBuildingBlocks();
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

			uint64 index = NoiseTool::Index(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
			if(!Info.GolbalBlocksID.Find(index)){
				Info.GolbalBlocksID.Emplace(index,targetBlockID);
            	chunk.BlocksID.Emplace(index,targetBlockID);
			}
		}

		//垫底方块
		for (int k = chunk.BlocksHeight[i][j]-20; k > chunk.BlocksHeight[i][j]-23; --k)
		{
		uint64 index = NoiseTool::Index(chunk.ChunkPosition.X*16+i,chunk.ChunkPosition.Y*16+j,k);
		Info.GolbalBlocksID.Emplace(index,1);
		}
	}
}


void ATerrianGenerationMode::GenerateBuildingBlocks(){
	for(auto &tuple: Info.GolbalBudildings){
		uint64 posIndex = tuple.Get<0>();
		int32 buildingIndex = tuple.Get<1>();
		int32 rotate = tuple.Get<2>();

		CreateBuilding(buildingIndex,rotate,NoiseTool::UnIndex(posIndex));
	} 
	Info.GolbalBudildings.Reset();
}


bool ATerrianGenerationMode::NeedLoadChunk(FVector2D chunkPosition){
	//是否已存在Chunks列表内
	if(Chunks.FindByPredicate(
		[chunkPosition](Chunk& chunk){
			return FVector2D::DistSquared(chunkPosition,chunk.ChunkPosition)<0.0001f;
		})){
		return false;
	}
	return true;
}


Chunk* ATerrianGenerationMode::GetDisplayChunk(FVector2D chunkPosition){
	Chunk* chunk = Chunks.FindByPredicate(
		[chunkPosition](Chunk& chunk){
			return FVector2D::DistSquared(chunkPosition,chunk.ChunkPosition)<0.0001f;
		});

	if(Chunks2Display.Find(chunk)) return nullptr;
	Chunks2Display.Add(chunk);
	return chunk;
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

int32 ATerrianGenerationMode::CaculateBlockID(Chunk& chunk,int32 i,int32 j,int32 k){
    if(i<0||i>=16||j<0||j>=16){return 0;}

    int32 dk = chunk.BlocksHeight[i][j]-k;
    /* None = 0 雪地 Snow = 1 草地 Green = 2
    泥地 Dry = 3 石地 Stone = 4 沙漠 Desert = 5
    */
    //地下石头
    if(dk>=3){return 2;}
    //地下泥土
    if(dk>=1){return 3;}
    //地表方块	
    switch(chunk.BlocksBiome[i][j])
	{
			case 1:return 10;break;
			case 2:return 1;break;
			case 3:return 3;break;
			case 4:return 5;break;
			case 5:return 4;break;
			default:return 0;break;
	};
}

//展示Chunk
void ATerrianGenerationMode::DisplayChunk(Chunk& chunk){
	//显示所有方块
	for(auto& itr : chunk.BlocksID){
		FVector v = NoiseTool::UnIndex(itr.Key);
		FVector BlockPosition = FVector(
			v.X,
			v.Y,
			v.Z);

		const int32 dx[6] = {1,-1,0,0,0,0};
    	const int32 dy[6] = {0,0,1,-1,0,0};
    	const int32 dz[6] = {0,0,0,0,-1,1};

		for(int d=0;d<6;++d)
		{
			uint64 index = NoiseTool::Index(v.X+dx[d],v.Y+dy[d],v.Z+dz[d]);
			auto result = Info.GolbalBlocksID.Find(index);
			bool needCreate = false;
			if(!result){needCreate = true;}
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
	//释放方块ID内存，因为已无用处
	chunk.BlocksID=std::move(TArray<TPair<uint64,int32>>());
}


bool ATerrianGenerationMode::CreateBlock(int32 id, FVector pos)
{
	if (id < 0 || id > MAX_BLOCKS_NUM) {return false;}
	uint64 index = NoiseTool::Index(pos.X,pos.Y,pos.Z);
	auto result = Blocks.Find(index);
	//已存在方块，失败
	if(result)return false;
	
	//雪是特殊方块，处理特殊高度
	if(id==24)pos.Z-=0.5f;
	//挖空方块，处理特殊空气方块
	if(id==0){return true;}

	//创建方块Actor
	ABlock* block = GetWorld()->SpawnActor<ABlock>(pos*100, FRotator::ZeroRotator);
	block->InitByBlockID(id);
	Blocks.Add(index,block);

	return true;
}


bool ATerrianGenerationMode::CreateBuilding(int32 id,int32 rotate, FVector pos){
	FString str = TEXT("Blueprint'/Game/Blueprints/BP_Building") + FString::FromInt(id)+TEXT(".BP_Building")+ FString::FromInt(id)+ TEXT("_C'");
	UClass* BPClass = LoadClass<AActor>(nullptr, *str);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(BPClass,pos*100+FVector(-50,-50,0), FRotator(0,rotate*90,0));

	return spawnActor!=nullptr;
}

/*
void ATerrianGenerationMode::Expose(GlobalInfo& info,int32 i,int32 j,int32 k){
    const int32 dx[6] = {1,-1,0,0,0,0};
    const int32 dy[6] = {0,0,1,-1,0,0};
    const int32 dz[6] = {0,0,0,0,-1,1};
    for(int d = 0;d<6;++d){
        uint64 index = NoiseTool::Index(
            ChunkPosition.X*16+i+dx[d],
            ChunkPosition.Y*16+j+dy[d],
            k+dz[d]
            );
        if(!info.GolbalBlocksID.Find(index)){
            info.GolbalBlocksID.Emplace(index,CaculateBlockID(i+dx[d],j+dy[d],k+dz[d]));
        }
    }
}
*/