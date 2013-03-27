#include "item.h"


Item::Item(int itemid)
{
    this->id=itemid;
}


Item::~Item(void)
{
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
    User * userPtr=::getUserPtrbyId(userid);
    EdgeMap::iterator iter=touserv.find(userid);
    if(iter==touserv.end())
    {
        Edge * oldedge=userPtr->tolocv[this->id];//获得在用户对象里面创建的edge对象指针
        touserv.insert(EdgeMap::value_type(userid,oldedge));
    }
}