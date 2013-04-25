#include"Edge.h"
#include"item.h"
#include<stdio.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include <vector>
#include <map>
#include <set>
#include<time.h>
#include<math.h>

#define TIME_ITERVAL 24*60*60
#define OUTPUT_NEG_EQUAL 1//1显示和正例相同的负例-train集，0显示所有的负例-test集
#define MOD_FILE_WRITE 1//1表示结果写文件，0表示结果忽略，只为了统计数据。
static  UserMap userlist;
static LocMap locationlist;
using std::cout;
int outputNegMod;
int outputFileWriteMod;

User* getUserPtrbyId(int id,int allownewtype){
    UserMap::iterator iter;
    iter =userlist.find(id);
    if(iter!=userlist.end()){
        return iter->second;
    }
    if(allownewtype==NOALLOWNEW) return NULL;
    User *newuser=new User(id);
    userlist.insert(UserMap::value_type(id,newuser));
    return newuser;
}
Location* getLocationPtrbyId(int id,int allownewtype)
{
    LocMap::iterator iter;
    iter =locationlist.find(id);
    if(iter!=locationlist.end()){
        return iter->second;
    }
    if(allownewtype==NOALLOWNEW) return NULL;
    Location *newloc=new Location(id);
    locationlist.insert(LocMap::value_type(id,newloc));
    return newloc;
}
Item * getItemPtrbyId(int id,int type,int allownewtype){
    if(type==TYPEUSER){
        return ::getUserPtrbyId(id,allownewtype);
    }else{
        return ::getLocationPtrbyId(id,allownewtype);
    }
}
Item * getItemPtrbyId(int id,char type,int allownewtype){
    if(type=='U'){
        return ::getUserPtrbyId(id,allownewtype);
    }else if(type='L'){
        return ::getLocationPtrbyId(id,allownewtype);
    }else{
        return NULL;
    }
}
bool isNeighbor(int fromid,int fromtype,int toid,int totype){
    Item * item;
    if(fromtype==TYPEUSER){
        item=::getUserPtrbyId(fromid,NOALLOWNEW);
    }else{
        item=::getLocationPtrbyId(fromid,NOALLOWNEW);
    }
    EdgeMap* emp=item->getToItemE(totype);
    EdgeMap::iterator iter=emp->find(toid);
    if(iter!=emp->end()){
        return true;
    }else{
       return false;
    }
}
bool isNeighbor(int fromid,char fromtype,int toid,char totype){
    int frominttype=TYPEUSER;
    if(fromtype=='L')
        frominttype=TYPELOCATION;
    int tointtype=TYPEUSER;
    if(totype=='L')
        tointtype=TYPELOCATION;
    return isNeighbor(fromid,frominttype,toid,tointtype);
}
void addFriend(int userFId,int userSId){
    User *fuser=::getUserPtrbyId(userFId,NOALLOWNEW);
    User *suser=::getUserPtrbyId(userSId,NOALLOWNEW);
    //cout<<"before add friends-------------------------"<<endl;
    if(fuser!=NULL&&suser!=NULL){
        //fuser->print("User");
        //suser->print("User");
        fuser->addToUserV(userSId);
        suser->addToUserV(userFId);
       // fuser->print("User");
        //suser->print("User");
    }
    else
    {
       // cout<<"NO USER"<<endl;
    }
    
}
vector<std::string> split(string str)
{
    char tmp[2];
    tmp[0]=9;
    tmp[1]='\0';
    string pattern(tmp);
     string::size_type pos;
     vector<string> result;
     str+=pattern;//扩展字符串以方便操作
     int size=str.size();
 
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             string s=str.substr(i,pos-i);
             result.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return result;
 }
