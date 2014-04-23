#pragma once
#include "basedrecommend.h"
#define ISDEBUG 0
class FriBasedRecommend :
    public BasedRecommend
{
public:
    FriBasedRecommend(Socialnet* socNet);

    void Recommend(string checkinDataFileName);
    ~FriBasedRecommend(void);
};

