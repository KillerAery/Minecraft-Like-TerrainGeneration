// Fill out your copyright notice in the Description page of Project Settings.

#include "Tool/NoiseTool.h"

//初始化全局顶点
FVector2D NoiseTool::GlobalVertex[4] = {};

//初始化全局偏移
FVector2D NoiseTool::GlobalOffset = FVector2D::ZeroVector;

//初始化全局种子
int32 NoiseTool::GlobalSeed = 107;

int32 NoiseTool::hash11(int32 position)
{
	const uint32 BIT_NOISE1 = 0x85297A4D;
	const uint32 BIT_NOISE2 = 0x68E31DA4;
	const uint32 BIT_NOISE3 = 0x1B56C4E9;
	uint32 mangled = position;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled%1024;
}

int32 NoiseTool::hash11WithGlobalSeed(int32 position){
	const uint32 BIT_NOISE1 = 0x85297A4D;
	const uint32 BIT_NOISE2 = 0x68E31DA4;
	const uint32 BIT_NOISE3 = 0x1B56C4E9;
	uint32 mangled = position ^ GlobalSeed;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled%1024;
}

FVector2D NoiseTool::hash22(FVector2D position2D)
{
	FVector2D v(
		hash11(0x651A6BE3*(int32)position2D.X-(int32)position2D.Y)%1024,
		hash11((int32)position2D.X*(int32)position2D.Y+0x218AE247)%1024
		);
	v/=1024.0f;
	return v;
}

int32 NoiseTool::hash21(FVector2D position2D)
{
	return hash11(0x651A6BE1 * (int32)position2D.X + (int32)position2D.Y)%1024;
}

FVector NoiseTool::hash33(FVector position3D)
{
	return FVector(hash11(position3D.X), hash11(position3D.Y),hash11(position3D.Z));
}

int32 NoiseTool::hash31(FVector position3D)
{
	return hash11(position3D.X * 0x651A6BE6 - position3D.Y * 0xCB251062 + position3D.Z);
}

void NoiseTool::setSeed(int32 seed){
	GlobalSeed = seed;
}

int32 NoiseTool::randInt(FVector2D position){
	position = position*1024;
	return hash11WithGlobalSeed(0x651A6BE1 * (int32)position.X + (int32)position.Y)%1024;
}
	
float NoiseTool::rand(FVector2D position){
	position = position*1024;
	return hash11WithGlobalSeed(0x651A6BE1 * (int32)position.X + (int32)position.Y)%1024/1024.0f;
}

float NoiseTool::grad(FVector2D vertex, FVector2D position2D)
{
	return FVector2D::DotProduct(vertex, position2D);
}

float NoiseTool::grad_f(FVector2D vertex, FVector2D p){
	switch((0x651A6BE1 * (int32)vertex.X + (int32)vertex.Y) % 4)
    {
      case 1: return  p.X + p.Y;  //代表梯度向量(1,1)
      case 2: return -p.X + p.Y;  //代表梯度向量(-1,1)
      case 3: return  p.X - p.Y;  //代表梯度向量(1,-1)
      case 4: return -p.X - p.Y;  //代表梯度向量(-1,-1)
      default: return 0; // never happens
    }
}


float NoiseTool::perlinNoise(FVector2D p)
{
	p = GlobalOffset + p;

	FVector2D w = p;
	//FVector2D w = p * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

	return 
	FMath::Clamp<float>(
		FMath::Lerp(
		FMath::Lerp(
			grad(GlobalVertex[0],p),
			grad(GlobalVertex[1],p - FVector2D(1.0f, 0.0f)),
			w.X),
		FMath::Lerp(
			grad(GlobalVertex[2],p - FVector2D(0.0f, 1.0f)),
			grad(GlobalVertex[3],p - FVector2D(1.0f, 1.0f)),
			w.X),
		w.Y)
	,-1,1)
	;
}

//TODO
float NoiseTool::valueNoise(FVector2D position2D)
{
	FVector2D pi = FVector2D(floor(position2D.X), floor(position2D.Y));

	FVector2D pf = position2D - pi;
	FVector2D w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

	return FMath::Clamp<float>(
		FMath::Lerp(
		FMath::Lerp(hash21(vertex[0]) % 17,
			hash21(vertex[1]) % 17,
			w.X),
		FMath::Lerp(hash21(vertex[2]) % 17,
			hash21(vertex[3]) % 17,
			w.X),
		w.Y), 0, 255);
}


float NoiseTool::simplexNoise(FVector2D p)
{
  p = p + GlobalOffset;

  const float K1 = 0.366025404; // (sqrt(3)-1)/2;
  const float K2 = 0.211324865; // (3-sqrt(3))/6;
  //坐标偏斜
  float s = (p.X + p.Y) * K1;
  FVector2D pi = FVector2D(floor(p.X+s),floor(p.Y+s));
  float t = (pi.X + pi.Y) *K2;
  FVector2D pf = p-pi+t*FVector2D::UnitVector;
  FVector2D vertex2Offset = (pf.X < pf.Y) ? FVector2D(0, 1) : FVector2D(1, 0);
  
  //顶点变换回单行网格空间
  FVector2D dist1 = pf;
  FVector2D dist2 = pf - vertex2Offset + K2 * FVector2D::UnitVector;
  FVector2D dist3 = pf - FVector2D(1,1) + 2 * K2 * FVector2D::UnitVector;

  //计算贡献度取和
  float hx = 0.5f - FVector2D::DotProduct(dist1, dist1);
  float hy = 0.5f - FVector2D::DotProduct(dist2, dist2);
  float hz = 0.5f - FVector2D::DotProduct(dist3, dist3);

  hx*=hx;
  hy*=hy;
  hz*=hz;

  return FMath::Clamp<float>(
		70*(hx*hx*FVector2D::DotProduct(dist1, hash22(pi)) 
	  	+hy*hy*FVector2D::DotProduct(dist2, hash22(pi + vertex2Offset))
	  	+hz*hz*FVector2D::DotProduct(dist3, hash22(pi + FVector2D(1,1)))
  		)
		,-1,1);
}


void NoiseTool::prehandlePerlinNoise(FVector2D position2D, int32 crystalSize,int32 frequence){
	FVector2D pi = FVector2D(floor(position2D.X / crystalSize), floor(position2D.Y / crystalSize));
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

	for(int32 i = 0;i<4;++i)
	GlobalVertex[i]=NoiseTool::hash22(vertex[i]);
	
	GlobalOffset = (position2D-pi*crystalSize)/crystalSize;
}

//TODO
void NoiseTool::prehandleValueNoise(FVector2D position2D, int32 crystalSize,int32 frequence){

}

void NoiseTool::prehandleSimplexNoise(FVector2D position2D, int32 crystalSize,int32 frequence){
	FVector2D pi = FVector2D(floor(position2D.X / crystalSize), floor(position2D.Y / crystalSize));
	GlobalOffset = position2D / crystalSize;
}


