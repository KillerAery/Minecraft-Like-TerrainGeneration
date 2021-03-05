// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API TreeGenerator
{
public:
	static void GenerateTree(Chunk& chunk,GlobalInfo& info);
	static bool GenerateFlower(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize);
	static bool GenerateTree(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize);
	static void GenerateLeaves(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 height,int32 radius,int32 targetLeafID);
private:
	static void AddBlockWithIndex(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 height,int32 targetID);
	TreeGenerator() = delete;
	~TreeGenerator() = delete;
};
