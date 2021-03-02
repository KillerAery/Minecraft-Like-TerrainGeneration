// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Chunk.h"
#include "CoreMinimal.h"
/**
 * 
 */
class TERRIANGENERATEDEMO_API TreeGenerator
{
public:
	static void GenerateTree(Chunk& chunk);
	static void GenerateLeaves(Chunk& chunk,int32 i,int32 j,int32 height,int32 radius,int32 targetLeafID);
private:
	TreeGenerator() = delete;
	~TreeGenerator() = delete;
};
