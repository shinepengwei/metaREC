#include "FriendRecAAN.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>

FriendRecAAN::FriendRecAAN(Socialnet* socNet):BasedRecommend(socNet)
{
}


FriendRecAAN::~FriendRecAAN(void)
{
}
void calRecResult(map<int,float>* recResult,int user1id){
    
}

void FriendRecAAN::Recommend(string checkinDataFileName){
    /*
    cout<<checkinDataFileName<<endl;
    ifstream testfile(checkinDataFileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> * recResult = NULL;//针对用户i的推荐结果，其中第一个int表示item的id，float表示预测到的评价值
    map<int,float> sortedRec1;//Top-k个推荐结果TOPN1-5
    map<int,float> sortedRec2;//Top-k个推荐结果TOPN2-10
    map<int,float> sortedRec3;//Top-k个推荐结果TOPN3-20

    int rightRec[3]={0};//推荐正确的数量
    int allRec[3]={0};//推荐的所有的元素数量
    int rightCase=0;//应该被推荐的数量


    while(true){
        i++;
        //读一行签到记录并解析
        if(testfile.eof()){
            cout<<"结束，最终结果："<<endl;
            break;
        }
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=2){
            break;
        }
        int user1id=atoi(result[0].data());
        int user2id=atoi(result[1].data());
        Item * user1=socialNet->getItemPtrById(user1id,ITEMTYPE_USER);
        Item * user2 = socialNet->getItemPtrById(user2id, ITEMTYPE_LOCATION);
        if(user1==NULL||user2==NULL){//如果用户或者位置不存在
            continue;
        }

        if(user1id!=lastUserId){//针对于一个新的用户，为其推荐TOP-K的地点
            if(allRec[0]!=0){
                //cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<"friendrec_aan"<<checkinDataFileName<<endl;
                cout<<" 精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                // cout<<"top-10:"<<endl;
                // printMap(sortedRec2);
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                //cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" 精确率rightRec[2]/allRec[2]："<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"召回率rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //首先清除一些东西

            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();


            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            calRecResult(recResult,user1id);
            cout<<"推荐结果：（位置ID，推荐度）"<<user1id<<endl;
            //printMap(recResult);

            //选择top-k个推荐结果
            sortRecResult(sortedRec1,*recResult,TOPN1);
            sortRecResult(sortedRec2,*recResult,TOPN2);
            sortRecResult(sortedRec3,*recResult,TOPN3);
            cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<userid<<endl;
            //printMap(sortedRec1);
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
    cout<<"UCFR"<<fileName<<endl;
    cout<<"完成："<<endl;
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
    */
}