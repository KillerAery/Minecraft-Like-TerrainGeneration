// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrianGenerateSetting.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API TerrianGenerator
{
public:
	TerrianGenerator();
	~TerrianGenerator();
	
	static float GetHeight(FVector2D blockPosition);

	static void GenerateHeight(FVector2D ChunkPosition,int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth]);


};
