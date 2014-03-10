#pragma once
#include "BasedRecommend.h"
class UCFRecommend:public BasedRecommend
{
public:
    UCFRecommend(Socialnet* socNet);
    ~UCFRecommend(void);
   
    void Recommend(string fileName);
    
    
};

