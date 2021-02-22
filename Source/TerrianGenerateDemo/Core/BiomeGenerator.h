// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"


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
  