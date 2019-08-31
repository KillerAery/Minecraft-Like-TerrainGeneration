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

	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) {
			BlocksHeight[i][j] = NoiseTool::perlinNoise(ChunkPosition + FVector2D(i*1.0f / 16.0f, j * 1.0f / 16.0f));
			UE_LOG(LogTemp, Warning, TEXT("%d"), BlocksHeight[i][j]);
		}
	

}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	int32 index = 0;
	UWorld* World = GetWorld();
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) 
		for (int k = 0; k < BlocksHeight[i][j]; ++k)
		{
			if (World)
			{
				World->SpawnActor<ABlock>(FVector(i*100, j*100, k*100), FRotator::ZeroRotator);
				//if (k == BlocksHeight[i][j] - 1) {
				//	Blocks[i][j][k]->Initialize(1);
				//}
				//else {
				//	Blocks[i][j][k]->Initialize(2);
				//}
			}
			index++;
		}

}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

