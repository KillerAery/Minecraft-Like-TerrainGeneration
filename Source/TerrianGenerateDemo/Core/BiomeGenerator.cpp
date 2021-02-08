// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/BiomeGenerator.h"
#include "Core/NoiseTool.h"

BiomeGenerator::BiomeGenerator()
{
}

BiomeGenerator::~BiomeGenerator()
{
}

BiomeType BiomeGenerator::GetBiomeType(FVector2D BlockPosition)
{
	return BiomeType::normal;
}

void BiomeGenerator::GenerateBiome(FVector2D ChunkPosition, BiomeType BlocksBiome[MaxBlocksWidth][MaxBlocksWidth])
{

}
