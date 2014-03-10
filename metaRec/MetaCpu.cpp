#include "MetaCpu.h"
#include<fstream>
#include <iostream>
#include<sstream>
#include<iterator>
#include "Edge.h"
extern int isDebug;
MetaCpu::MetaCpu(Socialnet *socialNet,bool outputNegEqualPos,bool outputFileWrite):outputFileWrite(outputFileWrite),outputNegEqualPos(outputNegEqualPos){
    this->socialNet=socialNet;
    featureCount=6;
    if (!socialNet->hasLLRelation) featureCount=3;
}

//读取下一阶段的数据，计算元路径特征值，并且生成正负例并将其保存在文件中。
void MetaCpu::metaCpu(string testDateFileName,string outputFileName){
    FeatureMap metaFeature[6];
    int route0[3]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION};//UUL
    int route1[4]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION};//UUUL
    int route2[4]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_USER,ITEMTYPE_LOCATION};//"ULUL";
    int route3[3]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"ULL";
    int route4[4]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"ULLL";
    int route5[4]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"UULL";

    ifstream testfile(testDateFileName);
    ofstream outfile(outputFileName,ios::out);
   

    //统计信息
    int userCount=0;
    int allCount=0;//所有的签到记录数量
    int metaPathInCount[6];//属于元路径到达的签到数量
    for(int i=0;i<featureCount;i++) metaPathInCount[i]=0;
    int reCount=0;//和train阶段重复的签到数量
    int testReCount=0;//在test阶段已经生成了正例的签到数量
    int noCount=0;//不属于元路径可到到的位置的签到数量
    int nullCount=0;//用户或者位置节点不属于原始数据集的签到数量

    int positiveCount=0;//正例数量
    int negativeCount=0;//放入数据集负例数量
    int trueNegCount=0;//真实的全部负例数量


    cout<<"读取比较文件生成正负例："<<testDateFileName<<endl;
    cout<<"结果写入文件："<<outputFileName<<endl;
    char buffer[100];
    int lastuserid=-1;
    int lastPosCount=0;//上一个userid的正例
    set<int> uLocSet;//用户到达过的位置集合
    string postiveStr="";//存储将要写入文件的正例字符串，在生成测试集阶段，如果正例不足20条就不写入，也不生成负例。
    while (true)
    {
        allCount++;
        if(allCount%10000==0) cout<<allCount<<",";
        if(testfile.eof()){
            break;
        }
        
        //对记录进行初步处理
        testfile.getline(buffer,100);
        string str(buffer);
        if (isDebug)
        {
            cout<<"处理记录："<<endl;
            cout<<str<<endl;
        }
        vector<std::string> result=split(str);
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        Item * user=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
        Item * location=socialNet->getItemPtrById(locid,ITEMTYPE_LOCATION);
        if(user==NULL||location==NULL){//如果用户或者位置不存在
            nullCount++;
            continue;
        }
        

        //如果发现用户id是一个新的用户，先对旧的用户进行处理（添加负例），然后在计算新的用户的元路径能到达的位置
        if(userid!=lastuserid){
          
            userCount++;
            uLocSet.clear();//清空用户访问的位置集合

            //把上一个User中的剩余签到作为负例
            if(lastuserid!=-1){
                negativeCount+=lastPosCount;//添加和正例数量相同的负例
                if(lastPosCount>=20||outputNegEqualPos){
                    //如果正例数量大于20，或者输出与正例相同的负例，才进行分析
                    if(isDebug){
                        cout<<"正例写入文件：*****"<<endl;
                        cout<<postiveStr<<"*********************"<<endl;
                    }
                    outfile<<postiveStr;
                    trueNegCount+=addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);

               }
            }

            //出现一个新的userid，找到这个用户通过元路径到达的所有位置，并计算出用户-位置的元路径特征值
            metaFeature[0]=calFeature(userid,route0,2);
            metaFeature[1]=calFeature(userid,route1,3);
            metaFeature[2]=calFeature(userid,route2,3);
            metaFeature[3]=calFeature(userid,route3,2);
            metaFeature[4]=calFeature(userid,route4,3);
            metaFeature[5]=calFeature(userid,route5,3);
            if (isDebug) printFeatureMap(metaFeature);
            lastuserid=userid;
            lastPosCount=0;
             postiveStr="";
        }


        //根据当前记录（user,location），以及获得的该用户的特征值集合，尝试添加一个正例
        int reFlag=addAPositiveCase(user,location,metaFeature,&uLocSet,metaPathInCount,postiveStr);
        if (reFlag==-2)
        {
            if (isDebug) cout<<"上一阶段（构建LBSN网络）已经访问过该位置"<<endl;
            reCount++;
        }else if (reFlag==-1)
        {
            if (isDebug) cout<<"这一阶段（生成正负例）已经访问过该位置"<<endl;
            testReCount++;
        }else if (reFlag==-3)
        {
            if (isDebug) cout<<"该位置不属于任何元路径"<<endl;
            noCount++;
        }else if (reFlag==1)
        {
            lastPosCount++;
            positiveCount++;
        }
    }

    //将最后的一个Userid的负例插入
    negativeCount+=lastPosCount;
    if(lastPosCount>=20||outputNegEqualPos){
        //如果正例数量大于20，或者输出与正例相同的负例，才进行分析
        if(isDebug){
            cout<<"正例写入文件：*****"<<endl;
            cout<<postiveStr<<"*********************"<<endl;
        }
        outfile<<postiveStr;
        trueNegCount+=addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);

    }

    //输出各种参数
    cout<<"测试数据集中出现的所有的用户数量userCount："<<userCount<<endl;
    cout<<"所有的签到记录数量AllCount:"<<allCount<<endl;
    for(int i=0;i<featureCount;i++)
        cout<<"属于元路径到达的签到数量metaPathInCount["<<i<<"]:"<<metaPathInCount[i]<<endl;

    cout<<"重复的签到数量ReCount:"<<reCount<<endl;
    cout<<"在test阶段重复的签到数量TestReCount:"<<testReCount<<endl;
    cout<<"不属于元路径可到到的位置的签到数量noCount:"<<noCount<<endl;
    cout<<"用户或者位置节点不属于原始数据集的签到数量nullCount"<<nullCount<<endl;
    cout<<"正例数量positiveCount："<<positiveCount<<endl;
    cout<<"选择的输出负例数量negativeCount："<<negativeCount<<endl;
    cout<<"负例数量trueNeCount："<<trueNegCount<<endl;
    outfile.close();
}


