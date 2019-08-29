// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TerrianGenerateDemoHUD.generated.h"

UCLASS()
class ATerrianGenerateDemoHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATerrianGenerateDemoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

