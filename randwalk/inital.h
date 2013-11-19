#pragma once
#include<vector>
#include<map>
#include<string>
#define TYPEUSER 0
#define TYPELOCATION 1
#define NOALLOWNEW 1

//CHECKINDATA和FRIENDDATA是原数据，包括签到数据和好友关系数据
//当使用基于元路径的推荐算法时
//CHECKTESTDATA表示下一个时间段的签到记录，以此获得正例或者负例
//TRAINDATA后改为FEATUREDATA
//FEATUREDATA表示输出文件，可能是测试集文件或者训练集文件，文件格式是正例或者负例。

//生成训练集所用的文件：
#define TRAIN_CHECKINDATA "d:\\data\\traindata1.txt"
#define TRAIN_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TRAIN_CHECKTESTDATA "d:\\data\\traindata2.txt"
#define TRAIN_FEATUREDATA "d:\\data\\trainfeaturedata.txt"

//生成测试集：
#define TEST_CHECKINDATA "d:\\data\\testdata1.txt"
#define TEST_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TEST_CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
#define TEST_FEATUREDATA "d:\\data\\testfeaturedata.txt"

//生成训练集所用的文件（不考虑LL边）
#define TRAIN_FEATUREDATANOLLEDGE "d:\\data\\trainfeaturedata_nolledge.txt"
//生成训练集所用的文件（不考虑LL边）
#define TEST_FEATUREDATANOLLEDGE "d:\\data\\testfeaturedata_nolledge.txt"

#define ISDEBUG 0

#define CPU_COUNT 0//0-直接将计算的值作为权值
#define CPU_LOG 1//1-log
//outputNegMod: 1显示和正例相同的负例-train集，0显示所有的负例-test集"
#define NEGMOD_ALL_FORTEST 0
#define NEGMOD_ALL_FORTRAIN 1



/*
2010年9月前的数据为traindata1，
2010年9月的数据为traindata2，
2010年9月前和9月的数据为testdata1，
2010年10月的数据为testdata2

"d:\\data\\filter_testdata2.txt":   该文件是对于测试集中对比文件中进行了处理，在这个阶段签到次数不足20的用户删掉

*/


/*
并发：
#define CHECKINDATA "d:\\data\\useful\\testdata1.txt"
#define FRIENDDATA "d:\\data\\useful\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\useful\\filter"
#define TRAINDATA "d:\\data\\useful\\testfeaturedata.txt"

测试
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
基于用户的协同过滤，基于好友关系的协同过滤
#define CHECKINDATA "d:\\data\\testdata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
#define TRAINDATA "d:\\data\\testfeaturedata.txt"

#define CHECKINDATA "d:\\data\\traindata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\traindata2.txt"
#define TRAINDATA "d:\\data\\trainfeaturedata.txt"

#define CHECKINDATA "d:\\data\\test\\traindata1.txt"
#define FRIENDDATA "d:\\data\\test\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\test\\t.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
/* 
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"

#define CHECKINDATA "d:\\data\\testdata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\testdata2.txt"
#define TRAINDATA "d:\\data\\testfeaturedata.txt"
*/

using namespace std;
class Edge;
class Item;
class User;
class Location;

typedef map<int ,Edge * > EdgeMap;
typedef map<int,User*> UserMap;
typedef map<int,Location*> LocMap;
typedef map<int,float> FeatureMap;


//全局变量，用于存储所有的用户和所有的位置的节点指针


User* getUserPtrbyId(int id,int type=0);
Location* getLocationPtrbyId(int id,int type=0);



