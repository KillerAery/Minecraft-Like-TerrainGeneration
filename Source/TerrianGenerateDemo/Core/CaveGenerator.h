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
private:
	CaveGenerator() = delete;
	~CaveGenerator() = delete;
};
