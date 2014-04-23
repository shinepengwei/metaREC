#pragma once
//#include <list>
#include "base.h"
class Edge
{
public:
    Edge(int weightCPUType,int time=-1);
    void addEdge(int time);
    void addEdge();
    void print();
    float getWeight();//根据权值计算方法和边的次数，计算并返回边的权值
    float getWeight(int windowTime);
    ~Edge(void);
private:
    int weightCPUType;//权重计算方法
    int count;//边的次数。
    int type;//边的类型

    //list<int> checkinTimes;
    int vecWindowTime[WINDOWTIME_COUNT] ;
};

