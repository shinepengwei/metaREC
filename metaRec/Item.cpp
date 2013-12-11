#include "item.h"
#include "Edge.h"
#include "base.h"
#include <iostream>

Item::Item(int itemid)
{
    this->id=itemid;
    this->toLocAllWei=-1;//表示未初始化，在第一次获取即调用getAllWei()时计算
    this->toUserAllWei=-1;
}
Item::Item(int itemid,int itemType){
    this->id=itemid;
    this->toLocAllWei=-1;//表示未初始化，在第一次获取即调用getAllWei()时计算
    this->toUserAllWei=-1;
    this->type=itemType;
}

int Item::getId(){
    return id;
}
 EdgeMap* Item::getToLocE(){
    return &(this->toLocE);
}
EdgeMap* Item::getToUserE() {
    return &(this->toUserE);
}
EdgeMap * Item::getToItemE(int type){
    if(type==ITEMTYPE_USER){
        return this->getToUserE();
    }else if(type==ITEMTYPE_LOCATION){
        return this->getToLocE();
    }else{
        return NULL;
    }
}


float Item::getAllWeight(int type){
    if(type==ITEMTYPE_LOCATION){
        if(this->toLocAllWei==-1){
            float allwei=0;
            for(EdgeMap::const_iterator iter=this->toLocE.begin();iter!=toLocE.end();++iter){
                allwei+=iter->second->getWeight();
            }
            toLocAllWei=allwei;
            return toLocAllWei;
        }
        return this->toLocAllWei;
    }else if(type==ITEMTYPE_USER){
        if(toUserAllWei==-1){
            float allwei=0;
            for(EdgeMap::const_iterator iter=this->toUserE.begin();iter!=toUserE.end();++iter){
                allwei+=iter->second->getWeight();
            }
            toUserAllWei=allwei;
            return toUserAllWei;
        }
        return this->toUserAllWei;
    }else{
        return -2;
    }
}


void Item::addEdge(int id,int weightCPUType,int time,int type){
    EdgeMap * edgeMap;
    if(type==ITEMTYPE_USER){
        edgeMap=&toUserE;
    }else{
        edgeMap=&toLocE;
    }
    EdgeMap::iterator iter=edgeMap->find(id);
    if(iter!=edgeMap->end())
    {//已经有了一条边，增加权值和时间set即可
        iter->second->addEdge(time);
    }else{
        Edge * newedge=new Edge(weightCPUType);
        edgeMap->insert(EdgeMap::value_type(id,newedge));
    }
}
void Item::addToLocE(int id,int weightCPUType,int time){
    this->addEdge(id,weightCPUType,time,ITEMTYPE_LOCATION);
}
void Item::addToUserE(int id,int weightCPUType,int time){
   this->addEdge(id,weightCPUType,time,ITEMTYPE_USER);
}


void Item::print(int itemtype,int kongbaiCount){
    string kongbai="     ";
    string kongbaijianyi="";
    while (kongbaiCount>0)
    {
        kongbai=kongbai+"     ";
        kongbaijianyi=kongbaijianyi+"     ";
        kongbaiCount--;
    }
    string type;
    if (itemtype==ITEMTYPE_USER)
    {
        type="USER";
    }else{
        type="LOCATION";
    }
    std::cout<<kongbaijianyi<<"_____"<<type<<" Begin_______"<<endl;
    cout<<kongbaijianyi<<type<<"ID："<<this->id<<endl;
    EdgeMap::iterator iter=this->toLocE.begin();
    cout<<kongbai<<"TO Location Edge:"<<endl;
    for(;iter!=toLocE.end();++iter){
        cout<<kongbai<<"    Location Id:"<<iter->first<<" [";
        if(iter->second!=NULL) iter->second->print();
        cout<<"]"<<endl;
    }
    cout<<kongbai<<"TO User Edge:"<<endl;
    for(iter=this->toUserE.begin();iter!=toUserE.end();++iter){
        cout<<kongbai<<"    User Id:"<<iter->first<<" ["; 
        if(iter->second!=NULL) iter->second->print();
        cout<<"]"<<endl;
    }
    cout<<kongbaijianyi<<"_____"<<type<<" End_______"<<endl;
}
int Item::getType(){
    return type;
}
