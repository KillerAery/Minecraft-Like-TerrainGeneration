// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/TerrianGenerationMode.h"
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
		(int32)(location.X / (MaxBlocksWidth * 100) - 0.5f),
		(int32)(location.Y / (MaxBlocksWidth * 100) - 0.5f));
}

void ATerrianGenerationMode::UpdateChunks()
{
	//生成新Chunk
	for (int i = 0; i < ChunkSize; ++i)
	for (int j = 0; j < ChunkSize; ++j) 
	{
		FVector2D f = FVector2D(ChunksCenterPosition.X + (i - Center),
								ChunksCenterPosition.Y + (j - Center)
								);

		if (NeedChunk(f)) {
			GenerateChunk(f);
		}
	}
}

bool ATerrianGenerationMode::NeedChunk(FVector2D chunkPosition)
{
	//是否已存在Chunks列表内
	if(Chunks.FindByPredicate(
		[chunkPosition](Chunk& chunk){
			return FVector2D::DistSquared(chunkPosition,chunk.ChunkPosition)<0.001f;
		}))
	{
		return false;
	}

	//不存在Chunk列表，则判断是否在加载范围
	FVector2D d = chunkPosition - ChunksCenterPosition;
	return abs((int32)(d.X)) < LoadRadius && abs((int32)fabs(d.Y)) < LoadRadius;
}

void ATerrianGenerationMode::GenerateChunk(FVector2D chunkPosition)
{
	int32 index = Chunks.Add(Chunk(chunkPosition));
	Chunk& chunk = Chunks[index];

	FVector2D chunkWorldPosition = FVector2D(chunkPosition.X * MaxBlocksWidth * 100, chunkPosition.Y * MaxBlocksWidth * 100);

	const int32 TRICK_EDGE_HEIGH = 5;

	for (int i = 0; i < 16; ++i)
	for (int j = 0; j < 16; ++j) 
	{
		for (int k = chunk.BlocksHeight[i][j],count = TRICK_EDGE_HEIGH; k >= 0; --k)
		{
			if(i == 0 || j == 0 || i == 15 || j == 15){
				if(count==0)continue;
				count--;
			}
			else if (k <= chunk.BlocksHeight[i - 1][j] &&
				k <= chunk.BlocksHeight[i][j - 1] &&
				k <= chunk.BlocksHeight[i + 1][j] &&
				k <= chunk.BlocksHeight[i][j + 1] &&
				k < chunk.BlocksHeight[i][j])
			{
					break;
			}

			FVector BlockPosition = FVector(
				chunkWorldPosition.X + i * 100,
				chunkWorldPosition.Y + j * 100,
				k * 100);
			
			if (k == chunk.BlocksHeight[i][j] && (rand() % 255 < 240)) {
				chunk.Blocks[i][j][k] = CreateBlock(1,BlockPosition);
			}
			else{
				chunk.Blocks[i][j][k] = CreateBlock(3,BlockPosition);
			}
				
		}
	}
}


int32 ATerrianGenerationMode::GetHeight(FVector2D position){
	return 0;
}

ABlock* ATerrianGenerationMode::CreateBlock(int32 id, FVector location)
{
	if (id <= 0 || id > MAX_BLOCKS_NUM) {
		return nullptr;
	}
	ABlock* block = GetWorld()->SpawnActor<ABlock>(location, FRotator::ZeroRotator);
	block->InitByBlockID(id);
	return block;
}



