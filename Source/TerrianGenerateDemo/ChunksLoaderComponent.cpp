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

}


void UChunksLoaderComponent::UpdateChunks()
{
	//清理
	size_t nextChunksIndex = !ChunksIndex;

	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			Chunks[nextChunksIndex][i][j] = nullptr;
		}

	FVector2D pos2D = FVector2D(position3D.X, position3D.Z);
	pos2D /= 1600;

	//对旧chunks进行载入检查
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[ChunksIndex][i][j]) {
				continue;
			}

			FVector2D chunkPosition = Chunks[ChunksIndex][i][j]->ChunkPosition;

			if (NeedChunk(chunkPosition)) {
				FVector2D d = chunkPosition - pos2D;
				Chunks[nextChunksIndex][(int32)d.X + Center][(int32)d.Y + Center] = Chunks[ChunksIndex][i][j];
			}
			else {
				Chunks[ChunksIndex][i][j]->Destroy();	//卸载chunk
				Chunks[ChunksIndex][i][j] = nullptr;
			}
		}

	int dt = 1;

	UWorld* World = GetWorld();
	//生成新Chunk
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j) {
			if (!Chunks[nextChunksIndex][i][j]) {
				AChunk::Initialize(FVector2D(pos2D.X + (i - Center) * dt,
					pos2D.Y + (j - Center) * dt));

				Chunks[nextChunksIndex][i][j] =
					World->SpawnActor<AChunk>
					(FVector((pos2D.X + (i - Center) * dt) * 1600,
						0,
						(pos2D.Y + (i - Center) * dt) * 1600),
						FRotator::ZeroRotator);
			}
		}


	//切换
	ChunksIndex = nextChunksIndex;
}

bool UChunksLoaderComponent::NeedChunk(FVector2D chunkPosition)
{
	FVector2D pos2D = FVector2D(position3D.X, position3D.Z);
	FVector2D d = chunkPosition - pos2D;
	return (int32)fabs(d.X) < LoadRadius && (int32)fabs(d.Y) < LoadRadius;
}

// Called every frame
void UChunksLoaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateChunks();
	// ...
}

