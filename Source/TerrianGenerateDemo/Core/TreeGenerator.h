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
private:
	TreeGenerator() = delete;
	~TreeGenerator() = delete;
};
