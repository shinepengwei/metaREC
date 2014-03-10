#include "GeograRecommend.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>
#include <math.h>
#define ISDEBUG 0
//地理位置相关推荐中使用的系数，按说应该是通过线性拟合确定，先这样吧 - TODO
#define A_PARAM_G 0.1
#define B_PARAM_G -100
//综合特征权重系数
#define A_PARAM_USG 0.1
#define B_PARAM_USG 0.2
#define C_PARAM_USG 1-A_PARAM_USG-B_PARAM_USG


GeograRecommend::GeograRecommend(Socialnet* socNet):BasedRecommend(socNet)
{
}

void GeograRecommend::CalRecResultForGEO(map<int,float>&recResult,int uid){
    Item * user = socialNet->getItemPtrById(uid,ITEMTYPE_USER);
    EdgeMap * userLocList = user->getToLocE();

    ItemMap* locList = socialNet->getLoclist();
    for(ItemMap::const_iterator locIter=locList->begin();locIter!=locList->end();++locIter){

    }
}
void GeograRecommend::Recommend(string checkinDataFileName, bool isUSG){
    ifstream testfile(checkinDataFileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> f2fWeightMap;//用户i和用户k的相似度：SIk,i，其中i为当前的userid，<int-k，float-f2fWeight：SIk,i>
    map<int,float> u2uWeightMap;//用户i和用户k的相似度：Wi,k，其中i为当前的userid，int-k，float-u2uWeight：Wi,k
    
    map<int,float> recResultU;//基于用户的协同过滤针对用户i的推荐概率，其中第一个int表示位置j的id，float表示预测到的评价值
    map<int,float> recResultS;//基于好友关系针对用户i的推荐概率，
    map<int,float> recResultG;//基于地理位置针对用户i的推荐概率

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
       
        Item * user=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
        Item * loc = socialNet->getItemPtrById(locid,ITEMTYPE_LOCATION);

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
                cout<<" FriendBased精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
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
            recResultU.clear();
            recResultS.clear();
            recResultG.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();

            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            if (isUSG)
            {
                calculateSIik(f2fWeightMap,userid);
                calculateWik(u2uWeightMap, userid);
                calRecResult(recResultU,u2uWeightMap,userid);
                calRecResult(recResultS,f2fWeightMap,userid);
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
    cout<<"FriendBased top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"召回率"<<(float)rightRec[0]/rightCase<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase<<endl;
    cout<<"top-20:"<<endl;
    cout<<" 精确率："<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"召回率"<<(float)rightRec[2]/rightCase<<endl;
}

GeograRecommend::~GeograRecommend(void)
{
}
