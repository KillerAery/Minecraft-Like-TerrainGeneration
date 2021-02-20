// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/NoiseTool.h"

uint32 NoiseTool::hash11(int32 position)
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
	return mangled;
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

uint32 NoiseTool::hash21(FVector2D position2D)
{
	return hash11(0x651A6BE1 * (int32)position2D.X + (int32)position2D.Y);
}

FVector NoiseTool::hash33(FVector position3D)
{
	return FVector(hash11(position3D.X), hash11(position3D.Y),hash11(position3D.Z));
}

uint32 NoiseTool::hash31(FVector position3D)
{
	return hash11(position3D.X * 0x651A6BE6 - position3D.Y * 0xCB251062 + position3D.Z);
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

float NoiseTool::grad(FVector2D vertex, FVector2D position2D)
{
	return FVector2D::DotProduct(vertex, position2D);
}

//TODO frequence
float NoiseTool::perlinNoise(FVector2D pf,int32 crystalSize,int32 frequence)
{
	//position2D /= crystalSize;

	//FVector2D w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);
	FVector2D w = pf;

	return FMath::Lerp(
		FMath::Lerp(
			grad(GlobalVertex[0],pf),
			grad(GlobalVertex[1],pf - FVector2D(1.0f, 0.0f)),
			w.X),
		FMath::Lerp(
			grad(GlobalVertex[2],pf - FVector2D(0.0f, 1.0f)),
			grad(GlobalVertex[3],pf - FVector2D(1.0f, 1.0f)),
			w.X),
		w.Y);
}

//TODO
float NoiseTool::valueNoise(FVector2D position2D,int32 crystalSize,int32 frequence)
{
	FVector2D pi = FVector2D(floor(position2D.X), floor(position2D.Y));

	FVector2D pf = position2D - pi;
	FVector2D w = pf * pf * (FVector2D(3.0f, 3.0f) - 2.0f * pf);

	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

	return FMath::Clamp<float>(FMath::Lerp(
		FMath::Lerp(hash21(vertex[0]) % 17,
			hash21(vertex[1]) % 17,
			w.X),
		FMath::Lerp(hash21(vertex[2]) % 17,
			hash21(vertex[3]) % 17,
			w.X),
		w.Y), 0, 255);
}

//TODO
float NoiseTool::simplexNoise(FVector2D p,int32 crystalSize,int32 frequence)
{
  const float K1 = 0.366025404; // (sqrt(3)-1)/2;
  const float K2 = -0.211324865; // (sqrt(3)-3)/6;
  
  //坐标偏斜
  FVector2D p2 = p + (p.X + p.Y) * K1;
  
  //向下取整
  FVector2D p2i = FVector2D((int)p2.X,(int)p2.Y);
  
  FVector2D p2f = p2 - p2i;
  FVector2D vertex2Offset = (p2f.X < p2f.Y) ? FVector2D(0, 1) : FVector2D(1, 0);
  
  //顶点变换回单行网格空间
  FVector2D a = p - (p2i + (p2i.X + p2i.Y) * K2 * FVector2D(1,1));
  //FVector2D b = p - (p2i + vertex2Offset + (p2i.X + p2i.Y + 1) * K2 * Vector2(1,1));
  FVector2D b = a - vertex2Offset - K2 * FVector2D(1,1);
  //FVector2D c = p - (p2i + Vecotr2(1,1) + (p2i.X+1 + p2i.Y+1) * K2 * Vector2(1,1));
  FVector2D c = a - FVector2D(1,1) - 2 * K2 * FVector2D(1,1);

  //计算贡献度取和
  float hx = 0.5f - grad(a, a);
  float hy = 0.5f - grad(b, b);
  float hz = 0.5f - grad(c, c);
  hx=hx*hx*hx*hx;
  hy=hy*hy*hy*hy;
  hz=hz*hz*hz*hz;
  FVector n = FVector(hx*grad(a, hash22(p2i)), hy*grad(b, hash22(p2i + vertex2Offset)), hz*grad(c, hash22(p2i + FVector2D(1,1))));
  return n.X + n.Y + n.Z;
}


void NoiseTool::setGlobalVertex(FVector2D vertexs[]){
	for(int32 i = 0;i<4;++i)
	GlobalVertex[i]=vertexs[i];
}

//初始化全局顶点
FVector2D NoiseTool::GlobalVertex[4] = {};
