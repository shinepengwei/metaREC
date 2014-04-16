#pragma once
#include "base.h"
#include "Item.h"

#define PI 3.141592653f
#define EARTH_RADIUS 6378.137f
class GeograRecommend;

class Socialnet
{
    friend class GeograRecommend;
private:
    static double rad(double d)
    {
        return d * PI / 180.0;
    }
    

public:
    ~Socialnet(void);
    Item * getItemPtrById(int id,int itemType,bool isAllowNewType=false, float longitude = -1, float latitude = -1);
    bool isNeighbor(int fromId,int fromItemType,int toId,int toItemType);
    static Socialnet * createSocialnet(string chinkinFileName,string friendFileName,
        int weightCpuType=WEIGHTCPUTYPE_NORMAL,
        bool hasLLRelation=true,
        int time_iterval=24
        ) ;
    //TODO
   
    bool const hasLLRelation;
    //根据经纬度返回亮点的距离。
    static float GetDistance(float lng1, float lat1, float lng2, float lat2){
        double radLat1 = rad(lat1);
        double radLat2 = rad(lat2);
        double a = radLat1 - radLat2;
        double b = rad(lng1) - rad(lng2);
        double s = 2 * sin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
        s = s * EARTH_RADIUS;
        return s;
    }
private:
    void GetNearestLocListMap(ItemMap & locList,int locId);
    ItemMap * getLoclist();
    Socialnet(int,bool);
    void readCheckinData(string chinkinFileName);
    void readFriendData(string friendFileNmae);
    void addCheckin(int userId,int locId, float longitude, float latitude,int checkinTime);
    void addFriend(int userId,int user2Id);
    void addLLRelation(int locId,int locId2);
    
private:
    ItemMap userList;
    ItemMap locList;

    int weightCpuType;//边的权值计算方法
    

    static int instanceCount;//单例模式
    int time_iterval;
};