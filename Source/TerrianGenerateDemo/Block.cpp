// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlock::Initialize(int32 ID)
{
	BlockID = ID;

	UStaticMeshComponent* meshComponent;
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(meshComponent);


}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