int MetaCpu::addAPositiveCase(
    //根据当前记录（user,location），以及获得的该用户的特征值集合，尝试添加一个正例
    Item * user,
    Item * location,
    FeatureMap * metaFea,
    set<int> *uLocSet,
    int *metaPathInCount,
    string & postiveStr////存储将要写入文件的正例字符串，在生成测试集阶段，如果正例不足20条就不写入，也不生成负例。
    ){
        EdgeMap *userEdgeList=user->getToLocE();
        if(userEdgeList->find(location->getId())!=userEdgeList->end()){//如果用户在构建社交网络阶段已经访问过该位置
            return -2;
        }
        if(uLocSet->find(location->getId())!=uLocSet->end()){//用户在用例生成阶段已经访问过//当出现用户在用例生成阶段第二次访问某一位置时，不再生成正例。
            return -1;
        }
        int flag=0;
        FeatureMap::iterator miter;

        float tmpFeature[6];
        for(int i=0;i<featureCount;i++){//从六个特征map里面分别找lid
            miter=metaFea[i].find(location->getId());
            if(miter!=metaFea[i].end()){
                //找到一个，说明通过此元路径可以到达该位置，则次元路径可以作为用户-位置对的一个特征
                tmpFeature[i]=miter->second;
                metaPathInCount[i]++;
                flag=1;
                metaFea[i].erase(miter);//并将此位置从特征MAP里面删掉，以防止负例选择时选择到他。
            }else{//此元路径无法从用户到达该位置
                tmpFeature[i]=0;
            }
        }
        if(flag==0) return -3;//不属于任何元路径可以访问得到的位置
        else{//添加一个正例
            int locid=location->getId();
            uLocSet->insert(locid);
            if(outputFileWrite){
                stringstream ss;
                string tmp;
                ss<<user->getId()<<","<<locid;
                for(int i=0;i<featureCount;i++)
                    ss<<","<<tmpFeature[i];
                ss<<",1";
                ss>>tmp;
                postiveStr.append(tmp);
                postiveStr.append("\n");
                if (isDebug)
                {
                    cout<<"生成正例："<<endl;
                    cout<<user->getId()<<","<<locid;
                    for(int i=0;i<featureCount;i++)
                        cout<<","<<tmpFeature[i];
                    cout<<",1"<<endl;
                }
            }
           return 1;
        }
}

