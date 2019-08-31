// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Block.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UCLASS()
class TERRIANGENERATEDEMO_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UPROPERTY(EditAnywhere)
	FVector2D ChunkPosition = FVector2D(0,0);

	ABlock* Blocks[16][16][256];

	int32 BlocksHeight[16][16];
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
