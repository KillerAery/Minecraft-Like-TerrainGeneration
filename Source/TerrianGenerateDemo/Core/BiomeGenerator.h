// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"


enum class BiomeType{
	None = 0,
	Snow = 1,
	Green = 2,
	Dry = 3,
	Stone = 4,
	Desert = 5
};

/**
 * 
 */
class TERRIANGENERATEDEMO_API BiomeGenerator
{
public:
	static void GenerateBiome(Chunk& chunk);
private:
	BiomeGenerator() = delete;
	~BiomeGenerator() = delete;
};
