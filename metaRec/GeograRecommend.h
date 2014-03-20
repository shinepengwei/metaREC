#pragma once
#include "BasedRecommend.h"

class GeograRecommend :
    public BasedRecommend
{
private:

    void CalRecResultForGEO(map<int,float>&recResult,int uid);
    float GetDistance(float longitude1, float latitude1, float longitude2, float latitude2);
    float CalProbability(float distance);

public:
    void Recommend(string checkinDataFileName, bool isUSG = false);

    void statistics(string outPutFileName);
    GeograRecommend(Socialnet* socNet);
    ~GeograRecommend(void);
};

