// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API HumidityGenerator
{
public:
	static void GenerateHumidity(Chunk& chunk);
private:
	HumidityGenerator() = delete;
	~HumidityGenerator() = delete;
};
