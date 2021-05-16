// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API NoiseTool
{
public:
	static int32 hash11(int32 position);

	static int32 hash11WithGlobalSeed(int32 position);

	static FVector2D hash22(FVector2D position2D);

	static int32 hash21(FVector2D position2D);

	static FVector hash33(FVector position3D);

	static int32 hash31(FVector position3D);
	
	//设置种子
	static void setSeed(int32 seed  = 107);

	//返还随机值 0~1023
	static int32 randInt(FVector2D position);
	
	//返还随机值 0.0f~1.0f
	static float rand(FVector2D position);

	//求梯度值	
	static float grad(FVector2D vertex, FVector2D position2D);
	
	//求梯度值(优化)
	static float grad_f(FVector2D vertex, FVector2D position2D);

	//输入2D坐标，输出柏林噪声值[-1,1]
	static float perlinNoise(FVector2D position2D);

	//输入2D坐标，输出value噪声值[-1,1]
	static float valueNoise(FVector2D position2D);

	//输入2D坐标，输出simplex噪声值[-1,1]
	static float simplexNoise(FVector2D position2D);

	//输入3D坐标，输出simplex噪声值[-1,1]
	static float simplexNoise(FVector position);

	//预处理噪声（预先设置全局顶点）：初始2D坐标，晶格大小
	static void prehandlePerlinNoise(FVector2D position2D, int32 crystalSize);

	//预处理噪声（预先设置全局顶点）：初始2D坐标，晶格大小
	static void prehandleValueNoise(FVector2D position2D, int32 crystalSize);

	//预处理噪声（预先设置全局顶点）：初始2D坐标，晶格大小
	static void prehandleSimplexNoise(FVector2D position2D, int32 crystalSize);

	//预处理噪声（预先设置全局顶点）：初始3D坐标，晶格大小
	static void prehandleSimplexNoise(FVector position,int32 crystalSize);

	//二阶bezier曲线 t应[0.0f~1.0f]
	static FVector2D bezier(FVector2D p0,FVector2D p1,FVector2D p2,float t);

	//三阶bezier曲线 t应[0.0f~1.0f]
	static FVector2D bezier(FVector2D p0,FVector2D p1,FVector2D p2,FVector2D p3,float t);

	//三维坐标转一维坐标（压缩坐标）
	static uint64 Index(int32 x,int32 y,int32 z=0);

	//一维坐标转三维坐标（解压坐标）
	static FVector UnIndex(uint64 index);
private:
	static FVector2D GlobalVertex[4];
	static FVector GlobalVertex3D[3];
	static FVector2D GlobalOffset;
	static FVector GlobalOffset3D;
	static int32 GlobalSeed;
private:
	NoiseTool() = delete;
	~NoiseTool() = delete;
};
