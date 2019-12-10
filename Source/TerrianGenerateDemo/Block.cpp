// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"

UStaticMesh* ABlock::BlockStaticMeshs[MAX_BLOCKS_NUM]{ nullptr };

// Sets default values
ABlock::ABlock()
{
	//禁止
	PrimaryActorTick.bCanEverTick = false;

	//带一个staticmesh组件
	mMeshComponent = NewObject<UStaticMeshComponent>(this, TEXT("Block"));
	mMeshComponent->RegisterComponent();
	RootComponent = mMeshComponent;
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
