// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


const int32 SeaLevel = 79;
/**
 * 
 */
class TERRIANGENERATEDEMO_API GlobalInfo
{
public:
	//全局方块列表
	TMap<uint64,int32> GolbalBlocksID;

	GlobalInfo();

	~GlobalInfo();
};
