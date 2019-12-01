// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockMeshHelper.h"
//#include "ConstructorHelpers.h"

const int BlockCount = 10;

BlockMeshHelper BlockMeshHelper::sBlockMeshHelper;

BlockMeshHelper::~BlockMeshHelper()
{
}

UStaticMesh* BlockMeshHelper::GetBlockMesh(int32 id)
{
	return sBlockMeshHelper.meshMap[id];
}

BlockMeshHelper::BlockMeshHelper()
{
	for (int i = 0; i <= BlockCount; ++i) {
		//FString NewString = FString::Printf(TEXT("/Game/Meshes/Block%d.uasset"), i); 
		//meshMap.Add(LoadStaticMeshFromPath(NewString));
		meshMap.Add(nullptr);
	}
}

UStaticMesh* BlockMeshHelper::LoadStaticMeshFromPath(const FString& Path)
{

	if (Path.IsEmpty()) return nullptr;

	return LoadObject<UStaticMesh>(nullptr, *(Path));
}
