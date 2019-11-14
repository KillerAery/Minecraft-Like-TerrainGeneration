// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Tools/NoiseTool.h"

FVector2D AChunk::InitialChunkPosition = FVector2D();

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChunkPosition = InitialChunkPosition;

	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			for (int k = 0; k < 256; ++k)
				Blocks[i][j][k] = nullptr;

}


void AChunk::Initialize(FVector2D chunkPosition)
{
	InitialChunkPosition = chunkPosition;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	GeneratePerlinNoise();
	GenerateBlocks();

}

void AChunk::GeneratePerlinNoise()
{
	FVector2D pi = FVector2D(floor(ChunkPosition.X), floor(ChunkPosition.Y));
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1.0f,pi.Y},{pi.X,pi.Y + 1.0f},{pi.X + 1.0f,pi.Y + 1.0f} };

	FVector2D pf,w;
	//todo
	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j) {
			
			pf = FVector2D(i/float(MaxBlocksWidth),j/float(MaxBlocksWidth));
			w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

			BlocksHeight[i][j] += 0.33f * FMath::Clamp<float>(FMath::Lerp(
				FMath::Lerp((int32)(NoiseTool::hash21(vertex[0]) % 57),
				(int32)(NoiseTool::hash21(vertex[1]) % 57),
					w.X),
				FMath::Lerp((int32)(NoiseTool::hash21(vertex[2]) % 57),
				(int32)(NoiseTool::hash21(vertex[3]) % 57),
					w.X),
				w.Y), 0, 57);
		}

	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j) {

			pf = FVector2D(i / float(MaxBlocksWidth), j / float(MaxBlocksWidth));
			w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

			BlocksHeight[i][j] += FMath::Clamp<float>(FMath::Lerp(
				FMath::Lerp((int32)(NoiseTool::hash21(vertex[0]/2) % 20),
				(int32)(NoiseTool::hash21(vertex[1]/2) % 20),
					w.X),
				FMath::Lerp((int32)(NoiseTool::hash21(vertex[2]/2) % 20),
				(int32)(NoiseTool::hash21(vertex[3]/2) % 20),
					w.X),
				w.Y), 0, 20);
		}


}

void AChunk::GenerateBlocks()
{
	int32 index = 0;
	UWorld* World = GetWorld();
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) {
			for (int k = 0; k <= BlocksHeight[i][j]; ++k)
			{

				if (k == BlocksHeight[i][j] && (rand() % 255 < 240)) {
					ABlock::Initialize(1);
				}
				else {
					ABlock::Initialize(3);
				}

				if (k < BlocksHeight[i][j] &&
					(i <= 0 || k < BlocksHeight[i - 1][j]) &&
					(j <= 0 || k < BlocksHeight[i][j - 1]) &&
					(i >= 15 || k < BlocksHeight[i + 1][j]) &&
					(j >= 15 || k < BlocksHeight[i][j + 1])) {

				}
				else {
					Blocks[i][j][k] = World->SpawnActor<ABlock>(
						FVector(i * 100 + ChunkPosition.X * 1600,
							j * 100 + ChunkPosition.Y * 1600,
							k * 100),
						FRotator::ZeroRotator);
				}


				index++;
			}
		}
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

