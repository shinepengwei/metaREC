#include "Socialnet.h"

#include <iostream>
using namespace std;
//²âÊÔ
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
int isDebug=1;

void main(){
    const string checkinFileName=CHECKINDATA;
    const string friendFileName=FRIENDDATA;
    Socialnet * socialNet=Socialnet::createSocialnet(checkinFileName,friendFileName,WEIGHTCPUTYPE_LOG,NOT_CONCERN_LL_RELATION);
    int cc;
    cin>>cc;

}