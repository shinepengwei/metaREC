#include "FriBasedRecommend.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>

FriBasedRecommend::FriBasedRecommend(Socialnet* socNet):BasedRecommend(socNet)
{
}

void FriBasedRecommend::Recommend(string checkinDataFileName){
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
    int rightCase[3]={0};//应该被推荐的数量

    int rightRecOfUser[3] = {0};
    int rightCaseOfUser=0;
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
            
            
            /*
            if (rightCaseOfUser>=TOPN1)
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;
            }
            if (rightCaseOfUser>=TOPN2)
            {
                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;
            }
            */
            if (rightCaseOfUser>=TOPN3)
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;

                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;

                rightRec[2]+=rightRecOfUser[2];
                rightCase[2] += rightCaseOfUser;
                allRec[2]+=TOPN3;
            }

            if(allRec[0]!=0){
                if(ISDEBUG){
                    cout<<"top-5:"<<endl;
                    printMap(sortedRec1);
                }

                cout<<" FriendBased精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
                if(ISDEBUG){
                    cout<<"top-10:"<<endl;
                    printMap(sortedRec2);
                }
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
                if(ISDEBUG){
                    cout<<"top-20:"<<endl;
                    printMap(sortedRec3);
                }
                cout<<" 精确率rightRec[2]/allRec[2]："<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase[2]<<" :"<<(float)rightRec[2]/rightCase[2]<<endl;
            }
            //计算这个新的user的推荐信息
            //首先清除一些东西
            f2fWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
            for (int i=0;i<3;i++)
            {
                rightRecOfUser[i]=0;
            }
            rightCaseOfUser=0;


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
            if (recResult.size()>20 &&(sortedRec1.size() != TOPN1||sortedRec2.size() != TOPN2||sortedRec3.size() != TOPN3))
            {
                cout<<"sortedRec.size() != TOPN"<<endl;
                int xx;
                cin>>xx;
            }
            //cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<userid<<endl;
            lastUserId=userid;
        }
        //计算精确率和召回率
        //第一步统计推荐正确的数量
        if(sortedRec1.find(locid)!=sortedRec1.end()){

            if(sortedRec1[locid]!=-1){
                rightRecOfUser[0]++;
                sortedRec1[locid]=-1;
            }
        }
        if(sortedRec2.find(locid)!=sortedRec2.end()){
            if(sortedRec2[locid]!=-1){
                rightRecOfUser[1]++;
                sortedRec2[locid]=-1;
            }
        }
        if(sortedRec3.find(locid)!=sortedRec3.end()){
            if(sortedRec3[locid]!=-1){
                rightRecOfUser[2]++;
                sortedRec3[locid]=-1;
            }
        }

        if(locIdSet.find(locid)==locIdSet.end()){
            rightCaseOfUser++;
            locIdSet.insert(locid);
        }
    }
    //计算精确率和召回率
    cout<<"FriendBased top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"召回率"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase[2]<<endl;
    cout<<"top-20:"<<endl;
    cout<<" 精确率："<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"召回率"<<(float)rightRec[2]/rightCase[3]<<endl;
}


FriBasedRecommend::~FriBasedRecommend(void)
{
}
