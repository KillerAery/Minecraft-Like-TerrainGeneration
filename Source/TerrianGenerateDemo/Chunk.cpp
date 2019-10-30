// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "Tools/NoiseTool.h"

// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			for (int k = 0; k < 256; ++k)
				Blocks[i][j][k] = nullptr;

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
	FVector2D position2D = ChunkPosition / BlockWidth;
	FVector2D pi = FVector2D(floor(position2D.X), floor(position2D.Y));
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1.0f,pi.Y},{pi.X,pi.Y + 1.0f},{pi.X + 1.0f,pi.Y + 1.0f} };


	FVector2D pf,w;

	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j) {
			
			pf = FVector2D(i/MaxBlocksWidth,j/MaxBlocksWidth);
			w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);


			BlocksHeight[i][j] = FMath::Clamp<float>(FMath::Lerp(
				FMath::Lerp(NoiseTool::grad(vertex[0], pf),
					NoiseTool::grad(vertex[1], pf - FVector2D(1.0f, 0.0f)),
					w.X),
				FMath::Lerp(NoiseTool::grad(vertex[2], pf - FVector2D(0.0f, 1.0f)),
					NoiseTool::grad(vertex[3], pf - FVector2D(1.0f, 1.0f)),
					w.X),
				w.Y) + 24, 0, 100);
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
					Blocks[i][j][k] = World->SpawnActor<ABlock>(
						FVector(i * 100 + ChunkPosition.X * 1600,
							j * 100 + ChunkPosition.Y * 1600,
							k * 100),
						FRotator::ZeroRotator);
				}
				else {

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

