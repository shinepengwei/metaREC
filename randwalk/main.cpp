#include"Edge.h"
#include"item.h"
#include<cstdio>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include <vector>
#include <map>
#include <set>
#include<ctime>
#include<iterator>
#include<cmath>
#include<math.h>

#define TIME_ITERVAL 24*60*60
#define OUTPUT_NEG_EQUAL 1//1显示和正例相同的负例-train集，0显示所有的负例-test集
#define MOD_FILE_WRITE 1//1表示结果写文件，0表示结果忽略，只为了统计数据。


#define TOPN1 5
#define TOPN2 10
#define TOPN3 20
static  UserMap userlist;
static LocMap locationlist;

using std::cout;

int outputNegMod1;
int outputFileWriteMod1;

string checkinDataFileName;//签到数据，用于构建社交网络并计算元路径特征
string friendDataFileName;//好友关系数据，用于构建社交网络并计算元路径特征
string testCheckinDataFileName;//签到数据，用于比较并定义正例和负例。
string featureDataFileName;//输出正例负例数据，数据格式为：用户ID，位置ID，特征值。。。，正负例label

int hasLLedge=1;//是否考虑L-L边
int weightCPUtype=0;//位置-位置权值的计算方法



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
void printMap(const map<int,float> &mapt ){
    for(map<int,float>::const_iterator iter=mapt.begin();iter!=mapt.end();++iter){
        cout<<iter->first<<"     "<<iter->second<<endl;
    }
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
            if(route[0]=='U'&&route[1]=='U'){
                weight=iter1->second->getCount();
            }else{
                weight=iter1->second->getWeight();
            }
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
                if(route[1]=='U'&&route[2]=='U'){
                    weight=iter2->second->getCount();
                }else{
                    weight=iter2->second->getWeight();
                }
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
                         if(route[2]=='U'&&route[3]=='U'){
                             weight=iter3->second->getCount();
                          }else{
                            weight=iter3->second->getWeight();
                          }
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
void readCheckinData(int testCount=-1){
    //读入签到数据
    ifstream checkinfile(checkinDataFileName);
    char buffer[100];
    int i=0;
    int lastUserId=0,lastLocId=0,lastTimeSec=0;//用于判断比较获得位置-位置边
    //根据用户的签到信息，读用户、位置节点信息以及用户-位置和位置-位置的边信息
    cout<<"读取签到文件"<<endl;
    int xxbug=0;
    while(xxbug<10000){
      //  xxbug++;
        i++;
        if(i%100000==0) cout<<i<<",";
        if(checkinfile.eof()){
            break;
        }
        if(testCount!=-1&&i>testCount){
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
        if(lastUserId==userid&&(lastTimeSec-tsec)<TIME_ITERVAL&&lastLocId!=locid&&lastLocId!=0){
            loc->addToLocV(lastLocId);
        }
        lastUserId=userid;
        lastTimeSec=tsec;
        lastLocId=locid;

        //cout<<"after dealing________________________________________________"<<endl;
        //user->print("user");
        //loc->print("location");
       // cout<<endl<<endl<<endl;
    }/*
    if(LTOLWEIGHTCPU){
        //使用新的边权值计算方法
        for(LocMap::const_iterator iter=locationlist.begin();iter!=locationlist.end();++iter){
            Location * loc=
        }

    }*/
    cout<<endl;
   // cout<<"\ncount:"<<userlist.size()<<","<<locationlist.size()<<endl;
    //cout<<endl<<endl<<endl;cout<<endl<<endl<<endl;
}
void readFriendData(int testCount=-1){
    //读用户关系数据
    ifstream friendfile(friendDataFileName);
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
        if(testCount!=-1&&i>testCount){
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
    cout<<endl;
}
//找到数量为count的负例,若count=-1，则将所有的负例都返回
int addNegativeCase(int fromid,FeatureMap * metaFea,ofstream &outFile,int count,int outputNegMod,int outputFileWriteMod,int featureCount){
    if(count==0) return 0;
    if(outputNegMod!=OUTPUT_NEG_EQUAL){
        count=-1;
    }
    int negcount=0;//选择的负例数量
    int trueNegCount=0;//原始的负例数量
    bool flag=true;//是否还要继续添加负例
    //ofstream outFile=outFilePtr;
    for(int i=0;i<featureCount;i++){
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
            if(i!=featureCount-1){
                for(int j=i+1;j<featureCount;j++){
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
                    for(int x=0;x<featureCount;x++){
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
//读取下一阶段的数据，计算元路径特征值，并且生成正负例并将其保存在文件中。
//outputNegMod: 1显示和正例相同的负例-train集，0显示所有的负例-test集
//outputFileWriteMod:1表示结果写文件，0表示结果忽略，只为了统计数据
void readTestData(int outputNegMod,int outputFileWriteMod){
    int featureCount=5;
    if(!hasLLedge){
        featureCount=3;
    }
    ifstream testfile(testCheckinDataFileName);
    ofstream outfile(featureDataFileName,ios::out);
    int userCount=0;
    char buffer[100];
    int lastuserid=-1;
    //针对每一个源节点，通过元路径到达一个目的节点，源节点和目的节点直接可以计算元路径特征值，
    //第i个元路径特征的计算的结果放在metaFeature[i]中，形式为<目的节点id，特征值>
    FeatureMap metaFeature[5];
    char route0[4]="UUL";
    char route1[5]="UUUL";
    char route2[5]="ULUL";
    char route3[4]="ULL";
    char route4[5]="ULLL";

    int allCount=0;//所有的签到记录数量
    int metaPathInCount[5];//属于元路径到达的签到数量
    for(int i=0;i<featureCount;i++) metaPathInCount[i]=0;
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
    int xbug=0;
    while(xbug<100){
        //xbug++;
        //读取文件中的新的一行，并进行处理获得用户id和位置id
        i++;
        //if(i==1000) break;//just for test
        if(i%10000==0) cout<<i<<",";
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


        //如果发现用户id是一个新的用户，先对旧的用户进行处理（添加负例），然后在计算新的用户的元路径能到达的位置
        if(userid!=lastuserid){
            userCount++;
            uLocSet.clear();//清空用户访问的位置集合
            //把上一个User中的剩余签到作为负例
            if(lastuserid!=-1){
                negativeCount+=lastPosCount;//添加和正例数量相同的负例
                if(lastPosCount>=20||outputNegMod==OUTPUT_NEG_EQUAL){
                    //如果正例数量大于20，或者输出与正例相同的负例，才进行分析
                    trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount,outputNegMod, outputFileWriteMod,featureCount);
                }
            }

            //出现一个新的userid，找到这个用户通过元路径到达的所有位置，并计算出用户-位置的元路径特征值
            if(hasLLedge){
                metaFeature[0]=::calFeature(userid,route0,2);
                metaFeature[1]=::calFeature(userid,route1,3);
                metaFeature[2]=::calFeature(userid,route2,3);
                metaFeature[3]=::calFeature(userid,route3,2);
                metaFeature[4]=::calFeature(userid,route4,3);
            }else{
                metaFeature[0]=::calFeature(userid,route0,2);
                metaFeature[1]=::calFeature(userid,route1,3);
                metaFeature[2]=::calFeature(userid,route2,3);
            }
            lastuserid=userid;
            lastPosCount=0;
        }
        ostream_iterator<int> output(std::cout,"");
        //std::os
        //cout<<"uLocSet:";
       
        //std::copy(uLocSet.begin(),uLocSet.end(),output);
        //cout<<endl;
        EdgeMap *emp=u->getToLocE();
        if(emp->find(locid)!=emp->end()){//如果用户train阶段已经访问过该位置
            reCount++;
            continue;
        }
        set<int>::const_iterator resIter=uLocSet.find(locid);
        if(resIter!=uLocSet.end()){//用户在test阶段已经访问过
        //当出现用户在test阶段第二次访问某一位置时，不再生成正例。
            testReCount++;
            continue;
        }
        int flag=0;
        FeatureMap::iterator miter;
        float tmpFeature[5];
        for(int i=0;i<featureCount;i++){//从五个特征map里面分别找lid
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
            uLocSet.insert(locid);
            if(outputFileWriteMod==MOD_FILE_WRITE){
                outfile<<userid<<","<<locid;
                for(int i=0;i<featureCount;i++)
                    outfile<<","<<tmpFeature[i];
                outfile<<",1"<<endl;
            }
            positiveCount++;
            lastPosCount++;
        }
    }
    cout<<endl;
    //将最后的一个Userid的负例插入
    negativeCount+=lastPosCount;
    if(lastPosCount>=20||outputNegMod==OUTPUT_NEG_EQUAL)
        trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount,outputNegMod, outputFileWriteMod,featureCount);

    //输出各种参数
    cout<<"测试数据集中出现的所有的用户数量userCount："<<userCount<<endl;
    cout<<"所有的签到记录数量AllCount:"<<allCount<<endl;
    for(int i=0;i<featureCount;i++)
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
void calculateWik(map<int,float> & wikMap,int userid){
    User * u=::getUserPtrbyId(userid,NOALLOWNEW);
    //计算每一个用户和其他用户的相似度Wi,k
    //临时计算变量，第一个int表示k，第二个int表示对应的值
    map<int,int> cijckjMap;
    int cij2=0;//对于当前的用户，有固定的cij2
    //Wik=cijckj/（sqrt（cij2）*sqrt（ckj2））
    EdgeMap *locEdgeList=u->getToLocE();
    //对于用户u来说，找到用户的所有访问过的位置，然后找到访问过这些位置的用户
    for(EdgeMap::const_iterator liter=locEdgeList->begin();liter!=locEdgeList->end();++liter){
        int lid=liter->first;
        int cij=liter->second->getWeight();
        cij2+=cij*cij;
        Location * loc=::getLocationPtrbyId(lid,NOALLOWNEW);
        EdgeMap * userEdgeList=loc->getToUserE();
        //计算cijckj的一个分量
        for(EdgeMap::const_iterator uiter=userEdgeList->begin();uiter!=userEdgeList->end();++uiter){
            int uid=uiter->first;
            if(uid==userid) continue;
            int ckj=uiter->second->getWeight();
            int cijckj=cij*ckj;
            if(wikMap.find(uid)==wikMap.end()){
                wikMap.insert(map<int,float>::value_type(uid,0));
                cijckjMap.insert(map<int,int>::value_type(uid,cijckj));
            }else{
                cijckjMap[uid]+=cijckj;
            }
        }
    }
    //计算用户k的ckj2，
    for(map<int,float>::const_iterator witer=wikMap.begin();witer!=wikMap.end();++witer){
        int toUId=witer->first;
        int ckj2=0;
        User *toUser=::getUserPtrbyId(toUId,NOALLOWNEW);
        EdgeMap * locList=toUser->getToLocE();
        for(EdgeMap::const_iterator edgIter=locList->begin();edgIter!=locList->end();++edgIter){
            ckj2+=edgIter->second->getWeight()*edgIter->second->getWeight();
        }
        wikMap[toUId]=cijckjMap[toUId]/(sqrt((float)cij2)*sqrt((float)ckj2));
    }
}

void calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid){
    int ss=0;
    User * user=::getUserPtrbyId(uid,NOALLOWNEW);
    int count=0;
    /*
    for(map<int,float>::const_iterator witer=u2uWeightMap.begin();witer!=u2uWeightMap.end();witer++){
    //统计为该用户要计算的位置的数量
        int toUserId=witer->first;
        if(toUserId==uid) continue;
        User * toUser=::getUserPtrbyId(toUserId,NOALLOWNEW);
        EdgeMap *locedgelist=toUser->getToLocE();
        count+=locedgelist->size();
       
    }*/
    //cout<<u2uWeightMap.size()<<"allsize:"<<count<<endl;
    //正式开始计算！：
    for(map<int,float>::const_iterator witer=u2uWeightMap.begin();witer!=u2uWeightMap.end();witer++){
        //计算每一个和用户i具有相似值的用户访问过的位置L1，将这些位置推荐给用户i
        //计算用户uid和相似用户的访问位置集合~
        int toUserId=witer->first;
        if(toUserId==uid) continue;
        User * toUser=::getUserPtrbyId(toUserId,NOALLOWNEW);
        EdgeMap *locedgelist=toUser->getToLocE();
        for(EdgeMap::const_iterator editer=locedgelist->begin();editer!=locedgelist->end();++editer){
        //计算用户i对位置toLId的隐含评价
            ss++;
            int toLId=editer->first;
            if(user->getToLocE()->find(toLId)!=user->getToLocE()->end()) continue;//如果用户i曾经访问过该位置，不推荐该位置
            if(recResult.find(toLId)!=recResult.end()) {
               // cout<<"该Location已经计算过了"<<endl;
                continue;//该Location已经计算过了
            }
            //计算隐含评价
            //Location * toLoc=::getLocationPtrbyId(toLId,NOALLOWNEW);
            float wikckjA=0;
            float wikA=0;

            for(map<int,float>::const_iterator u2uWiter=u2uWeightMap.begin();u2uWiter!=u2uWeightMap.end();u2uWiter++){
            //找到所有和用户i相似的用户，然后使用公式计算
                int tmpUid=u2uWiter->first;
                User* tmpUser=::getUserPtrbyId(tmpUid,NOALLOWNEW);
                if(tmpUser->getToLocE()->find(toLId)!=tmpUser->getToLocE()->end()){
                    //如果用户tmpUid访问过该位置toLId，wikckj加进来，否则，wikckj元素为0。
                    wikckjA+=u2uWiter->second*(tmpUser->getToLocE()->find(toLId)->second->getWeight());
                }
                wikA+=u2uWiter->second;
            }
            if(wikA!=0)
                recResult.insert(map<int,float>::value_type(toLId,wikckjA/wikA));
            else
                cout<<"err:wik-ALL is 0"<<endl;
            
            /*if(ss%20000==0){
                cout<<"recResultSize:"<<recResult.size();
            }*/
        }
    }
}
void sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N=20){
    int leastUserId=-1;
    float leastRanked=INT_MAX;
    int sortedCount=0;
    for(map<int,float>::const_iterator recIter=recResult.begin();recIter!=recResult.end();++recIter){
        if(sortedCount<N){
            sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
            sortedCount++;
            if(sortedCount==N){
                //K个满了，就存储最小的元素
                for(map<int,float>::const_iterator sorIter=sortedRec.begin();sorIter!=sortedRec.end();++sorIter){
                    if(sorIter->second<leastRanked){
                        leastRanked=sorIter->second;
                        leastUserId=sorIter->first;
                    }
                }
            }
        }else{
           //20个满了，比较当前元素和已排序的最小元素，如果更大，则删掉最小元素，将当前元素插入，然后再找最小元素
            if(recIter->second>leastRanked){
                sortedRec.erase(leastUserId);
                sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
                //查找最小元素
                leastRanked=INT_MAX;
                for(map<int,float>::const_iterator sorIter=sortedRec.begin();sorIter!=sortedRec.end();++sorIter){
                    if(sorIter->second<leastRanked){
                        leastRanked=sorIter->second;
                        leastUserId=sorIter->first;
                    }
                }
            }
        }
    }
}

//根据用户ID，计算该用户和他的朋友之间的朋友度（好友的亲密程度）
void calculateSIik(map<int,float>& f2fWeightMap,int userid){
    const float para=0.5;//自定义权值

    User * u=::getUserPtrbyId(userid,NOALLOWNEW);
    //计算每一个用户和他的好友的好友度SIk,i
    EdgeMap * frdsEdgeList=u->getToUserE();
    for(EdgeMap::const_iterator fliter=frdsEdgeList->begin();fliter!=frdsEdgeList->end();++fliter){
        int coFrdCount=0;
        int allFrdCount=0;
        int coVisitedCount=0;
        int allVisitedCount=0;
        int fid=fliter->first;
        User * frdUser=::getUserPtrbyId(fid);

        //统计俩用户共同的朋友和全部的朋友数量
        EdgeMap * frdsOfFrdEdgeList=frdUser->getToUserE();
        for(EdgeMap::const_iterator FOfFLiter=frdsOfFrdEdgeList->begin();FOfFLiter!=frdsOfFrdEdgeList->end();++FOfFLiter){
            if(frdsEdgeList->find(FOfFLiter->first)!=frdsEdgeList->end()){
                coFrdCount++;
            }
        }
        allFrdCount=frdsEdgeList->size()+frdsOfFrdEdgeList->size()-coFrdCount;

        //统计俩用户共同访问的位置和全部的位置数量
        EdgeMap * locEdgeList=u->getToLocE();//该用户访问过的位置
        EdgeMap * locOfFrdEdgeList=frdUser->getToLocE();
        for(EdgeMap::const_iterator LOfFLiter=locOfFrdEdgeList->begin();LOfFLiter!=locOfFrdEdgeList->end();++LOfFLiter){
            if(locEdgeList->find(LOfFLiter->first)!=locEdgeList->end()){
                coVisitedCount++;
            }
        }
        allVisitedCount=locEdgeList->size()+locOfFrdEdgeList->size()-coVisitedCount;

        //将计算结果存入map中
        float frty=para*coFrdCount/allFrdCount+(1-para)*coVisitedCount/allVisitedCount;
        f2fWeightMap.insert(map<int,float>::value_type(fid,frty));
    }
}
void FriendBasedRecommend(){
    /*
   int index=0;
    cout<<"请输入1-12的数字选择对应的数据文件"<<endl;
    cin>>index;
    string fileName;
    stringstream ss;
    ss<<CHECKTESTDATA<<index<<".txt";
    ss>>fileName;
    cout<<fileName<<endl;
    ifstream testfile(fileName);*/
    ifstream testfile(checkinDataFileName);



    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> f2fWeightMap;//用户i和用户k的相似度：SIk,i，其中i为当前的userid，<int-k，float-f2fWeight：SIk,i>
    map<int,float> recResult;//针对用户i的推荐结果，其中第一个int表示位置j的id，float表示预测到的评价值
    map<int,float> sortedRec1;//Top-k个推荐结果TOPN1-5
    map<int,float> sortedRec2;//Top-k个推荐结果TOPN2-10
    map<int,float> sortedRec3;//Top-k个推荐结果TOPN3-20

    set<int> locIdSet;//测试集中当前用户已访问过的用户，用于消除测试集中用户多次访问某一位置
    int rightRec[3]={0};//推荐正确的数量
    int allRec[3]={0};//推荐的所有的元素数量
    int rightCase=0;//应该被推荐的数量
    while(true){
        i++;
        //读一行签到记录并解析
        if(testfile.eof()) break;
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);

        if(result.size()!=5){
            break;
        }

        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        User * user=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * loc=::getLocationPtrbyId(locid,NOALLOWNEW);
        //解析签到记录完毕

        if(user==NULL||loc==NULL){//如果用户或者位置不存在
            continue;
        }

        if(userid!=lastUserId){//针对于一个新的用户，为其推荐TOP-K的地点
            //先把上一个user的信息打印出来
            if(allRec[0]!=0){
                if(ISDEBUG){
                    cout<<"top-5:"<<endl;
                    printMap(sortedRec1);
                }
                cout<<" 精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                if(ISDEBUG){
                    cout<<"top-10:"<<endl;
                    printMap(sortedRec2);
                }
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                if(ISDEBUG){
                    cout<<"top-20:"<<endl;
                    printMap(sortedRec3);
                }
                cout<<" 精确率rightRec[2]/allRec[2]："<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //计算这个新的user的推荐信息
            //首先清除一些东西
            f2fWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
           //计算每一个用户和其他用户的相似度Wi,k,将结果存放在u2uWeightMap
            calculateSIik(f2fWeightMap,userid);
            if(ISDEBUG){
                cout<<"用户："<<userid<<endl;
                cout<<"用户-用户权值：（用户，权值）"<<userid<<endl;
                printMap(f2fWeightMap);
            }
            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            calRecResult(recResult,f2fWeightMap,userid);//！！？？！！！现在使用的是UCF的推荐函数，不确定可不可以用。
            if(ISDEBUG){
                cout<<"推荐结果：（位置ID，推荐度）"<<userid<<endl;
                printMap(recResult);
            }

            //选择top-k个推荐结果

            sortRecResult(sortedRec1,recResult,TOPN1);
            sortRecResult(sortedRec2,recResult,TOPN2);
            sortRecResult(sortedRec3,recResult,TOPN3);
            //cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<userid<<endl;
            allRec[0]+=TOPN1;
            allRec[1]+=TOPN2;
            allRec[2]+=TOPN3;
            lastUserId=userid;
        }
         //计算精确率和召回率
        //第一步统计推荐正确的数量
        if(sortedRec1.find(locid)!=sortedRec1.end()){

            if(sortedRec1[locid]!=-1){
                rightRec[0]++;
                sortedRec1[locid]=-1;
            }
        }
        if(sortedRec2.find(locid)!=sortedRec2.end()){
            if(sortedRec2[locid]!=-1){
                rightRec[1]++;
                sortedRec2[locid]=-1;
            }
        }
        if(sortedRec3.find(locid)!=sortedRec3.end()){
            if(sortedRec3[locid]!=-1){
                rightRec[2]++;
                sortedRec3[locid]=-1;
            }
        }

        if(locIdSet.find(locid)==locIdSet.end()){
            rightCase++;
            locIdSet.insert(locid);
        }
    }
    //计算精确率和召回率
    cout<<"top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;

    cout<<"召回率"<<(float)rightRec[0]/rightCase<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase<<endl;
    cout<<"top-20:"<<endl;
    cout<<" 精确率："<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"召回率"<<(float)rightRec[2]/rightCase<<endl;
}
void UCFRecommend(){
    int index=0;
    cout<<"请输入1-12的数字选择对应的数据文件"<<endl;
    cin>>index;
    string fileName;
    stringstream ss;
    ss<<checkinDataFileName<<index<<".txt";
    ss>>fileName;
    cout<<fileName<<endl;
    ifstream testfile(fileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> u2uWeightMap;//用户i和用户k的相似度：Wi,k，其中i为当前的userid，int-k，float-u2uWeight：Wi,k
    map<int,float> recResult;//针对用户i的推荐结果，其中第一个int表示item的id，float表示预测到的评价值
    map<int,float> sortedRec1;//Top-k个推荐结果TOPN1-5
    map<int,float> sortedRec2;//Top-k个推荐结果TOPN2-10
    map<int,float> sortedRec3;//Top-k个推荐结果TOPN3-20

    set<int> locIdSet;//测试集中当前用户已访问过的用户，用于消除测试集中用户多次访问某一位置
    int rightRec[3]={0};//推荐正确的数量
    int allRec[3]={0};//推荐的所有的元素数量
    int rightCase=0;//应该被推荐的数量
    while(true){
        i++;
        //读一行签到记录并解析
        if(testfile.eof()) break;
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        User * user=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * loc=::getLocationPtrbyId(locid,NOALLOWNEW);
        //解析签到记录完毕

        if(user==NULL||loc==NULL){//如果用户或者位置不存在
            continue;
        }
        if(userid!=lastUserId){//针对于一个新的用户，为其推荐TOP-K的地点
            if(allRec[0]!=0){
                cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<" 精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                cout<<"top-10:"<<endl;
                //printMap(sortedRec2);
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" 精确率rightRec[2]/allRec[2]："<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //首先清除一些东西
            u2uWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
           //计算每一个用户和其他用户的相似度Wi,k,将结果存放在u2uWeightMap
            calculateWik(u2uWeightMap,userid);
            cout<<fileName<<endl;
            cout<<"用户："<<userid<<endl;

            //cout<<"用户-用户权值：（用户，权值）"<<userid<<endl;
           // printMap(u2uWeightMap);
            
            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            calRecResult(recResult,u2uWeightMap,userid);
            //cout<<"推荐结果：（位置ID，推荐度）"<<userid<<endl;
            //printMap(recResult);

            //选择top-k个推荐结果
            sortRecResult(sortedRec1,recResult,TOPN1);
            sortRecResult(sortedRec2,recResult,TOPN2);
            sortRecResult(sortedRec3,recResult,TOPN3);
            //cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<userid<<endl;
            //printMap(sortedRec);
            allRec[0]+=TOPN1;
            allRec[1]+=TOPN2;
            allRec[2]+=TOPN3;
            lastUserId=userid;
        }
         //计算精确率和召回率
        //第一步统计推荐正确的数量
        if(sortedRec1.find(locid)!=sortedRec1.end()){
            if(sortedRec1[locid]!=-1){
                rightRec[0]++;
                sortedRec1[locid]=-1;
            }
        }
        if(sortedRec2.find(locid)!=sortedRec2.end()){
            if(sortedRec2[locid]!=-1){
                rightRec[1]++;
                sortedRec2[locid]=-1;
            }
        }
        if(sortedRec3.find(locid)!=sortedRec3.end()){
            if(sortedRec3[locid]!=-1){
                rightRec[2]++;
                sortedRec3[locid]=-1;
            }
        }

        if(locIdSet.find(locid)==locIdSet.end()){
            rightCase++;
            locIdSet.insert(locid);
        }
    }
    //计算精确率和召回率
    cout<<"top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"召回率"<<(float)rightRec[0]/rightCase<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase<<endl;
    cout<<"top-20:"<<endl;
    cout<<" 精确率："<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"召回率"<<(float)rightRec[2]/rightCase<<endl;
}
void main()
{
    int outputNegMod;
    cout<<"11-训练集特征文件输出（考虑L-L边，边的权值直接为次数）："<<endl;
    cout<<"12-测试集特征文件输出（考虑L-L边，边的权值直接为次数）："<<endl;
    cout<<"21-训练集特征文件输出（考虑L-L边，边的权值直接为log(次数)）："<<endl;
    cout<<"22-测试集特征文件输出（考虑L-L边，边的权值直接为log(次数)）："<<endl;
    cout<<"31-训练集特征文件输出（考虑L-L边，边的权值直接为0、1）："<<endl;
    cout<<"32-测试集特征文件输出（考虑L-L边，边的权值直接为0、1）："<<endl;
    cout<<"41-训练集特征文件输出（不考虑L-L边，边的权值直接为次数）："<<endl;
    cout<<"42-测试集特征文件输出（不考虑L-L边，边的权值直接为次数）："<<endl;
    int inparam;
    cin>>inparam;
    if(inparam==11){
        checkinDataFileName=TRAIN_CHECKINDATA;//签到数据，用于构建社交网络并计算元路径特征
        friendDataFileName=TRAIN_FRIENDDATA;//好友关系数据，用于构建社交网络并计算元路径特征
        testCheckinDataFileName=TRAIN_CHECKTESTDATA;//签到数据，用于比较并定义正例和负例。
        featureDataFileName=TRAIN_FEATUREDATA;//输出正例负例数据，数据格式为：用户ID，位置ID，特征值。。。，正负例label
        hasLLedge=true;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTRAIN;
    }else if(inparam==12){
        //12-测试集特征文件输出（考虑L-L边，边的权值直接为次数）：
        checkinDataFileName=TEST_CHECKINDATA;
        friendDataFileName=TEST_FRIENDDATA;
        testCheckinDataFileName=TEST_CHECKTESTDATA;
        featureDataFileName=TEST_FEATUREDATA;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTEST;
    }else if(inparam==41){
        //训练集特征文件输出（不考虑L-L边，边的权值直接为次数）
        checkinDataFileName=TRAIN_CHECKINDATA;
        friendDataFileName=TRAIN_FRIENDDATA;
        testCheckinDataFileName=TRAIN_CHECKTESTDATA;
        featureDataFileName=TRAIN_FEATUREDATANOLLEDGE;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTRAIN;
    }else if(inparam==42){
        //训练集特征文件输出（不考虑L-L边，边的权值直接为次数）
        checkinDataFileName=TEST_CHECKINDATA;
        friendDataFileName=TEST_FRIENDDATA;
        testCheckinDataFileName=TEST_CHECKTESTDATA;
        featureDataFileName=TEST_FEATUREDATANOLLEDGE;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTEST;
    }else{
        cout<<"ERROR INPUT!"<<endl;

    }
    cout<<"Begin:"<<endl;
    cout<<"CHECKINDATA"<<checkinDataFileName<<endl;
    cout<<"FRIENDDATA"<<friendDataFileName<<endl;
    cout<<"CHECKTESTDATA"<<testCheckinDataFileName<<endl;
    cout<<"FEATUREDATA"<<featureDataFileName<<endl;
    cout<<"hasLLedge"<<hasLLedge<<endl;
    readCheckinData();
    readFriendData();

    cout<<"请选择推荐方法："<<endl;
    cout<<"1-基于用户的协同过滤"<<endl;
    cout<<"2-基于好友的协同过滤"<<endl;
    cout<<"3-基于链路预测的位置推荐，输出结果为训练集或测试集"<<endl;

    int method=0;
    cin>>method;
    if(method==1){
        //基于用户的协同过滤的位置推荐
        UCFRecommend();
    }else if(method==2){
        //基于好友的位置推荐
        FriendBasedRecommend();
    }else if(method==3){
        //基于链路预测的位置推荐
        
        int outputFileWriteMod;
        cout<<"outputFileWriteMod:1表示结果写文件，0表示结果忽略，只为了统计数据。"<<endl;
        cin>>outputFileWriteMod;

        ::readTestData(outputNegMod,outputFileWriteMod);
        cout<<"数据集中的所有用户数量："<<::userlist.size()<<endl;
        cout<<"数据集中所有的位置数量："<<::locationlist.size()<<endl;
    }
    int xx;
    cin>>xx;
}
