// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chunk.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameModeBase.h"
#include "TerrianGenerationMode.generated.h"

const int32 LoadRadius = 3;
const int32 ChunkSize = LoadRadius * 2 - 1;
const int32 Center = LoadRadius - 1;
const int32 MAX_BLOCKS_NUM = 9;
/**
 * 
 */
UCLASS()
class TERRIANGENERATEDEMO_API ATerrianGenerationMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATerrianGenerationMode();

	void BeginPlay()override;

	void Tick(float DeltaSeconds)override;

	//�������������ң�����λ��
	UFUNCTION(BlueprintCallable, Category = "Func")
	void SetCameraLoaction(FVector location);

	//������ΧChunk
	UFUNCTION(BlueprintCallable, Category = "Func")
	void UpdateChunks();

	//����λ��
	UPROPERTY(VisibleAnywhere)
	FVector2D ChunksCenterPosition;
protected:
	//ȫ��Chunk
	Chunk* Chunks[2][ChunkSize][ChunkSize];
	//��ǰ������������ʹ��˫�ػ��壩
	size_t ChunksIndex = 0;
	//����Block����
	UBlueprint* BlocksTemplate[MAX_BLOCKS_NUM];
	bool NeedChunk(FVector2D chunkPosition);
	Chunk* GenerateChunk(FVector2D chunkPosition);
	ABlock* CreateBlock(int32 id, FVector location);
};
