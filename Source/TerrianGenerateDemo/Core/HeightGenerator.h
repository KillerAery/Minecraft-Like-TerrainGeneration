// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GenerationSettings.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API HeightGenerator
{
public:
	HeightGenerator();
	~HeightGenerator();
	
	static float GetHeight(FVector2D blockPosition);

	static void GenerateHeight(FVector2D ChunkPosition,int32 BlocksHeight[MaxBlocksWidth][MaxBlocksWidth]);


};
