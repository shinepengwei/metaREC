#pragma once
#include"inital.h"
class Edge
{
private:
    int count;//代表该边的访问次数

    void addCount();
    //std::vector<int> time;
public:
    Edge(void);
    void addEdge(int time);
    void addEdge();
    void print();

    float getWeight();//边的权值
    int getCount();//代表该边的访问次数
};
