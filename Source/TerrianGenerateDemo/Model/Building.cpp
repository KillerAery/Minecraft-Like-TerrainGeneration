// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

//载入网格资源用
UStaticMesh* ABuilding::BuildingStaticMeshs[MAX_BUILDINGS_NUM]={};

// Sets default values
ABuilding::ABuilding()
{
	//禁止
	PrimaryActorTick.bCanEverTick = false;

	//带一个staticmesh组件
	mMeshComponent = NewObject<UStaticMeshComponent>(this, TEXT("Building"));
	this->SetRootComponent(mMeshComponent);
}


void ABuilding::InitByBuildingID(int32 id){
    if(id <=0|| id>MAX_BUILDINGS_NUM)return;

	//如果没有加载相应的staticmesh，则加载之
	if (!BuildingStaticMeshs[id - 1]) {
		BuildingStaticMeshs[id - 1] = LoadObject<UStaticMesh>(this,
			*("StaticMesh'/Game/Meshes/Buildings/building" +
				FString::FromInt(id) + ".building" +
				FString::FromInt(id) + "'"));
	}
	//设置staticmesh
	mMeshComponent->SetStaticMesh(BuildingStaticMeshs[id - 1]);
}