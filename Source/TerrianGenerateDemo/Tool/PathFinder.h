// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>
#include <vector>
#include <queue>
#include "CoreMinimal.h"

//open节点结构
struct OpenNode;

//比较器，用以优先队列的指针类型比较
struct OpenPointPtrCompare;

/**
 * A*寻路器
 */
class TERRIANGENERATEDEMO_API PathFinder
{
public:
	//寻路
	static TArray<FVector2D> findPath(FVector2D startPos,FVector2D endPos);
	//设置寻路障碍条件
	static void setConditionInBarrier(std::function<bool(FVector2D pos)> func);
	//设置寻路权值公式
	static void setWeightFormula(std::function<TPair<float,float>(FVector2D pos,FVector2D endPos,float cost)> func);
private:
	//创建一个开启点
	static OpenNode* createOpenNode(FVector2D startPos,FVector2D endPos, float cost,OpenNode* fatherNode);
	//开启检查，检查父节点
	static void open(OpenNode& pointToOpen,FVector2D endPos);
private:
	static int depth;          //记录深度
	static int depthLimit;     //深度限制	

	//函数接口：检查是否有障碍物（不可走）
	static std::function<bool(FVector2D pos)> inBarrier;
	
	//函数接口：计算权值公式
	static std::function<TPair<float,float>(FVector2D pos,FVector2D endPos,float cost)> weightFormula;

	//使用最大优先队列
	static std::priority_queue<OpenNode*, std::vector<OpenNode*>, OpenPointPtrCompare> openlist;

	//存储OpenNode的内存空间
	static std::vector<OpenNode> pointList;
private:
	PathFinder() = delete;
	~PathFinder() = delete;
};

struct OpenNode{
    FVector2D pos;           // 位置
    float cost;              // 耗费值
    float pred;              // 预测值
    OpenNode* father;        // 父节点
    OpenNode() = default;
    OpenNode(FVector2D pos,float cost,float pred, OpenNode* fatherNode)
		:pos(pos),cost(cost),pred(pred),father(fatherNode){}
};

struct OpenPointPtrCompare {
	bool operator()(OpenNode* a, OpenNode* b) {return a->pred > b->pred;}
};