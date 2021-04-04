// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Building.generated.h"

const int32 MAX_BUILDINGS_NUM = 3;

UCLASS()
class TERRIANGENERATEDEMO_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	void InitByBuildingID(int32 id);
	
protected:
	UPROPERTY(EditAnywhere, Category = "Setup")
	UStaticMeshComponent* mMeshComponent;

	//载入网格资源用
	static UStaticMesh* BuildingStaticMeshs[MAX_BUILDINGS_NUM];
};
