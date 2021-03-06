#include "Socialnet.h"
#include<fstream>
#include <iostream>
#include<ctime>
#include "Item.h"
#include "atltime.h"
extern int isDebug;

//处理文件用到的字符串分割函数,定义在tool.cpp
vector<std::string> split(string str);
int Socialnet::instanceCount=0;
void Socialnet::calculateUserDensity(){
    int userCount = this->userList.size();
    int relationCount =0;
    for (ItemMap::const_iterator userIter=userList.begin();userIter != userList.end();userIter++)
    {
        Item * user = userIter->second;
        relationCount +=user->getAllWeight(ITEMTYPE_USER);
    }
    cout<<"用户节点数量："<<userCount<<endl;
    cout<<"好友关系数量"<<relationCount/2<<endl;
    cout<<"用户好友网络的网络密度为："<<(double)relationCount/(2*userCount*userCount)<<endl;
}
void Socialnet::calculateUserLocDensity(){
    int userCount = this->userList.size();
    int locCount = this->locList.size();
    int relationCount =0;
    for (ItemMap::const_iterator userIter=userList.begin();userIter != userList.end();userIter++)
    {
        Item * user = userIter->second;
        relationCount +=user->getAllWeight(ITEMTYPE_LOCATION);
    }
    cout<<"用户节点数量："<<userCount<<endl;
    cout<<"位置节点数量："<<locCount<<endl;
    cout<<"签到（不重复）数量"<<relationCount/2<<endl;
    cout<<"签到网络的网络密度为："<<(double)relationCount/(2*userCount*locCount)<<endl;
}
Socialnet::Socialnet(int weightCpuType,bool hasLLRelation):hasLLRelation(hasLLRelation)
{
    this->weightCpuType=weightCpuType;   
}
Socialnet *Socialnet::createSocialnet(string chinkinFileName,string friendFileName,int weightCpuType,bool hasLLRelation,int time_iterval) {
    if (instanceCount==1)
    {
        return NULL;//已经创建了一个实力对象
    }
    Socialnet * socNet=new Socialnet(weightCpuType,hasLLRelation);
    socNet->time_iterval=time_iterval*60*60;
    socNet->readCheckinData(chinkinFileName);
    socNet->readFriendData(friendFileName);
    instanceCount=1;
    return socNet;
}
int Socialnet::getWindowTime(int dayOfWeek, int hourOfDay){
    int rt=-1;//时间框架
    if (dayOfWeek != 1 && dayOfWeek != 7)
    {
        //工作日
        if (hourOfDay>=23 ||hourOfDay <9)
        {
            rt = 0;
        }else if (hourOfDay>=9 && hourOfDay <18)
        {
            rt=2;
        }else{
            rt = 3;
        }
    }else{
        //周末
        if (hourOfDay>=23 ||hourOfDay <9)
        {
            rt = 0;
        }else if (hourOfDay>=9 && hourOfDay <18)
        {
            rt=2;
        }else{
            rt = 3;
        }
    }
    return rt;
}
void Socialnet::readCheckinData(string chinkinFileName){
     //读入签到数据
    ifstream checkinfile(chinkinFileName);
    char buffer[100];
    int i=0;
    int lastUserId=0,lastLocId=0,lastTimeSec=0;//用于判断比较获得位置-位置边
    //根据用户的签到信息，读用户、位置节点信息以及用户-位置和位置-位置的边信息
    cout<<"读取签到文件:"<<chinkinFileName<<endl;
    while(true){
        i++;

        if (i%100000 == 0) cout<<i<<",";

        if(checkinfile.eof()){
            break;
        }
        checkinfile.getline(buffer,100);
        string str(buffer);
        if (isDebug) cout<<"读取一条记录:"<<endl<<str<<endl;
        vector<std::string> result=split(str);
        if(result.size()!=5)
            break;
        //一行字符串分割后转意
        struct tm t; 
        t.tm_year=atoi(result[1].substr(0,4).data())-1900;
        t.tm_mon=atoi(result[1].substr(5,2).data())-1;
        t.tm_mday=atoi(result[1].substr(8,2).data());
        t.tm_hour=atoi(result[1].substr(11,2).data());
        t.tm_min=atoi(result[1].substr(14,2).data());
        t.tm_sec=atoi(result[1].substr(17,2).data());
        
        t.tm_isdst=0;
        time_t tsec;
        tsec=mktime(&t);
        CTime ct(tsec);
        int dayOfWeek = ct.GetDayOfWeek();
        int hourOfDay = ct.GetHour();
        
        if (isDebug) cout<<"时间:"<<t.tm_year<<"年"<<t.tm_mon<<"月"<<t.tm_mday<<"日"<<t.tm_hour<<"时"<<t.tm_min<<"分:"<<tsec<<endl;
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());


        float longitude = atof(result[3].data());
        float latitude = atof(result[2].data());
        //插入签到数据
        addCheckin(userid,locid,longitude,latitude,getWindowTime(dayOfWeek,hourOfDay));


        //插入位置-位置边数据
        if(lastUserId==userid&&(tsec-lastTimeSec)<this->time_iterval&&lastLocId!=locid&&lastLocId!=0){
            addLLRelation(lastLocId,locid);
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
    cout<<"读取好友关系文件"<<friendFileName<<endl;
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

Item * Socialnet::getItemPtrById(int id,int itemType,bool isAllowNewType, float longitude, float latitude){
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

    Item * newItem=new Item(id, itemType);
    itemList->insert(ItemMap::value_type(id,newItem));
    if (longitude != -1 && latitude != -1 &&itemType == ITEMTYPE_LOCATION)
    {
        newItem->insertLongLati(longitude,latitude);
    }
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

void Socialnet::addCheckin(int userId,int locId, float longitude, float latitude,int windowTime){
    Item * user=getItemPtrById(userId,ITEMTYPE_USER,ALLOW_INSERT_IF_NULL);
    Item * loc=getItemPtrById(locId,ITEMTYPE_LOCATION,ALLOW_INSERT_IF_NULL,longitude,latitude);
    //网络中插入用户-位置边数据
    user->addToLocE(locId,weightCpuType,windowTime);
    loc->addToUserE(userId,weightCpuType,windowTime);
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
ItemMap * Socialnet::getLoclist(){
    return & this->locList;
}
//TODO 
void Socialnet::GetNearestLocListMap(ItemMap & locList,int locId){
    Item * loc = getItemPtrById(locId, ITEMTYPE_LOCATION);
    if (loc == NULL)
    {
        locList.clear();
        return;
    }
    int locCount=0;
    int mostFarDistance=0;
    locList.clear();
    return;

    for(ItemMap::const_iterator lociter=locList.begin();lociter!=locList.end();++lociter){
        Item * toLoc = lociter->second;
        float dis=Socialnet::GetDistance(loc->getLongitude(), loc->getLatitude(), toLoc->getLongitude(), toLoc->getLatitude());
    }
}