#pragma once
#include "base.h"
class Item
{
public:
    Item(int itemid);
    Item(int itemid,int itemType);
    void print(int type,int kongbaiCount=0);

    //获得边集合的不同接口
    EdgeMap* getToLocE() ;
    EdgeMap* getToUserE() ;
    EdgeMap * getToItemE(int type) ;




    void addToLocE(int id,int weightCPUType,int time=0);
    void addToUserE(int id,int weightCUPType,int time=0);


    //获取某一类边的权值之和
    float getAllWeight(int type);
    int getId();
    int getType();


private:
    void addEdge(int id,int weightCPUType,int time,int type);
protected:
    int id;
    int toUserAllWei;//同上
    int type;//ITEMTYPE_USER or ITEMTYPE_LOCATION
    EdgeMap toLocE;//locationId,*Edge
    EdgeMap toUserE;//userId,*Edge
    int toLocAllWei;//指向位置节点的所有的边的权重和,默认值为-1.-1时重新计算，如果不是-1说明已经计算完毕
};

















/*
class User:public Item
{
    friend class Location;
public:
    User(int userid);

    void addToUserE(int userid);
};
//位置-用户边上的权值和时间集合存放在user对象中
class Location:public Item{
public:
    Location(int locationid);
    //无向边，所以要添加两次信息，第一次用户-》位置，第二次位置-》用户。
    //第一次根据权值和时间等信息创建edge对象，第二次直接指向第一次创建的对象即可
    void addToUserE(int userid);

};
*/
