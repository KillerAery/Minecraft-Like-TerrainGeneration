// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrianGenerationMode.h"
#include "TerrianGenerateDemoHUD.h"
#include "TerrianGenerateDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATerrianGenerationMode::ATerrianGenerationMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATerrianGenerateDemoHUD::StaticClass();
	//初始化空缓冲区
	for (int index = 0; index < 2; ++index)
	for (int i = 0; i < ChunkSize; ++i)
	for (int j = 0; j < ChunkSize; ++j) {
		Chunks[index][i][j] = nullptr;
	}
	//加载所有BLock种类
	for (int i = 0; i < MAX_BLOCKS_NUM; ++i) {
		FStringAssetReference asset = ("Blueprint'/Game/Blueprints/Blocks/BP_Block"+FString::FromInt(i+1)+".BP_Block"+ FString::FromInt(i+1) +"'");
		UObject* itemObj = asset.ResolveObject();
		BlocksTemplate[i] = Cast<UBlueprint>(itemObj);
	}
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
	ChunksCenterPosition = FVector2D((int32)(location.X / 1600 - 0.5), (int32)(location.Y / 1600 - 0.5));
}

void ATerrianGenerationMode::UpdateChunks()
{
	UE_LOG(LogTemp, Warning, TEXT("CCCC"));
	//清理
	size_t nextChunksIndex = !ChunksIndex;
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			Chunks[nextChunksIndex][i][j] = nullptr;
		}

	//对旧chunks进行载入检查
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[ChunksIndex][i][j]) {
				continue;
			}

			FVector2D chunkPosition = Chunks[ChunksIndex][i][j]->ChunkPosition;

			if (NeedChunk(chunkPosition)) {
				FVector2D d = chunkPosition - ChunksCenterPosition;
				Chunks[nextChunksIndex][(int32)d.X + Center][(int32)d.Y + Center] =
					Chunks[ChunksIndex][i][j];
			}
			else {
				delete Chunks[ChunksIndex][i][j];	//卸载chunk
				Chunks[ChunksIndex][i][j] = nullptr;
			}
		}

	//生成新Chunk
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[nextChunksIndex][i][j]) {
				Chunks[nextChunksIndex][i][j] = GenerateChunk(FVector2D(
					ChunksCenterPosition.X + (i - Center),
					ChunksCenterPosition.Y + (j - Center))
				);
			}
		}
	//切换
	ChunksIndex = nextChunksIndex;
}

bool ATerrianGenerationMode::NeedChunk(FVector2D chunkPosition)
{
	FVector2D d = chunkPosition - ChunksCenterPosition;
	return abs((int32)(d.X)) < LoadRadius && abs((int32)fabs(d.Y)) < LoadRadius;
}

Chunk* ATerrianGenerationMode::GenerateChunk(FVector2D chunkPosition)
{
	Chunk* chunk = new Chunk(chunkPosition);

	UWorld* World = GetWorld();
	FVector2D chunkWorldPosition = FVector2D(chunkPosition.X * 1600, chunkPosition.Y * 1600);

	int32 index = 0;
	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j) {
			for (int k = 0; k <= chunk->BlocksHeight[i][j]; ++k)
			{
				if (k < chunk->BlocksHeight[i][j] &&
					(i <= 0 || k < chunk->BlocksHeight[i - 1][j]) &&
					(j <= 0 || k < chunk->BlocksHeight[i][j - 1]) &&
					(i >= 15 || k < chunk->BlocksHeight[i + 1][j]) &&
					(j >= 15 || k < chunk->BlocksHeight[i][j + 1])) {
				}
				else {
					if (k == chunk->BlocksHeight[i][j] && (rand() % 255 < 240)) {
						chunk->Blocks[i][j][k] = CreateBlock(1,
							FVector(chunkWorldPosition.X + i * 100,
								chunkWorldPosition.Y + j * 100,
								k * 100)
						);
					}
					else {
						chunk->Blocks[i][j][k] = CreateBlock(3,
							FVector(chunkWorldPosition.X + i * 100,
								chunkWorldPosition.Y + j * 100,
								k * 100)
						);
					}
				}
				index++;
			}
		}
	return chunk;
}

ABlock* ATerrianGenerationMode::CreateBlock(int32 id, FVector location)
{
	if (id <= 0 || id > MAX_BLOCKS_NUM || !BlocksTemplate[id - 1]) {
		return nullptr;
	}
	return GetWorld()->SpawnActor<ABlock>(BlocksTemplate[id-1]->GeneratedClass,location,FRotator::ZeroRotator);
}



