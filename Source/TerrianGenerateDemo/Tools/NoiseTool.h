// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API NoiseTool
{
public:
	static uint32_t hash11(int32 position);
	static FVector2D hash22(FVector2D position2D);
	static uint32_t hash21(FVector2D position2D);
	static FVector hash33(FVector position3D);
	static uint32_t hash31(FVector position3D);

	static float grad(FVector2D vertex,FVector2D position2D);
	static float perlinNoise(FVector2D position2D);
	static float valueNoise(FVector2D position2D);
private:
	NoiseTool() = delete;
	~NoiseTool() = delete;
};
