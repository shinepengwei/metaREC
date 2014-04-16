#pragma once
#include "basedrecommend.h"
class FriendRecAAENT :
    public BasedRecommend
{
private:
    map<int,map<int,float> *> aaentFeature;//存储用户之间的aan特征。分别代表着user1_ID,user2_ID,相似度
    void calRecResult(map<int,float>*recResult,int user1id);
public:
    void Recommend(string checkinDataFileName);
    FriendRecAAENT(Socialnet* socNet);
    ~FriendRecAAENT(void);
};

