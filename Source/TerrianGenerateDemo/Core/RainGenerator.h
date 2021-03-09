// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API RainGenerator
{
public:
	static void GenerateRain(Chunk& chunk,GlobalInfo& info);
	static void flow(Chunk& chunk,GlobalInfo& info,float rain,int i,int j,int k);
	static TSet<uint64> waters;
private:
	RainGenerator() = delete;
	~RainGenerator() = delete;
};
