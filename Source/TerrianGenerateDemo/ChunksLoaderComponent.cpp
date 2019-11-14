// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunksLoaderComponent.h"

// Sets default values for this component's properties
UChunksLoaderComponent::UChunksLoaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	for (int index = 0; index < 2; ++index)
		for (int x = 0; x < ChunkSize; ++x)
			for (int y = 0; y < ChunkSize; ++y) {
				Chunks[index][x][y] = nullptr;
			}
}


void UChunksLoaderComponent::SetPlayerLoactionEveryTick(FVector location)
{
	ChunksCenterPosition = FVector2D((int32)(location.X / 1600), (int32)(location.Y / 1600));
}

// Called when the game starts
void UChunksLoaderComponent::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerLoactionEveryTick(FVector::ZeroVector);
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
				Chunks[nextChunksIndex][(int32)d.X + Center][(int32)d.Y + Center] = Chunks[ChunksIndex][i][j];
			}
			else {
				Chunks[ChunksIndex][i][j]->Destroy();	//卸载chunk
				Chunks[ChunksIndex][i][j] = nullptr;
			}
		}

	UWorld* World = GetWorld();
	//生成新Chunk
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[nextChunksIndex][i][j]) {
				AChunk::Initialize(FVector2D(ChunksCenterPosition.X + (i - Center),
					ChunksCenterPosition.Y + (j - Center)));

				Chunks[nextChunksIndex][i][j] =
					World->SpawnActor<AChunk>
					(FVector((ChunksCenterPosition.X + i - Center) * 1600,
						0,
						(ChunksCenterPosition.Y + i - Center) * 1600),
						FRotator::ZeroRotator);
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
	
	UpdateChunks();
	// ...
}

