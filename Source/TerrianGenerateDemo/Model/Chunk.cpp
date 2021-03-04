﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/Chunk.h"

Chunk::Chunk(FVector2D chunkPosition):
	BlocksHeight{0},BlocksTemperature{0},BlocksHumidity{0},BlocksBiome{0},Blocks{nullptr}
{
	ChunkPosition = chunkPosition;
}