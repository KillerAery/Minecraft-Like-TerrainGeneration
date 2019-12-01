// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

/**
 * 
 */
class TERRIANGENERATEDEMO_API BlockMeshHelper
{
public:
	~BlockMeshHelper();
	static UStaticMesh* GetBlockMesh(int32 id);
private:
	BlockMeshHelper();
	
	TSparseArray<UStaticMesh*> meshMap;

	static BlockMeshHelper sBlockMeshHelper;

	UStaticMesh* LoadStaticMeshFromPath(const FString& Path);
};
