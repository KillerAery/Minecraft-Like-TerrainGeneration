// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"
#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API BuildingGenerator
{
public:
	static void GenerateBuilding(Chunk& chunk,GlobalInfo& info);
public:
	BuildingGenerator() = delete;
	~BuildingGenerator() = delete;
};
