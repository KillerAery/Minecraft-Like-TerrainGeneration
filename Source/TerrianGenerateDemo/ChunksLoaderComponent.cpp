// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunksLoaderComponent.h"

// Sets default values for this component's properties
UChunksLoaderComponent::UChunksLoaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UChunksLoaderComponent::SetPlayerLoactionEveryTick(FVector location)
{
	ChunksCenterPosition = FVector2D((int32)(location.X / 1600 - 0.5), (int32)(location.Y / 1600 - 0.5));
}

// Called when the game starts
void UChunksLoaderComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UChunksLoaderComponent::UpdateChunks()
{
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
				Chunks[nextChunksIndex][i][j] =	GenerateChunk(FVector2D(
					ChunksCenterPosition.X + (i - Center),
					ChunksCenterPosition.Y + (j - Center))
				);
			}
		}
	//切换
	ChunksIndex = nextChunksIndex;
}

bool UChunksLoaderComponent::NeedChunk(FVector2D chunkPosition)
{
	FVector2D d = chunkPosition - ChunksCenterPosition;
	return abs((int32)(d.X)) < LoadRadius && abs((int32)fabs(d.Y)) < LoadRadius;
}

// Called every frame
void UChunksLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//UpdateChunks();
	// ...
}


Chunk* UChunksLoaderComponent::GenerateChunk(FVector2D chunkPosition)
{
	Chunk* chunk = new Chunk(chunkPosition);
	UWorld* World = GetWorld();

	FVector2D chunkWorldPosition = FVector2D(chunkPosition.X * 1600,chunkPosition.Y * 1600);

	int32 index = 0;
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k <= chunk->BlocksHeight[i][j]; ++k)
			{
				if (k < chunk->BlocksHeight[i][j] &&
					(i <= 0 || k < chunk->BlocksHeight[i - 1][j]) &&
					(j <= 0 || k < chunk->BlocksHeight[i][j - 1]) &&
					(i >= 15 || k < chunk->BlocksHeight[i + 1][j]) &&
					(j >= 15 || k < chunk->BlocksHeight[i][j + 1])) {
				}
				else {
					chunk->Blocks[i][j][k] = World->SpawnActor<ABlock>(
						FVector(chunkWorldPosition.X + i * 100,
							chunkWorldPosition.Y + j * 100,
							k * 100),
						FRotator::ZeroRotator);

					chunk->Blocks[i][j][k]->SetFolderPath("/Blocks");

					//if (k == BlocksHeight[i][j] && (rand() % 255 < 240)) {
					//	Blocks[i][j][k] = 1;
					//}
					//else {
					//	Blocks[i][j][k] = 3;
					//}
				}
				index++;
			}
		}
	return chunk;
}