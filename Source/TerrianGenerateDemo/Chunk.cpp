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

	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j) {
			BlocksHeight[i][j] = NoiseTool::perlinNoise(ChunkPosition + FVector2D(i / 16.0f, j / 16.0f));
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *ChunkPosition.ToString());
		}

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

				World->SpawnActor<ABlock>(FVector(i * 100 + ChunkPosition.X * 1600, j * 100 + ChunkPosition.Y * 1600, k * 100), FRotator::ZeroRotator);

					////产生acotor开始
					//FTransform spawnTransform(FRotator::ZeroRotator, FVector(i * 100 + ChunkPosition.X * 1600, j * 100 + ChunkPosition.Y * 1600, k * 100), FVector(1.0f, 1.0f, 1.0f));
					////产生acotor开始
					//ABlock*landSample = Cast<ABlock>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ABlock::StaticClass(), spawnTransform));
					////初始化传参
					//if (k == BlocksHeight[i][j] - 1) {

					//	ABlock::Initialize(1);
					//}
					//else {
					//	ABlock::Initialize(3);
					//}
					////产生阶段结束
					//UGameplayStatics::FinishSpawningActor(landSample, spawnTransform);
				
				index++;
			}
		}
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

