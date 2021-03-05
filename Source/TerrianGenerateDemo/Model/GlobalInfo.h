// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API GlobalInfo
{
public:
	//特殊方块列表
	//一个chunk的所有特殊方块生成完成后，应该清理
	TMap<uint64,int32> SpecialBlocksID;

	GlobalInfo();

	~GlobalInfo();
};
