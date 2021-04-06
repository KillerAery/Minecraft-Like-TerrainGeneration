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
	static TSet<uint64> domains;
	static FVector2D startPoint;

	static void GenerateBuilding(Chunk& chunk,GlobalInfo& info);

	static void GenerateDomains(Chunk& chunk,GlobalInfo& info);
	static void GenerateBuildings(Chunk& chunk,GlobalInfo& info);

	static void PlaceBuilding(GlobalInfo& info,int32 x,int32 y,int32 buildingIndex,int32 rotate);

	static void GeneratePaths(Chunk& chunk,GlobalInfo& info);
protected:
	BuildingGenerator() = delete;
	~BuildingGenerator() = delete;
};
