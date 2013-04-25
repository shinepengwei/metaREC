#pragma once
#include<vector>
#include<map>
#define TYPEUSER 0
#define TYPELOCATION 1
#define NOALLOWNEW 1
#define CHECKINDATA "d:\\data\\test\\traindata1.txt"
#define FRIENDDATA "d:\\data\\test\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\test\\traindata2.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
/* 
#define CHECKINDATA "d:\\data\\traindata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\traindata2.txt"
#define TRAINDATA "d:\\data\\trainfeaturedata.txt"

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
