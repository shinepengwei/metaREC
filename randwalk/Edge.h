#pragma once
#include"inital.h"
class Edge
{
private:
    int weight;
    //std::vector<int> time;
public:
    Edge(void);
    void addEdge(int time);
    void addEdge();
    void print();
    int getWeight();
};