//计算基于route元路径从id为fromId的ITEM到另一个item的特征值MAP
FeatureMap calFeature(int fromId,const char *route,int length){
    FeatureMap result;
    Item *item1=::getItemPtrbyId(fromId,route[0],NOALLOWNEW);
    if(item1==NULL){
        cout<<"error:NO this Item!"<<endl;
        return result;
    }
    EdgeMap *edgelist1=item1->getToItemE(route[1]);
    int allweight1=item1->getAllWeight(route[1]);
    for(EdgeMap::const_iterator iter1=edgelist1->begin();iter1!=edgelist1->end();++iter1){
        int id2=iter1->first;
        float weight=1;
        if(iter1->second!=NULL){
            weight=iter1->second->getWeight();
        }
        float feat1=weight/allweight1;
        //cout<<feat1;
        Item * item2=::getItemPtrbyId(id2,route[1],NOALLOWNEW);
        EdgeMap *edgelist2=item2->getToItemE(route[2]);
        int allweight2=item2->getAllWeight(route[2]);
        for(EdgeMap::const_iterator iter2=edgelist2->begin();iter2!=edgelist2->end();++iter2){
            int id3=iter2->first;
            float weight=1;
            if(iter2->second!=NULL){
                weight=iter2->second->getWeight();
            }
            float feat2=weight/allweight2;
            if(length==2){
                if(!::isNeighbor(fromId,route[0],id3,route[2])){
                    //这个节点和原节点没有连接，计算他们之间的元路径特征值
                    float feature=feat1*feat2;
                    FeatureMap::iterator fiter=result.find(id3);
                    if(fiter==result.end()){
                            result.insert(FeatureMap::value_type(id3,feature));
                    }
                    else{
                        fiter->second=feature+fiter->second;
                    }
                }
            }else{
                Item * item3=::getItemPtrbyId(id3,route[2],NOALLOWNEW);
                EdgeMap *edgelist3=item3->getToItemE(route[3]);
                int allweight3=item3->getAllWeight(route[3]);
                for(EdgeMap::const_iterator iter3=edgelist3->begin();iter3!=edgelist3->end();++iter3){
                    int id4=iter3->first;
                    float weight=1;
                    if(iter3->second!=NULL){
                        weight=iter3->second->getWeight();
                    }
                    float feat3=weight/allweight3;
                    if(!::isNeighbor(fromId,route[0],id4,route[3])){
                        float feature=feat1*feat2*feat3;
                        FeatureMap::iterator fiter=result.find(id4);
                        if(fiter==result.end()){
                            result.insert(FeatureMap::value_type(id4,feature));
                        }
                        else{
                            fiter->second=feature+fiter->second;
                        }
                    }
                }
            }
         }
    }
    return result;
}
/*
float getMetaPathFeatures(char *route,int fromid,vector<int> toid,int length){
    FeatureMap m=::calFeature(fromid,route,length);
    for(int i=0;i<toid.size();i++){
        FeatureMap::iterator iter=m.find(toid[i]);

    }
    
    if(iter!=m.end()){
        return iter->second;
    }
    return -1;
}*/
void readTrainCheckinData(){
    ifstream checkinfile(CHECKINDATA);
    char buffer[100];
    int i=0;
    int lastUserId=0,lastLocId=0,lastTimeSec=0;//用于判断比较获得位置-位置边
    //根据用户的签到信息，读用户、位置节点信息以及用户-位置和位置-位置的边信息
    cout<<"读取签到文件"<<endl;
    while(true){
        i++;
        if(i%10000==0) cout<<i<<endl;
        if(checkinfile.eof()){
            break;
        }
        checkinfile.getline(buffer,100);
        string str(buffer);
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

        //cout<<"some ID________________________________________________"<<endl;
       // cout<<"userid:"<<userid<<endl;
        //cout<<"LocId:"<<locid<<endl;
        //cout<<"Time:"<<result[1]<<"time second:"<<tsec<<endl;

        //插入或者找到用户和位置节点。
        User * user=getUserPtrbyId(userid);
        Location * loc=::getLocationPtrbyId(locid);
        //cout<<"before dealing________________________________________________"<<endl;
        //user->print("user");
        //loc->print("location");

        //网络中插入用户-位置边数据
        user->addToLocV(locid,tsec);
        loc->addToUserV(userid);
        //插入位置-位置边数据
        if(lastUserId==userid&&(lastTimeSec-tsec)<24*60*60&&lastLocId!=locid&&lastLocId!=0){
            loc->addToLocV(lastLocId);
        }
        lastUserId=userid;
        lastTimeSec=tsec;
        lastLocId=locid;

        //cout<<"after dealing________________________________________________"<<endl;
        //user->print("user");
        //loc->print("location");
       // cout<<endl<<endl<<endl;
    }
   // cout<<"\ncount:"<<userlist.size()<<","<<locationlist.size()<<endl;
    //cout<<endl<<endl<<endl;cout<<endl<<endl<<endl;
}
void readTrainFriendData(){
    ifstream friendfile(FRIENDDATA);
    char buffer[100];
    //读用户关系数据
    int i=0;
    cout<<"读取好友关系文件"<<endl;
    while(true){
        i++;
        if(i%10000==0) cout<<i<<endl;
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
    }
}
//找到数量为count的负例,若count=-1，则将所有的负例都返回
int addNegativeCase(int fromid,FeatureMap * metaFea,ofstream &outFile,int count){
    if(count==0) return 0;
    if(outputNegMod!=OUTPUT_NEG_EQUAL){
        count=-1;
    }
    int negcount=0;//选择的负例数量
    int trueNegCount=0;//原始的负例数量
    bool flag=true;//是否还要继续添加负例
    //ofstream outFile=outFilePtr;
    for(int i=0;i<5;i++){
        float feat[5];
        if(i>0){
            for(int j=0;j<i;j++){
                feat[j]=0;
            }
        }
        FeatureMap *meF=&metaFea[i];
        for(FeatureMap::iterator iter=meF->begin();iter!=meF->end();++iter){
            int toid=iter->first;
            feat[i]=iter->second;
            if(i!=4){
                for(int j=i+1;j<5;j++){
                    FeatureMap* meF2=&metaFea[j];
                    FeatureMap::iterator iter2=meF2->find(toid);
                    if(iter2!=meF2->end()){
                        feat[j]=iter2->second;
                        meF2->erase(iter2);
                    }else{
                        feat[j]=0;
                    }
                }
            }
            trueNegCount++;
            if(count==-1||(flag&&(rand()%100)==1)){
                if(outputFileWriteMod==MOD_FILE_WRITE){
                    outFile<<fromid<<","<<toid<<",";
                    for(int x=0;x<5;x++){
                        outFile<<feat[x]<<",";
                    }
                    outFile<<"0"<<endl;
                }
                negcount++;
                if(negcount==count){
                    flag=false;
                }

            }
        }
    }
    return trueNegCount;
}