//计算基于route元路径从id为fromId的ITEM到其他item（所有元路径可以到达的ITEM）的特征值MAP
FeatureMap MetaCpu::calFeature(int fromId,const int *route,int length){
    FeatureMap result;
    Item *item1=socialNet->getItemPtrById(fromId,route[0]);
    if(item1==NULL) return result;

    string kongbai="    ";
    if (isDebug)  {
        cout<<"开始计算一个Item的一个元路径特征***************"<<endl;
        cout<<"计算初始节点为ID的item到其他Item的特征值"<<endl<<"初始节点："<<endl;item1->print(route[0]);
    }

    EdgeMap *edgelist1=item1->getToItemE(route[1]);
    float allweight1=item1->getAllWeight(route[1]);

    for(EdgeMap::const_iterator iter1=edgelist1->begin();iter1!=edgelist1->end();++iter1){
        int id2=iter1->first;
        float weight1;
        if(iter1->second!=NULL){
            weight1=iter1->second->getWeight();
            if (weight1==0) continue;
        }
        float feat1=weight1/allweight1;
        
        Item * item2=socialNet->getItemPtrById(id2,route[1]);
        if (item2==NULL) continue;

        if (isDebug)
        {
            cout<<kongbai<<"初始节点到第二个节点："<<weight1<<"/"<<allweight1<<"="<<feat1<<endl;
            cout<<kongbai<<"第二个节点："<<endl;
            item2->print(route[1],1);

        }

        EdgeMap *edgelist2=item2->getToItemE(route[2]);
        float allweight2=item2->getAllWeight(route[2]);
        for(EdgeMap::const_iterator iter2=edgelist2->begin();iter2!=edgelist2->end();++iter2){
            int id3=iter2->first;
            float weight2;
            if(iter2->second!=NULL){
                weight2=iter2->second->getWeight();
                if (weight2==0) continue;
            }
            float feat2=weight2/allweight2;
            if (isDebug) cout<<kongbai<<kongbai<<"第二节点到第三个节点："<<weight2<<"/"<<allweight2<<"="<<feat2<<endl;
            if(length==2){
                if(!socialNet->isNeighbor(fromId,route[0],id3,route[2])){
                    //这个节点和原节点没有连接，计算他们之间的元路径特征值
                    float feature=feat1*feat2;
                    FeatureMap::iterator fiter=result.find(id3);
                    if(fiter==result.end()){
                        if (isDebug) cout<<kongbai<<kongbai<<"**生成一个新的特征值:"<<id3<<"-"<<feature<<endl;
                        result.insert(FeatureMap::value_type(id3,feature));
                    }
                    else{
                        if (isDebug) cout<<kongbai<<kongbai<<"**更新特征值（存在其他路径可达）:"<<id3<<"-"<<feature<<"+"<<fiter->second<<"="<<feature+fiter->second<<endl;
                        fiter->second=feature+fiter->second;
                    }
                }else{
                    if (isDebug) cout<<kongbai<<kongbai<<"**两个节点以存在边"<<endl;
                }
            }else{
                Item * item3=socialNet->getItemPtrById(id3,route[2]);
                if(item3==NULL) continue;
                if (item3->getId()==fromId&&item3->getType()==route[0])
                {
                    if (isDebug) {cout<<kongbai<<kongbai<<"第三个节点和第一个节点重复，忽略"<<endl;}
                    continue;
                }
                if (isDebug) {cout<<kongbai<<kongbai<<"第三个节点："<<endl;item3->print(route[1],2);}
                EdgeMap *edgelist3=item3->getToItemE(route[3]);
                float allweight3=item3->getAllWeight(route[3]);

                for(EdgeMap::const_iterator iter3=edgelist3->begin();iter3!=edgelist3->end();++iter3){
                    int id4=iter3->first;
                    float weight3=1;
                    if(iter3->second!=NULL){
                        weight3=iter3->second->getWeight();
                        if (weight3==0) continue;
                    }
                    float feat3=weight3/allweight3;
                    if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"第三节点到第四个节点："<<weight3<<"/"<<allweight3<<"="<<feat3<<endl;
                    if(!socialNet->isNeighbor(fromId,route[0],id4,route[3])){
                        float feature=feat1*feat2*feat3;
                        FeatureMap::iterator fiter=result.find(id4);
                        if(fiter==result.end()){
                            if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**生成一个新的特征值:"<<id4<<"-"<<feature<<endl;
                            result.insert(FeatureMap::value_type(id4,feature));
                        }
                        else{
                            if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**更新特征值（存在其他路径可达）:"<<id4<<"-"<<feature+fiter->second<<endl;
                            fiter->second=feature+fiter->second;
                        }
                    }else{
                        if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**两个节点以存在边"<<endl;
                    }
                }
            }
        }
    }
    if (isDebug)
    {
        cout<<"计算该Item的该元路径特征值结束*************"<<endl;
    }
    return result;
}
//找到数量为count的负例,若count=-1，则将所有的负例都返回
int MetaCpu::addNegativeCase(int fromid,
    FeatureMap * metaFea,
    ofstream &outFile,
    int count){
    /*
        经过正例的选择，现在metaFea里面剩下的全部都是负例的特征值。
        所以，函数需要解决的问题时如何根据特征集合metaFea找出需要的负例。
        找出方法：
        先从第一个元路径可达的节点开始，找到元路径特征值，即为一个负例
        然后找第二个元路径可达的节点，这时，第一个元路径可达的节点在第一次处理时被删掉，所以现在找到的节点第一元路径特征值为0.
        以此类推。
    */
    if(count==0) return 0;
    if(!outputNegEqualPos)  count=-1;
    


    int negcount=0;//选择的负例数量
    int trueNegCount=0;//原始的负例数量
    bool flag=true;//是否还要继续添加负例

    if (isDebug) printFeatureMap(metaFea);
    for(int i=0;i<featureCount;i++){
        float feat[6];
        if(i>0){
            for(int j=0;j<i;j++){
                feat[j]=0;
            }
        }
        FeatureMap *meF=&metaFea[i];
        for(FeatureMap::iterator iter=meF->begin();iter!=meF->end();++iter){
            int toid=iter->first;
            feat[i]=iter->second;
            if(i!=featureCount-1){
                for(int j=i+1;j<featureCount;j++){
                    FeatureMap* meF2=&metaFea[j];
                    FeatureMap::iterator iter2=meF2->find(toid);
                    if(iter2!=meF2->end()){
                        feat[j]=iter2->second;
                        meF2->erase(iter2);
                    }else{
                        feat[j]=0;
                    }
                }
            }
            trueNegCount++;
            if(count==-1||(flag&&(rand()%100)==1)){
                if(outputFileWrite){
                    outFile<<fromid<<","<<toid<<",";
                    for(int x=0;x<featureCount;x++){
                        outFile<<feat[x]<<",";
                    }
                    outFile<<"0"<<endl;
                    if (isDebug)
                    {
                        cout<<fromid<<","<<toid<<",";
                        for(int x=0;x<featureCount;x++){
                            cout<<feat[x]<<",";
                        }
                        cout<<"0"<<endl;
                    }
                }
                negcount++;
                if(negcount==count){
                    flag=false;
                }

            }
        }
    }
    return trueNegCount;
}

void MetaCpu::printFeatureMap(const FeatureMap *fm) const{

    string kongbai="    ";
    cout<<"****输出特征值集合Feature:"<<endl;
    for(int i=0;i<featureCount;i++){
        cout<<"元路径"<<i<<":"<<endl;
        for(FeatureMap::const_iterator iter=fm[i].begin();iter!=fm[i].end();++iter){
            cout<<kongbai<<iter->first<<":"<<iter->second<<endl;
        }
    }
    cout<<"****特征值集合输出结束****"<<endl;
}