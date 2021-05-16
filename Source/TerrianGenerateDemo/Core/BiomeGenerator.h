// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"
#include "Model/Biome.h"


/**
 * 生物群落分类器
 */
class TERRIANGENERATEDEMO_API BiomeGenerator
{
public:
	static void GenerateBiome(Chunk& chunk);
private:
	BiomeGenerator() = delete;
	~BiomeGenerator() = delete;
};
