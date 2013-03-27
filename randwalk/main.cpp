#include"Edge.h"
#include"item.h"
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include<time.h>
static  UserMap userlist;
static LocMap locationlist;

User* getUserPtrbyId(int id){
    UserMap::iterator iter;
    iter =userlist.find(id);
    if(iter!=userlist.end()){
        return iter->second;
    }
    User *newuser=new User(id);
    userlist.insert(UserMap::value_type(id,newuser));
    return newuser;
}
Location* getLocationPtrbyId(int id)
{
    LocMap::iterator iter;
    iter =locationlist.find(id);
    if(iter!=locationlist.end()){
        return iter->second;
    }
    Location *newloc=new Location(id);
    locationlist.insert(LocMap::value_type(id,newloc));
    return newloc;
}
vector<std::string> split(string str,string pattern)
 {
     string::size_type pos;
     vector<string> result;
     str+=pattern;//扩展字符串以方便操作
     int size=str.size();
 
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             string s=str.substr(i,pos-i);
             result.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return result;
 }
void main()
{
    ifstream myfile("d:\\totalCheckins.txt");
    char buffer[100];
    int i=0;
    while(true){
        i++;
        if(i%100==0){
            cout<<i<<endl;
        }
        if(i==10000){
            break;
        }
        myfile.getline(buffer,100);
        string str(buffer);
        if(str.find("xx")!=str.npos){
            break;
        }
        char tmp[2];
        tmp[0]=9;
        tmp[1]='\0';
        string pattern(tmp);

        vector<std::string> result=split(str,pattern);
        //一行字符串分割后转意
        struct tm t; 
        t.tm_year=atoi(result[1].substr(0,4).data())-1900;
        t.tm_mon=atoi(result[1].substr(5,2).data()-1);
        t.tm_mday=atoi(result[1].substr(8,2).data());
        t.tm_hour=atoi(result[1].substr(11,2).data());
        t.tm_min=atoi(result[1].substr(13,2).data());
        t.tm_sec=atoi(result[1].substr(15,2).data());
        t.tm_isdst=0;
        time_t tsec;
        tsec=mktime(&t);
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        //网络中插入数据
        User * user=::getUserPtrbyId(userid);
        user->addToLocV(locid,tsec);
        Location * loc=::getLocationPtrbyId(locid);
        loc->addToUserV(userid);
    }
    cout<<"\ncount:"<<userlist.size()<<","<<locationlist.size()<<endl;
    int xx;
    cin>>xx;
}