#pragma once
#include <map>
#include "Socialnet.h"
#define TOPN1 5
#define TOPN2 10
#define TOPN3 20
class BasedRecommend
{
protected:
    map<int, map<int,float>> allf2fWeight;

    Socialnet * socialNet;
    void calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid);
    void sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N=20);
    void printMap(const map<int,float> &mapt );
     //根据用户ID，计算该用户和他的朋友之间的朋友度（好友的亲密程度）
    void calculateSIik(map<int,float>& f2fWeightMap,int userid);
    //根据用户的历史记录，计算用户之间的相似度
    void calculateWik(map<int,float> & wikMap,int userid);
public:
    BasedRecommend(Socialnet * socNet);
    ~BasedRecommend(void);
     
};

