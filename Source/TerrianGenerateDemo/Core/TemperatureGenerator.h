// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"


/**
 * 
 */
class TERRIANGENERATEDEMO_API TemperatureGenerator
{
public:
	static void GenerateTemperature(Chunk& chunk);
private:
	TemperatureGenerator() = delete;
	~TemperatureGenerator() = delete;
};
  