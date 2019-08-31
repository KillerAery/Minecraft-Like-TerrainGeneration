// Fill out your copyright notice in the Description page of Project Settings.

#include "NoiseTool.h"

uint32_t NoiseTool::hash11(int32 position)
{
	const unsigned int BIT_NOISE1 = 0x85297A4D;
	const unsigned int BIT_NOISE2 = 0x68E31DA4;
	const unsigned int BIT_NOISE3 = 0x1B56C4E9;
	unsigned int mangled = position;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled % 71;
}

FVector2D NoiseTool::hash22(FVector2D position2D)
{
	uint32_t h = hash11(position2D.X);
	return FVector2D(h, hash11(h+position2D.Y)).GetSafeNormal() * 100;
}

uint32_t NoiseTool::hash21(FVector2D position2D)
{
	return hash11(position2D.X * 0x651A6BE6 + position2D.Y);
}

FVector NoiseTool::hash33(FVector position3D)
{
	return FVector(hash11(position3D.X), hash11(position3D.Y),hash11(position3D.Z));
}

uint32_t NoiseTool::hash31(FVector position3D)
{
	return hash11(position3D.X * 0x651A6BE6 - position3D.Y * 0xCB251062 + position3D.Z);
}

//求梯度值（本质是求顶点代表的梯度向量与距离向量的点积）
float NoiseTool::grad(FVector2D vertex, FVector2D position2D)
{
	return FVector2D::DotProduct(hash22(vertex), position2D);
}

//二维柏林噪声
float NoiseTool::perlinNoise(FVector2D position2D)
{	
	//向量两个纬度值向下取整
	FVector2D pi =FVector2D((int32)position2D.X,(int32)position2D.Y);

	//计算缓和曲线
	FVector2D pf = position2D - pi;
	FVector2D w = pf * pf * (FVector2D(3.0f,3.0f) - 2.0f * pf);

	//二维晶体格四个顶点
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

	return FMath::Clamp<float>(FMath::Lerp(
		FMath::Lerp(grad(vertex[0], pf),
			grad(vertex[1], pf - FVector2D(1.0f, 0.0f)),
			w.X),
		FMath::Lerp(grad(vertex[2], pf - FVector2D(0.0f, 1.0f)),
			grad(vertex[3], pf - FVector2D(1.0f, 1.0f)),
			w.X),
		w.Y) + 32,0,255);
}