// Fill out your copyright notice in the Description page of Project Settings.

#include "BiomeGenerator.h"
#include "NoiseTool.h"

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
