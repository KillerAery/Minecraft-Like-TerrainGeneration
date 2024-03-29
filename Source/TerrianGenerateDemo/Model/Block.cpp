﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Model/Block.h"

UStaticMesh* ABlock::BlockStaticMeshs[MAX_BLOCKS_NUM] = { nullptr };

// Sets default values
ABlock::ABlock():BlockID(0)
{
	//禁止
	PrimaryActorTick.bCanEverTick = false;

	//带一个staticmesh组件
	mMeshComponent = NewObject<UStaticMeshComponent>(this, TEXT("Block"));
	this->SetRootComponent(mMeshComponent);
}


int32 ABlock::GetBlockID(){
	return BlockID;
}

void ABlock::InitByBlockID(int32 id)
{	
	if (!mMeshComponent)return;

	//如果没有加载相应的staticmesh，则加载之
	if (!BlockStaticMeshs[id - 1]) {
		BlockStaticMeshs[id - 1] = LoadObject<UStaticMesh>(this,
			*("StaticMesh'/Game/Meshes/Block" +
				FString::FromInt(id) + ".Block" +
				FString::FromInt(id) + "'"));
	}
	//设置staticmesh
	mMeshComponent->SetStaticMesh(BlockStaticMeshs[id - 1]);
}

