// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

int32 ABlock::tempID = 1;

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(meshComponent);

	BlockID = tempID;
	FString NewString = FString::Printf(TEXT("/Game/Meshes/Block%d"), BlockID);
	ConstructorHelpers::FObjectFinder<UStaticMesh> staticAsset(*NewString);
	if (staticAsset.Succeeded()) {
		meshComponent->SetStaticMesh(staticAsset.Object);
	}

	//Initialize(1);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlock::Initialize(int32 ID)
{
	tempID = ID;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

