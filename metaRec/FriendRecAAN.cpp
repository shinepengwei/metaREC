#include "FriendRecAAN.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>
#include <math.h>
using namespace std;
FriendRecAAN::FriendRecAAN(Socialnet* socNet):BasedRecommend(socNet)
{

}


FriendRecAAN::~FriendRecAAN(void)
{
}
void FriendRecAAN::calRecResult(map<int,float>* recResult,int user1id){
    Item * user1 = this->socialNet->getItemPtrById(user1id,ITEMTYPE_USER);
    for (EdgeMap::const_iterator user1friendsIter = user1->getToUserE()->begin(); user1friendsIter != user1->getToUserE()->end(); ++user1friendsIter){
        Item * user1friend = socialNet->getItemPtrById(user1friendsIter->first,ITEMTYPE_USER);
        for (EdgeMap::const_iterator user2Iter = user1friend->getToUserE()->begin();user2Iter != user1friend->getToUserE()->end(); ++user2Iter){
            Item * user2 = socialNet->getItemPtrById(user2Iter->first, ITEMTYPE_USER);
            if (user2->getId() == user1id || user1->getToUserE()->find(user2->getId()) != user1->getToUserE()->end())
            {
                //如果user2和user1相同，或者他们已经是朋友，忽略
                continue;
            }
            if (recResult->find(user2->getId()) != recResult->end())
            {
                //已经计算过了
                continue;
            }
            //对于用户user1，计算他和其所有的朋友的朋友(user2)之间的相似度
           // if (this->aanFeature.find(user2->getId()) != aanFeature.end())
            //{
                //曾经计算过user2和user1的，与user1和user2的相同
               // recResult->insert(map<int,float>::value_type(user2->getId(),aanFeature[user2->getId()]->at(user1id)));
            //}else{
                //没有计算过
                float resultf = 0.0f;
                for (EdgeMap::const_iterator user1friendsIterForCal = user1->getToUserE()->begin(); user1friendsIterForCal != user1->getToUserE()->end(); ++user1friendsIterForCal){
                    if (user2->getToUserE()->find(user1friendsIterForCal->first) != user2->getToUserE()->end())
                    {
                        Item * coFriend = socialNet->getItemPtrById(user1friendsIterForCal->first,ITEMTYPE_USER);
                        resultf += 1.0f/log((double)coFriend->getToUserE()->size());
                    }
                }
                recResult->insert(map<int,float>::value_type(user2->getId(),resultf));
            //}
        }
    }
    //aanFeature.insert(map<int, map<int,float>*>::value_type(user1id,recResult));
}

void FriendRecAAN::Recommend(string friendData2FileName){
    cout<<friendData2FileName<<endl;
    ifstream testfile(friendData2FileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> * recResult = NULL;//针对用户i的推荐结果，其中第一个int表示item的id，float表示预测到的评价值
    map<int,float> sortedRec1;//Top-k个推荐结果TOPN1-5
    map<int,float> sortedRec2;//Top-k个推荐结果TOPN2-10

    int rightRec[2]={0};//推荐正确的数量
    int allRec[2]={0};//推荐的所有的元素数量
    int rightCase[2]={0};//应该被推荐的数量

    int rightRecOfUser[2] = {0};
    int rightCaseOfUser=0;

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

            if (rightCaseOfUser>=TOPN2 )//|| rightRecOfUser[1]!=0
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;

                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;

            }
            if(allRec[0]!=0){
                //cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<"friendrec_aan"<<friendData2FileName<<endl;
                cout<<" 精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase[0]"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
                // cout<<"top-10:"<<endl;
                // printMap(sortedRec2);
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase[1]"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
            }
            //首先清除一些东西

            sortedRec1.clear();
            sortedRec2.clear();
            for (int i=0;i<2;i++)
            {
                rightRecOfUser[i]=0;
            }
            rightCaseOfUser=0;

            if (recResult != NULL)
            {
                recResult->clear();
                delete recResult;
            }
            
            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            recResult = new map<int,float>();
            calRecResult(recResult,user1id);
            //cout<<"推荐结果：（位置ID，推荐度）"<<user1id<<endl;
            //printMap(recResult);

            //选择top-k个推荐结果
            sortRecResult(sortedRec1,*recResult,TOPN1);
            sortRecResult(sortedRec2,*recResult,TOPN2);
            cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<user1id<<endl;
            //printMap(sortedRec1);
            lastUserId=user1id;
        }
        //计算精确率和召回率
        //第一步统计推荐正确的数量
        if(sortedRec1.find(user2id)!=sortedRec1.end()){
            if(sortedRec1[user2id]!=-1){
                rightRecOfUser[0]++; 
                sortedRec1[user2id]=-1;
            }
        }
        if(sortedRec2.find(user2id)!=sortedRec2.end()){
            if(sortedRec2[user2id]!=-1){
                rightRecOfUser[1]++;    
                sortedRec2[user2id]=-1;
            }
        }
         rightCaseOfUser++;
    }
    //计算精确率和召回率
    cout<<"FriendRecAAN"<<friendData2FileName<<endl;
    cout<<"完成："<<endl;
    cout<<"top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"召回率"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase[1]<<endl;
}