void readTestData(){
    ifstream testfile(CHECKTESTDATA);
    ofstream outfile(TRAINDATA,ios::out);
    int userCount;
    char buffer[100];
    int lastuserid=-1;
    FeatureMap metaFeature[5];
    char route0[4]="ULL";
    char route1[4]="UUL";
    char route2[5]="ULLL";
    char route3[5]="UUUL";
    char route4[5]="ULUL";

    int allCount=0;//所有的签到记录数量
    int metaPathInCount[5];//属于元路径到达的签到数量
    for(int i=0;i<5;i++) metaPathInCount[i]=0;
    int reCount=0;//和train阶段重复的签到数量
    int testReCount=0;//在test阶段已经生成了正例的签到数量
    int noCount=0;//不属于元路径可到到的位置的签到数量
    int nullCount=0;//用户或者位置节点不属于原始数据集的签到数量

    int positiveCount=0;//正例数量
    int negativeCount=0;//放入数据集负例数量
    int trueNegCount=0;//真实的全部负例数量

    int i=0;
    std::cout<<"读取test文件"<<endl;
    int lastPosCount=0;//上一个userid的正例

    //用户到达过的位置集合:
    set<int> uLocSet;
    while(true){
        i++;
        //if(i==1000) break;//just for test
        if(i%10000==0) cout<<i<<endl;
        if(testfile.eof()){
            break;
        }
        allCount++;
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        User * u=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * l=::getLocationPtrbyId(locid,NOALLOWNEW);

        if(u==NULL||l==NULL){//如果用户或者位置不存在
            nullCount++;
            continue;
        }
        if(userid!=lastuserid){
            userCount++;
            uLocSet.clear();//清空用户访问的位置集合
            //把上一个User中的剩余签到作为负例
            if(lastuserid!=-1){
                negativeCount+=lastPosCount;//添加和正例数量相同的负例
                if(lastPosCount>=20){//如果正例数量大于20，才进行分析
                    trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);
                }
            }
            //出现一个新的userid，找到这个用户通过元路径到达的所有位置，并计算出用户-位置的元路径特征值
            metaFeature[0]=::calFeature(userid,route0,2);
            metaFeature[1]=::calFeature(userid,route1,2);
            metaFeature[2]=::calFeature(userid,route2,3);
            metaFeature[3]=::calFeature(userid,route3,3);
            metaFeature[4]=::calFeature(userid,route4,3);

            lastuserid=userid;
            lastPosCount=0;
        }
        EdgeMap *emp=u->getToLocE();
        if(emp->find(locid)!=emp->end()){//如果用户train阶段已经访问过该位置
            reCount++;
            continue;
        }
        set<int>::const_iterator resIter=uLocSet.find(locid);
        if(resIter!=uLocSet.end()){//用户在test阶段已经访问过
        //当出现用户在test阶段两次访问某一位置时，不生成正例。
            testReCount++;
            continue;
        }
        int flag=0;
        FeatureMap::iterator miter;
        float tmpFeature[5];
        for(int i=0;i<5;i++){//从五个特征map里面分别找lid
            miter=metaFeature[i].find(locid);
            if(miter!=metaFeature[i].end()){//找到一个，说明通过此元路径可以到达该位置，则次元路径可以作为用户-位置对的一个特征
                tmpFeature[i]=miter->second;
                metaPathInCount[i]++;
                flag=1;
                metaFeature[i].erase(miter);//并将此位置从特征MAP里面删掉，以防止负例选择时选择到他。
            }else{//此元路径无法从用户到达该位置
                 tmpFeature[i]=0;
            }
        }
        if(flag==0) noCount++;//不属于任何元路径可以访问得到的位置
        else{//添加一个正例
            if(outputFileWriteMod==MOD_FILE_WRITE){
                outfile<<userid<<","<<locid;
                for(int i=0;i<5;i++)
                    outfile<<","<<tmpFeature[i];
                outfile<<",1"<<endl;
            }
            positiveCount++;
            lastPosCount++;
        }
    }
    //将最后的一个Userid的负例插入
    negativeCount+=lastPosCount;
    if(lastPosCount>=20)
        trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);

    //输出各种参数
    cout<<"测试数据集中出现的所有的用户数量userCount："<<userCount<<endl;
    cout<<"所有的签到记录数量AllCount:"<<allCount<<endl;
    for(int i=0;i<5;i++)
        cout<<"属于元路径到达的签到数量metaPathInCount["<<i<<"]:"<<metaPathInCount[i]<<endl;

    cout<<"重复的签到数量ReCount:"<<reCount<<endl;
    cout<<"在test阶段重复的签到数量TestReCount:"<<testReCount<<endl;
    cout<<"不属于元路径可到到的位置的签到数量noCount:"<<noCount<<endl;
    cout<<"用户或者位置节点不属于原始数据集的签到数量nullCount"<<nullCount<<endl;
    cout<<"正例数量positiveCount："<<positiveCount<<endl;
    cout<<"选择的输出负例数量negativeCount："<<negativeCount<<endl;
    cout<<"负例数量trueNeCount："<<trueNegCount<<endl;
    outfile.close();
}
void main()
{
     /*
    ifstream checkinfile("d:\\testdata.txt");
    ifstream friendfile("d:\\testfriend.txt");
    ifstream testfile("d:\\testtestdata.txt");*/
    cout<<"outputNegMod: 1显示和正例相同的负例-train集，0显示所有的负例-test集"<<endl;
    cin>>outputNegMod;
    cout<<"outputFileWriteMod:1表示结果写文件，0表示结果忽略，只为了统计数据。"<<endl;
    cin>>outputFileWriteMod;

    readTrainCheckinData();
    readTrainFriendData();
    ::readTestData();
    cout<<"数据集中的所有用户数量："<<::userlist.size()<<endl;
    cout<<"数据集中所有的位置数量："<<::locationlist.size()<<endl;
    int xx;
    cin>>xx;
}