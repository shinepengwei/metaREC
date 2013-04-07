#include "Edge.h"
#include<iostream>
#include<string>
using namespace std;
void Edge::addEdge(int time)
{
   // this->time.push_back(time);
    this->weight++;
}
void Edge::addEdge()
{
    this->weight++;
}

Edge::Edge(void)
{
    weight=0;
}
int Edge::getWeight(){
    return weight;
}
void Edge::print(){
    cout<<"weight:"<<this->weight<<" ";
    /*
    if(time.size()!=0){
         cout<<"time: ";
         for(int i=0;i<time.size();i++)
             cout<<time[i]<<",";
    }*/
   
    
}
