#include "Edge.h"
#include "base.h"
#include<iostream>
using namespace std;
Edge::Edge(int weightCPUType,int windowTime)
{
    this->type=type;
    this->weightCPUType=weightCPUType;
    count=1;
    for (int i=0;i<WINDOWTIME_COUNT;i++)
    {
        vecWindowTime[i] = -1;
    }
    if (windowTime != -1)
    {
        vecWindowTime[windowTime] = 1;
    }
}


Edge::~Edge(void)
{
}
void Edge::addEdge(int windowTime)
{
    //this->checkinTimes.push_back(time); 
    if (windowTime != -1)
    {
        vecWindowTime[windowTime]++;
    }
    this->count++;
}
void Edge::addEdge()
{
    this->count++;
}

float Edge::getWeight(){//根据权值计算方法和边的次数，计算并返回边的权值
    if(this->weightCPUType==WEIGHTCPUTYPE_LOG){
        return log((double)this->count);
    }else if(this->weightCPUType==WEIGHTCPUTYPE_NORMAL){
        return (float)this->count;
    }else if(weightCPUType==WEIGHTCPUTYPE_BINARY){
        return 1;
    }else if(weightCPUType==WEIGHTCPUTYPE_CLASSIFY){
        //TODO::分阶段计算权值。
    }else{
        return -999999;//ERROR
    }
    return count;
}
float Edge::getWeight(int windowTime){
    if (windowTime == -1)
    {
        return this->getWeight();
    }
    if(this->weightCPUType==WEIGHTCPUTYPE_LOG){
        if (vecWindowTime[windowTime] = -1)
        {
            return 0;
        }
        return log((double)this->vecWindowTime[windowTime]);
    }else if(this->weightCPUType==WEIGHTCPUTYPE_NORMAL){
        if (vecWindowTime[windowTime] = -1)
        {
            return 0;
        }
        return (float)this->vecWindowTime[windowTime];
    }else if(weightCPUType==WEIGHTCPUTYPE_BINARY){
        if (vecWindowTime[windowTime]>0)
        {
            return 1;
        }
        return 0;
    }else if(weightCPUType==WEIGHTCPUTYPE_CLASSIFY){
        //TODO::分阶段计算权值。
    }else{
        return -999999;//ERROR
    }
    return vecWindowTime[windowTime];
}
void Edge::print(){
    cout<<"weight:"<<this->getWeight()<<" "<<"count:"<<count;
    /*
    if(time.size()!=0){
         cout<<"time: ";
         for(int i=0;i<time.size();i++)
             cout<<time[i]<<",";
    }*/
   
    
}