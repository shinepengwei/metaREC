#include<string>
#include <vector>
using namespace std;
vector<std::string> split(string str)
{
    char tmp[2];
    tmp[0]=9;
    tmp[1]='\0';
    string pattern(tmp);
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
