// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API HeightGenerator
{
public:
	static float GetHeight(FVector2D blockPosition);
	static void GenerateHeight(Chunk& chunk,GlobalInfo& info);
private:
	HeightGenerator() = delete;
	~HeightGenerator() = delete;
};
