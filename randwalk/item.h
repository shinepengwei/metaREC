#pragma once
#include"inital.h"
#include"Edge.h"
class Item
{
protected:
    int id;
    EdgeMap tolocv;//locationId,*Edge
    EdgeMap touserv;//userId,*Edge
    int tolocAllWei;//指向位置节点的所有的边的权重和
    int touserAllWei;
public:
    Item(int itemid);
    void print(string type);
    EdgeMap* getToLocE();
    EdgeMap* getToUserE();
    EdgeMap * getToItemE(int type);
    EdgeMap * getToItemE(char type);
    int getAllWeight(char type);
    ~Item(void);
};

class User:public Item
{
    friend class Location;
public:
    User(int userid);
    ~User(void);
    void addToLocV(int locid,int time);
    void addToUserV(int userid);
};
//位置-用户边上的权值和时间集合存放在user对象中
class Location:public Item{
public:
    Location(int locationid);
    void addToLocV(int locid);
    //无向边，所以要添加两次信息，第一次用户-》位置，第二次位置-》用户。
    //第一次根据权值和时间等信息创建edge对象，第二次直接指向第一次创建的对象即可
    void addToUserV(int userid);

};
