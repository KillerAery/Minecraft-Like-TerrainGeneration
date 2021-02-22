// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "Block.generated.h"

const int32 MAX_BLOCKS_NUM = 11;

UCLASS()
class TERRIANGENERATEDEMO_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 BlockID = 0;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitByBlockID(int32 id);

	UPROPERTY(EditAnywhere, Category = "Setup")
	UStaticMeshComponent* mMeshComponent;
protected:
	static UStaticMesh* BlockStaticMeshs[MAX_BLOCKS_NUM];
};
