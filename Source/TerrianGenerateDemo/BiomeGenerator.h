// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class BiomeType
{
	hot,
	normal,
	cold
};

/**
 * 
 */
class TERRIANGENERATEDEMO_API BiomeGenerator
{
public:
	BiomeGenerator();
	~BiomeGenerator();
	static BiomeType GetBiomeType(FVector2D BlockPosition);
};
