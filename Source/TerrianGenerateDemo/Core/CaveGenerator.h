// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"
#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API CaveGenerator
{
public:
	static void GenerateCave(Chunk& chunk,GlobalInfo& info);
	static void Expose(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 k);
private:
	static int32 CaculateUnderBlock(Chunk& chunk,int32 i,int32 j,int32 k);
	CaveGenerator() = delete;
	~CaveGenerator() = delete;
};
