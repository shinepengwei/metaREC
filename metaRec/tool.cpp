#pragma once
#include<string>
#include <vector>
#include "base.h"
#include <iostream>
using namespace std;
vector<std::string> split(string str)
{
    char tmp[2];
    tmp[0]=9;
    tmp[1]='\0';
    string pattern(tmp);
    string::size_type pos;
    vector<string> result;
    str+=pattern;//À©Õ¹×Ö·û´®ÒÔ·½±ã²Ù×÷
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
int itemType(const char t){
    if (t=='L')
    {
        return ITEMTYPE_LOCATION;
    }else if (t=='U')
    {
        return ITEMTYPE_USER;
    }else{
        cout<<"³ÌÐò´íÎó£ºtool.cpp-itemType():"<<t<<endl;
        exit(0);
    }
}
