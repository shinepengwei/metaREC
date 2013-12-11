#pragma once
#include "base.h"
#include <string>
#include "Socialnet.h"
#include <set>


class MetaCpu
{
public:
    //读取下一阶段的数据，计算元路径特征值，并且生成正负例并将其保存在文件中。
    void metaCpu(string testDateFileName,string outputFileName);
    MetaCpu(Socialnet *socialNet,bool outputNegEqualPos,bool outputFileWrite);
private:
    Socialnet *socialNet;

    //outputNegEqualPos: 1显示和正例相同的负例-train集，0显示所有的负例-test集
    //outputFileWrite:1表示结果写文件，0表示结果忽略，只为了统计数据
    const bool outputNegEqualPos;
    const bool outputFileWrite;

    int featureCount;




    //计算基于route元路径从id为fromId的ITEM到其他item（所有元路径可以到达的ITEM）的特征值MAP
    FeatureMap calFeature(int fromId,//起始item的ID，返回的特征集为这个item和其他item直接的特征值
        const int *route,//元路径特征
        int length);//描述这个元路径的长度


    //读取下一阶段的数据，计算元路径特征值，并且生成正负例并将其保存在文件中。
    int addNegativeCase(int fromid,//其实item ID
        FeatureMap * metaFea,//特征集存放地点
        ofstream &outFile,//训练集和测试集的输出文件
        int count//找到数量为count的负例,若count=-1，则将所有的负例都返回
        );
    int addAPositiveCase(//尝试生成一个正例，返回是否生成
        Item * user,
        Item * location,
        FeatureMap * metaFea,
        set<int> *uLocSet,
        ofstream &outFile,
        int * metaPathInCount//记录该正例属于那一个元路径
        );

    void printFeatureMap(const FeatureMap *fm) const;
};

