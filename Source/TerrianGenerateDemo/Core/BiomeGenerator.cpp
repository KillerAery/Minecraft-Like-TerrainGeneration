// Fill out your copyright notice in the Description page of Project Settings.


#include "BiomeGenerator.h"



/*
	None = 0
	雪地 Snow = 1
	草地 Green = 2
	泥地 Dry = 3
    石地 Stone = 4
	沙漠 Desert = 5
*/

void BiomeGenerator::GenerateBiome(Chunk& chunk){
    for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];

        BiomeType targetBiome = BiomeType::None;

        if(temperature < -0.2f){targetBiome = BiomeType::Snow;}
		else if(temperature < 0.20f && humidity < 0.05f){targetBiome = BiomeType::Stone;}
		else if(temperature < 0.20f && humidity < 0.1f){targetBiome = BiomeType::Dry;}
		else if(temperature < 0.20f){targetBiome = BiomeType::Green;}
		else if(humidity > 0.7f){targetBiome = BiomeType::Green;}
		else{targetBiome = BiomeType::Desert;}

        chunk.BlocksBiome[i][j] = (int32)targetBiome;
    }
}