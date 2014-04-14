#pragma once
#include "BasedRecommend.h"
class FriendRecAAN:
    public BasedRecommend
{
private:
    map<int,map<int,float> *> aanFeature;//存储用户之间的aan特征。分别代表着user1_ID,user2_ID,相似度
    void calRecResult(map<int,float>*recResult,int user1id);
public:
    void Recommend(string checkinDataFileName);
    FriendRecAAN(Socialnet* socNet);
    ~FriendRecAAN(void);
};

