#include "Socialnet.h"
#include<fstream>
#include <iostream>
#include<ctime>
extern int isDebug;

//处理文件用到的字符串分割函数,定义在tool.cpp
vector<std::string> split(string str);
int Socialnet::instanceCount=0;

Socialnet::Socialnet(int weightCpuType,bool hasLLRelation):hasLLRelation(hasLLRelation)
{
    this->weightCpuType=weightCpuType;   
}
Socialnet *Socialnet::createSocialnet(string chinkinFileName,string friendFileName,int weightCpuType,bool hasLLRelation) {
    if (instanceCount==1)
    {
        return NULL;//已经创建了一个实力对象
    }
    Socialnet * socNet=new Socialnet(weightCpuType,hasLLRelation);
    socNet->readCheckinData(chinkinFileName);
    socNet->readFriendData(friendFileName);
    instanceCount=1;
    return socNet;
}
void Socialnet::readCheckinData(string chinkinFileName){
     //读入签到数据
    ifstream checkinfile(chinkinFileName);
    char buffer[100];
    int i=0;
    int lastUserId=0,lastLocId=0,lastTimeSec=0;//用于判断比较获得位置-位置边
    //根据用户的签到信息，读用户、位置节点信息以及用户-位置和位置-位置的边信息
    cout<<"读取签到文件"<<endl;
    while(true){
        i++;
        if(i%100000==0) cout<<i<<",";
        if(checkinfile.eof()){
            break;
        }
        checkinfile.getline(buffer,100);
        string str(buffer);
        if (isDebug) cout<<"读取一条记录:"<<endl<<str<<endl;
        vector<std::string> result=split(str);
        //一行字符串分割后转意
        struct tm t; 
        t.tm_year=atoi(result[1].substr(0,4).data())-1900;
        t.tm_mon=atoi(result[1].substr(5,2).data()-1);
        t.tm_mday=atoi(result[1].substr(8,2).data());
        t.tm_hour=atoi(result[1].substr(11,2).data());
        t.tm_min=atoi(result[1].substr(13,2).data());
        t.tm_sec=atoi(result[1].substr(15,2).data());
        t.tm_isdst=0;
        time_t tsec;
        tsec=mktime(&t);
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());

        //插入签到数据
        addCheckin(userid,locid);

        if (hasLLRelation)
        {
            //插入位置-位置边数据
            if(lastUserId==userid&&(lastTimeSec-tsec)<this->time_iterval&&lastLocId!=locid&&lastLocId!=0){
                addLLRelation(lastLocId,locid);
            }
        }
        lastUserId=userid;
        lastTimeSec=tsec;
        lastLocId=locid;

        if (isDebug) cout<<"此条记录处理结束！=================================="<<endl<<endl<<endl<<endl;

    }
    cout<<endl;
}
void Socialnet::readFriendData(string friendFileName){
    //读用户关系数据
    ifstream friendfile(friendFileName);
    char buffer[100];
    int i=0;
    cout<<"读取好友关系文件"<<endl;
    int xxbug=0;
    while(xxbug<10000){
        // xxbug++;
        i++;
        if(i%100000==0) cout<<i<<",";
        if(friendfile.eof()){
            break;
        }
        friendfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=2){
            break;
        }
        int fuid=atoi(result[0].data());
        int suid=atoi(result[1].data());
        addFriend(fuid,suid);
        if (isDebug) cout<<"此条记录处理结束！=================================="<<endl<<endl<<endl<<endl;
    }
    cout<<endl;
}
Socialnet::~Socialnet(void)
{
    instanceCount=0;
}

Item * Socialnet::getItemPtrById(int id,int itemType,bool isAllowNewType){
    ItemMap * itemList;
    if(itemType==ITEMTYPE_USER){
        itemList=&userList;
    }else{
        itemList=&locList;
    }
    ItemMap::iterator iter=itemList->find(id);
    if(iter!=itemList->end()){
        return iter->second;
    }
    if(isAllowNewType==false) return NULL;

    Item * newItem=new Item(id,itemType);
    itemList->insert(ItemMap::value_type(id,newItem));
    if (isDebug)
    {
        cout<<"创建一个新的ITEM"<<endl;
        newItem->print(itemType);
    }
    return newItem;
}
bool Socialnet::isNeighbor(int fromId,int fromItemType,int toId,int toItemType){
    Item * item;
    item=this->getItemPtrById(fromId,fromItemType);
    EdgeMap* emp=item->getToItemE(toItemType);
    EdgeMap::iterator iter=emp->find(toId);
    if(iter!=emp->end()){
        return true;
    }else{
        return false;
    }
}

void Socialnet::addCheckin(int userId,int locId){
    Item * user=getItemPtrById(userId,ITEMTYPE_USER,ALLOW_INSERT_IF_NULL);
    Item * loc=getItemPtrById(locId,ITEMTYPE_LOCATION,ALLOW_INSERT_IF_NULL);
    //网络中插入用户-位置边数据
    user->addToLocE(locId,weightCpuType);
    loc->addToUserE(userId,weightCpuType);
    if (isDebug)
    {
        cout<<"添加用户-位置边:user-"<<userId<<",loc-"<<locId<<endl;
        user->print(ITEMTYPE_USER);
        loc->print(ITEMTYPE_LOCATION);
        cout<<"=================="<<endl;
    }
}
void Socialnet::addLLRelation(int locId1,int locId2){
    Item *loc1=getItemPtrById(locId1,ITEMTYPE_LOCATION);
    Item * loc2=getItemPtrById(locId2,ITEMTYPE_LOCATION);
    if (loc1!=loc2&&loc1!=NULL&&loc2!=NULL)
    {
        loc1->addToLocE(locId2,weightCpuType);
        //loc2->addToLocE(locId1,weightCpuType);L-L边是有方向的
        if (isDebug)
        {
            cout<<"添加位置-位置边："<<locId1<<"-"<<locId2<<endl;
            loc1->print(ITEMTYPE_LOCATION);
            loc2->print(ITEMTYPE_LOCATION);
            cout<<"================"<<endl;
        }
    }
}

void Socialnet::addFriend(int userId,int user2id){
    Item *fuser=getItemPtrById(userId,ITEMTYPE_USER);
    Item *suser=getItemPtrById(user2id,ITEMTYPE_USER);
    if(fuser!=suser&&fuser!=NULL&&suser!=NULL){
        fuser->addToUserE(user2id,WEIGHTCPUTYPE_BINARY);
        suser->addToUserE(userId,WEIGHTCPUTYPE_BINARY);
        if (isDebug)
        {
            cout<<"添加好友关系："<<endl;
            fuser->print(ITEMTYPE_USER);
            suser->print(ITEMTYPE_USER);
            cout<<"=================="<<endl;
        }
    }
}