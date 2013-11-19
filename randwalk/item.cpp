#include "item.h"
#include <iostream>
#include <string>
using namespace std;

Item::Item(int itemid)
{
    this->id=itemid;
    this->tolocAllWei=0;
    this->touserAllWei=0;
}


Item::~Item(void)
{
}
EdgeMap* Item::getToLocE(){
    return &(this->tolocv);
}
EdgeMap* Item::getToUserE(){
    return &(this->touserv);
}
EdgeMap * Item::getToItemE(int type){
    if(type==TYPEUSER){
        return this->getToUserE();
    }else if(type==TYPELOCATION){
        return this->getToLocE();
    }else{
        return NULL;
    }
}
EdgeMap * Item::getToItemE(char type){
    if(type=='L'){
        return this->getToLocE();
    }else if(type=='U'){
        return this->getToUserE();
    }else{
        return NULL;
    }
}
int Item::getAllWeight(char type){
    if(type=='L'){
        return this->tolocAllWei;
    }else if(type=='U'){
        return this->touserAllWei;
    }else{
        return -1;
    }
}
User::User(int userid)
    :Item(userid)
{
}
Location::Location(int locationid)
    :Item(locationid){}


User::~User(void)
{
}

void User::addToLocV(int locid,int time){
    this->tolocAllWei++;
    Location* locPtr=::getLocationPtrbyId(locid);
    EdgeMap::iterator iter;
    iter=tolocv.find(locid);
    if(iter!=tolocv.end())
    {//已经有了一条边，增加权值和时间set即可
        iter->second->addEdge(time);
    }else{
        Edge * newedge=new Edge();
        newedge->addEdge(time);
        tolocv.insert(EdgeMap::value_type(locid,newedge));
    }
}

void Location::addToUserV(int userid){
    this->touserAllWei++;
    User * userPtr=::getUserPtrbyId(userid);
    EdgeMap::iterator iter=touserv.find(userid);
    if(iter==touserv.end())
    {
        Edge * oldedge=userPtr->tolocv[this->id];//获得在用户对象里面创建的edge对象指针
        touserv.insert(EdgeMap::value_type(userid,oldedge));
    }
}


void User::addToUserV(int userid){
    this->touserAllWei++;
    User * userPtr=::getUserPtrbyId(userid);
    EdgeMap::iterator iter=this->touserv.find(userid);
    Edge * newedge=NULL;
    if(iter==touserv.end()){
       touserv.insert(EdgeMap::value_type(userid,newedge));
    }
}

void Location::addToLocV(int locid){
    this->tolocAllWei++;
    Location * locPtr=::getLocationPtrbyId(locid);
    EdgeMap::iterator iter=this->tolocv.find(locid);
    if(iter!=tolocv.end()){
        iter->second->addEdge();//增加权值    
    }else{
        Edge * newedge=new Edge();
        newedge->addEdge();
        tolocv.insert(EdgeMap::value_type(locid,newedge));
    }
}

void Item::print(string type){
    cout<<"_____"<<type<<" Begin_______"<<endl;
    cout<<type<<"ID："<<this->id<<endl;
    EdgeMap::iterator iter=this->tolocv.begin();
    cout<<"TO Location Edge:"<<endl;
    for(;iter!=tolocv.end();++iter){
        cout<<"    Location Id:"<<iter->first<<" [";
        if(iter->second!=NULL) iter->second->print();
        cout<<"]"<<endl;
    }
    cout<<"TO User Edge:"<<endl;
    for(iter=this->touserv.begin();iter!=touserv.end();++iter){
        cout<<"    User Id:"<<iter->first<<" ["; 
        if(iter->second!=NULL) iter->second->print();
        cout<<"]"<<endl;
    }
    cout<<"_____Item End_______"<<endl;

}
