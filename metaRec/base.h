#pragma once
#include<vector>
#include<map>
#include<string>

//权值计算方法
#define WEIGHTCPUTYPE_NORMAL 0//普通
#define WEIGHTCPUTYPE_LOG 1//log
#define WEIGHTCPUTYPE_BINARY 2//0-1
#define WEIGHTCPUTYPE_CLASSIFY 3//分级，TODO

//item类型
#define ITEMTYPE_USER 0
#define ITEMTYPE_LOCATION 1

//EDGE类型
#define EDGETYPE_CHECKIN 0
#define EDGETYPE_FRIEND 1
#define EDGETYPE_LLRELATION 2

//查找item时同时允许插入
#define ALLOW_INSERT_IF_NULL true

#define NOT_CONCERN_LL_RELATION false



using namespace std;
class Edge;
class Item;
typedef map<int ,Edge * > EdgeMap;
typedef map<int,Item*> ItemMap;

typedef map<int,float> FeatureMap;



