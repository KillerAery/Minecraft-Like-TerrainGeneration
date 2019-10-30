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
	position3D = location;
}

// Called when the game starts
void UChunksLoaderComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateChunks();
}


void UChunksLoaderComponent::UpdateChunks()
{
	//清理
	size_t nextChunksIndex = !ChunksIndex;

	for (int x = 0; x < ChunkSize; ++x)
		for (int y = 0; y < ChunkSize; ++y) {
			Chunks[nextChunksIndex][x][y] = nullptr;
		}

	FVector2D position2D;
	position2D.X = position3D.X / 1600;
	position2D.Y = position3D.Z / 1600;

	UWorld* World = GetWorld();

	//对旧chunks进行载入检查
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[ChunksIndex][i][j]) {
				continue;
			}

			FVector2D chunkPosition2D = Chunks[ChunksIndex][i][j]->ChunkPosition;
			chunkPosition2D /= 1600;

			if (!NeedToLoadChunk(position2D, chunkPosition2D)) {
				Chunks[ChunksIndex][i][j]->Destroy();	//卸载chunk
				Chunks[ChunksIndex][i][j] = nullptr;
			}
			else {
				FVector2D d = chunkPosition2D - position2D;
				Chunks[nextChunksIndex][(int32)d.X + Center][(int32)d.Y + Center] = Chunks[ChunksIndex][i][j];
			}

		}

	//生成新Chunk
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[nextChunksIndex][i][j]) {
				Chunks[nextChunksIndex][i][j] =
					World->SpawnActor<AChunk>
					(FVector(position3D.X + i * 1600,
						position3D.Z + j * 1600,
						0),
						FRotator::ZeroRotator);

				continue;
			}
		}


	//切换
	ChunksIndex = nextChunksIndex;
}

bool UChunksLoaderComponent::NeedToLoadChunk(FVector2D position, FVector2D chunkPosition)
{
	FVector2D d = position - chunkPosition;
	return (int32)fabs(d.X)<=1 && (int32)fabs(d.Y) <= 1;
}

// Called every frame
void UChunksLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

