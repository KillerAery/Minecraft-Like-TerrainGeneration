// Fill out your copyright notice in the Description page of Project Settings.

#include "PathFinder.h"

//记录深度
int PathFinder::depth = 0;        

//深度限制
int PathFinder::depthLimit = 2000;     

//存储OpenNode的内存空间
std::vector<OpenNode> PathFinder::pointList = std::vector<OpenNode>(depthLimit);

//函数接口：检查是否有障碍物（不可走）
std::function<bool(FVector2D pos)> PathFinder::inBarrier = nullptr;
	
//函数接口：计算权值预测公式
std::function<float(FVector2D pos,FVector2D endPos)> PathFinder::predict = nullptr;

TArray<FVector2D> PathFinder::findPath(FVector2D startPos,FVector2D endPos) {
    TArray<FVector2D> road;
    // 创建并开启一个父节点
    openlist.push(createOpenNode(startPos, endPos, 0, nullptr));
    OpenNode* toOpen = nullptr;
    //重复寻找预测和花费之和最小节点开启检查
    while (!openlist.empty()){
        toOpen = openlist.top();
        // 找到终点后，则停止搜索
        if (toOpen->pos.X == endPos.X && toOpen->pos.Y ==endPos.Y) {
            break;
        }
        //若超出一定深度(1000深度)，则搜索失败
        if (depth >= depthLimit) {
            toOpen = nullptr;
            break;
        }
        //将父节点从openlist移除
        openlist.pop();
        open(*toOpen,endPos);
    }

    for (auto rs = toOpen; rs != nullptr; rs = rs->father) {
        road.Push(rs->pos);
    }

    return road;
}


void PathFinder::setConditionInBarrier(std::function<bool(FVector2D pos)>& func){
    inBarrier = func;
}

void PathFinder::setPredict(std::function<float(FVector2D pos,FVector2D endPos)>& func){
    predict = func;
}

OpenNode* PathFinder::createOpenNode(FVector2D pos,FVector2D endPos, int cost, OpenNode* fatherNode){
    pointList.push_back(OpenNode(pos,cost,predict(pos,endPos)+cost,fatherNode));
    return &pointList.back();
}

void PathFinder::open(OpenNode& OpenNode,FVector2D endPos) {
	//八方的位置
	const int direction[8][2] = { {1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,1},{-1,-1},{1,-1}};
    const int cost[8] = {10,10,10,10,14,14,14,14};

    //深度+1
    depth++;

    //检查p点八方的点
    for (int i = 0; i < 8; ++i)
    {   
        FVector2D newPos = OpenNode.pos + FVector2D(direction[i][0],direction[i][1]);
        if (!inBarrier(newPos)) {
            openlist.push(createOpenNode(newPos,endPos, OpenNode.cost + cost[i], &OpenNode));
        }
    }
}


