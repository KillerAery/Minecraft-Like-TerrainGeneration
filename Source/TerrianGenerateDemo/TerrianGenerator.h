// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

const size_t MaxBlocksWidth = 16;
const size_t MaxBlocksHeigth = 256;

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
