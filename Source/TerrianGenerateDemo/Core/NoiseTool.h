// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API NoiseTool
{
public:
	static uint32 hash11(int32 position);
	static FVector2D hash22(FVector2D position2D);
	static uint32 hash21(FVector2D position2D);
	static FVector hash33(FVector position3D);
	static uint32 hash31(FVector position3D);
	
	//求梯度值	
	static float grad(FVector2D vertex, FVector2D position2D);
	//求梯度值(优化)
	static float grad_f(FVector2D vertex, FVector2D position2D);
	
	//输入2D坐标，输出噪声值
	static float perlinNoise(FVector2D position2D);
	static float valueNoise(FVector2D position2D);
	static float simplexNoise(FVector2D position2D);

	//预处理噪声（预先设置全局顶点）
	//初始2D坐标，晶格大小，频率
	static void prehandlePerlinNoise(FVector2D position2D, int32 crystalSize,int32 frequence);
	static void prehandleValueNoise(FVector2D position2D, int32 crystalSize,int32 frequence);
	static void prehandleSimplexNoise(FVector2D position2D, int32 crystalSize,int32 frequence);
private:
	static FVector2D GlobalVertex[4];
	static FVector2D GlobalOffset;
private:
	NoiseTool() = delete;
	~NoiseTool() = delete;
};
