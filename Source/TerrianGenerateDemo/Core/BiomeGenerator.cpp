// Fill out your copyright notice in the Description page of Project Settings.


#include "BiomeGenerator.h"

void BiomeGenerator::GenerateBiome(Chunk& chunk){
    for (int i = 0; i < MaxBlocksWidth; ++i)
	for (int j = 0; j < MaxBlocksWidth; ++j)
	{
        float temperature = chunk.BlocksTemperature[i][j];
        float humidity = chunk.BlocksHumidity[i][j];

        BiomeType targetBiome = BiomeType::None;

        if(temperature < -0.2f)
			targetBiome = BiomeType::Snow;
		else if(temperature < 0.2f && humidity < 0.05f)
			targetBiome = BiomeType::Stone;
		else if(temperature < 0.2f && humidity < 0.1f)
			targetBiome = BiomeType::Dry;
		else if(temperature < 0.2f || humidity > 0.7f)
			targetBiome = BiomeType::Green;
		else
			targetBiome = BiomeType::Desert;

        chunk.BlocksBiome[i][j] = int32(targetBiome);
    }
}