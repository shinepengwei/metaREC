#pragma once
#include<vector>
#include<map>
using namespace std;
class Edge;
class Item;
class User;
class Location;

typedef map<int ,Edge * > EdgeMap;
typedef map<int,User*> UserMap;
typedef map<int,Location*> LocMap;

//全局变量，用于存储所有的用户和所有的位置的节点指针


User* getUserPtrbyId(int id);
Location* getLocationPtrbyId(int id);
