#include "BasedRecommend.h"

#include <vector>
#include <math.h>
#include <set>
#include<iostream>
#include<iterator>
#include "base.h"
#include "Item.h"
#include "Edge.h"
void BasedRecommend::printMap(const map<int,float> &mapt ){
    for(map<int,float>::const_iterator iter=mapt.begin();iter!=mapt.end();++iter){
        cout<<iter->first<<"     "<<iter->second<<endl;
    }
}
BasedRecommend::BasedRecommend(Socialnet * socNet)
{
    this->socialNet = socNet;
}
void BasedRecommend::calculateWik(map<int,float> & wikMap,int userid){
    Item * u=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
    //计算每一个用户和其他用户的相似度Wi,k
    //临时计算变量，第一个int表示k，第二个int表示对应的值
    map<int,int> cijckjMap;
    int cij2=0;//对于当前的用户，有固定的cij2
    //Wik=cijckj/（sqrt（cij2）*sqrt（ckj2））
    EdgeMap *locEdgeList=u->getToLocE();
    //对于用户u来说，找到用户的所有访问过的位置，然后找到访问过这些位置的用户
    for(EdgeMap::const_iterator liter=locEdgeList->begin();liter!=locEdgeList->end();++liter){
        int lid=liter->first;
        float cij=liter->second->getWeight();
        cij2+=cij*cij;
        Item * loc=socialNet->getItemPtrById(lid,ITEMTYPE_LOCATION);
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
        Item *toUser=socialNet->getItemPtrById(toUId,ITEMTYPE_USER);
        EdgeMap * locList=toUser->getToLocE();
        for(EdgeMap::const_iterator edgIter=locList->begin();edgIter!=locList->end();++edgIter){
            ckj2+=edgIter->second->getWeight()*edgIter->second->getWeight();
        }
        wikMap[toUId]=cijckjMap[toUId]/(sqrt((float)cij2)*sqrt((float)ckj2));
    }
}
void BasedRecommend::calculateSIik(map<int,float>& f2fWeightMap,int userid){
    const float para=0.5;//自定义权值
    Item * u=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
    //计算每一个用户和他的好友的好友度SIk,i
    EdgeMap * frdsEdgeList=u->getToUserE();
    for(EdgeMap::const_iterator fliter=frdsEdgeList->begin();fliter!=frdsEdgeList->end();++fliter){
        int coFrdCount=0;
        int allFrdCount=0;
        int coVisitedCount=0;
        int allVisitedCount=0;
        int fid=fliter->first;
        Item * frdUser = socialNet->getItemPtrById(fid,ITEMTYPE_USER);

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
void BasedRecommend::calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid){
    int ss=0;
    Item * user=socialNet->getItemPtrById(uid, ITEMTYPE_USER);
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
        Item * toUser=socialNet->getItemPtrById(toUserId,ITEMTYPE_USER);//::getUserPtrbyId(toUserId,NOALLOWNEW);
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
                Item *tmpUser=socialNet->getItemPtrById(tmpUid,ITEMTYPE_USER);
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
void BasedRecommend::sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N){
    int leastUserId=-1;
    float leastRanked=INT_MAX;
    int sortedCount=0;
    for(map<int,float>::const_iterator recIter=recResult.begin();recIter!=recResult.end();++recIter){
        if(sortedCount<N){
            sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
            sortedCount++;
            if(sortedCount==N){
                //N个满了，就存储最小的元素
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
BasedRecommend::~BasedRecommend(void)
{
}
