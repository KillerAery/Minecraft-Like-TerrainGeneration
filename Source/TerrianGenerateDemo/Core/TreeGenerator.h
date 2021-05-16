// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/Chunk.h"
#include "Model/GlobalInfo.h"

/**
 * 植被生成器
 */
class TERRIANGENERATEDEMO_API PlantGenerator
{
public:
	//生成树
	static void GeneratePlant(Chunk& chunk,GlobalInfo& info);
	//生成花
	static bool GenerateFlower(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize);
	//生成树
	static bool GenerateTree(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 cystalSize);
	//生成树叶
	static void GenerateLeaves(Chunk& chunk,GlobalInfo& info,int32 i,int32 j,int32 height,int32 radius,int32 targetLeafID);
private:
	PlantGenerator() = delete;
	~PlantGenerator() = delete;
private:
	//树叶模板（生成树叶阶段用）
	static bool leavesTemplate[4][5][5];
};
