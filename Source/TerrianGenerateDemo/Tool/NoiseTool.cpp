// Fill out your copyright notice in the Description page of Project Settings.

#include "Tool/NoiseTool.h"

//初始化全局顶点
FVector2D NoiseTool::GlobalVertex[4] = {};
FVector NoiseTool::GlobalVertex3D[3] = {};

//初始化全局偏移
FVector2D NoiseTool::GlobalOffset = FVector2D::ZeroVector;
FVector NoiseTool::GlobalOffset3D = FVector::ZeroVector;

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

FVector NoiseTool::hash33(FVector position)
{
	FVector v(
		hash11((int32)position.X^0x651A6BE3+(int32)position.Y^0x218A6147-(int32)position.Z^0x118A5191)%1024,
		hash11((int32)position.X^0x118A5191-(int32)position.Y^0x218AE247+(int32)position.Z^0x2B8AE147)%1024,
		hash11((int32)position.X^0x21613122-(int32)position.Y^0x118A5191-(int32)position.Z^0x218AE247)%1024
		);
	v/=1024.0f;
	return v;
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

//TODO
float NoiseTool::simplexNoise(FVector p){
 	p = p + GlobalOffset3D;

	// Skew the input space to determine which simplex cell we're in
 	const float F3 = 1.0/3.0;

 	float s = (p.X+p.Y+p.Z)*F3; // Very nice and simple skew factor for 3D
	FVector pi = FVector(floor(p.X+s),floor(p.Y+s),floor(p.Z+s));

 	const float G3 = 1.0/6.0; // Very nice and simple unskew factor, too
 	float t = (pi.X+pi.Y+pi.Z)*G3;
	 
	// Unskew the cell origin back to (x,y,z) space
	FVector p0 = FVector(p.X-pi.X+t,p.Y-pi.Y+t,p.Z-pi.Z+t);// The x,y,z distances from the cell origin

 	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
 	// Determine which simplex we are in.
	FVector pi1;
	FVector pi2;
 	// Offsets for second corner of simplex in (i,j,k) coords
 	if(p0.X>=p0.Y) {
 		if(p0.Y>=p0.Z){ 
			pi1 = FVector(1,0,0);
			pi2 = FVector(1,1,0);
		} // X Y Z order
 		else if(p0.X>=p0.Z) {
			pi1 = FVector(1,0,0);
			pi2 = FVector(1,0,1);
		} // X Z Y order
 		else {
			pi1 = FVector(0,0,1);
			pi2 = FVector(1,0,1);
		} // Z X Y order
 	}
 	else { // p0.X<p0.Y
 		if(p0.Y<p0.Z) {			 
			pi1 = FVector(0,0,1);
			pi2 = FVector(0,1,1); 
		} // Z Y X order
 		else if(p0.X<p0.Z) {
			pi1 = FVector(0,1,0);
			pi2 = FVector(0,1,1); 
		} // Y Z X order
 		else {
			pi1 = FVector(0,1,0);
			pi2 = FVector(1,1,0); 
		} // Y X Z order
 	}

 	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
 	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
 	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
 	// c = 1/6.
	FVector p1 = p0 - pi1 + FVector::OneVector*(G3);// Offsets for second corner in (x,y,z) coords
	FVector p2 = p0 - pi2 + FVector::OneVector*(2.0f*G3);// Offsets for third corner in (x,y,z) coords
	FVector p3 = p0 - FVector::OneVector*(-1.0f+3.0f*G3);// Offsets for last corner in (x,y,z) coords

 	// Calculate the contribution from the four corners
 	float t0 = 0.6 - p0.X*p0.X - p0.Y*p0.Y - p0.Z*p0.Z;
	float n0, n1, n2, n3; // Noise contributions from the four corners
 	if(t0<0) n0 = 0.0;
 	else {
 		t0 *= t0;
 		n0 = t0 * t0 * FVector::DotProduct(hash33(pi),p0);
 	}
 	float t1 = 0.6 - FVector::DotProduct(p1,p1);
 	if(t1<0) n1 = 0.0;
 	else {
 		t1 *= t1;
 		n1 = t1 * t1 * FVector::DotProduct(hash33(FVector(pi.X+pi1.X,pi.Y+pi1.Y,pi.Z+pi1.Z)),p1);
 	}
 	float t2 = 0.6 - FVector::DotProduct(p2,p2);
 	if(t2<0) n2 = 0.0;
 	else {
 		t2 *= t2;
 		n2 = t2 * t2 * FVector::DotProduct(hash33(FVector(pi.X+pi2.X,pi.Y+pi2.Y,pi.Z+pi2.Z)),p2);
 	}
 	float t3 = 0.6 - FVector::DotProduct(p3,p3);
 	if(t3<0) n3 = 0.0;
 	else {
 		t3 *= t3;
 		n3 = t3 * t3 * FVector::DotProduct(hash33(FVector(pi.X+1,pi.Y+1,pi.Z+1)),p3);
 	}
 	// Add contributions from each corner to get the final noise value.
 	// The result is scaled to stay just inside [-1,1]
 	return 32.0f*(n0 + n1 + n2 + n3);
}


void NoiseTool::prehandlePerlinNoise(FVector2D position2D, int32 crystalSize){
	FVector2D pi = FVector2D(floor(position2D.X / crystalSize), floor(position2D.Y / crystalSize));
	FVector2D vertex[4] = { {pi.X,pi.Y},{pi.X + 1,pi.Y},{pi.X,pi.Y + 1},{pi.X + 1,pi.Y + 1} };

	for(int32 i = 0;i<4;++i)
	GlobalVertex[i]=NoiseTool::hash22(vertex[i]);
	
	GlobalOffset = (position2D-pi*crystalSize)/crystalSize;
}

//TODO
void NoiseTool::prehandleValueNoise(FVector2D position2D, int32 crystalSize){

}

void NoiseTool::prehandleSimplexNoise(FVector2D position2D, int32 crystalSize){
	GlobalOffset = position2D / crystalSize;
}

void NoiseTool::prehandleSimplexNoise(FVector position,int32 crystalSize){
	GlobalOffset3D = position / crystalSize;
}

//二阶bezier曲线 t应[0.0f~1.0f]
FVector2D NoiseTool::bezier(FVector2D p0,FVector2D p1,FVector2D p2,float t){
	return ((1-t)*(1-t)*p0+t*(1-t)*p1+t*t*p2);
}

//三阶bezier曲线 t应[0.0f~1.0f]
FVector2D NoiseTool::bezier(FVector2D p0,FVector2D p1,FVector2D p2,FVector2D p3,float t){
	return bezier(p0,p1,p2,t)*(1-t)+bezier(p1,p2,p3,t)*t;
}

//三维坐标转一维坐标（压缩坐标）
uint64 NoiseTool::Index(int32 x,int32 y,int32 z){
		const int32 offset = 16384;
		uint64 t =
		uint64(x+offset)<<40 |
		uint64(y+offset)<<20 | 
		uint64(z+offset);
		return t;
	}

//一维坐标转三维坐标（解压坐标）
FVector NoiseTool::UnIndex(uint64 index){
	const int32 offset = 16384;
	return FVector(
		int32(index>>40)-offset,
		int32((index>>20)&0xFFFFF)-offset,
		int32(index&0xFFFFF)-offset
	);
}