# -*- coding: cp936 -*-
#useful！！！！！
#对测试集中的用户进行推荐，选择TOP-N的推荐结果，判断是否去过，然后去计算精确率和召回率
#不考虑LL边的情况，只有三个特征
f=open("d:\\data\\filter_testfeaturedata_nolledge.txt",'r')
def calculateResult(x1,x2,x3):
    return x1*-16.2088+x2*-81.0757+x3*-0.1966
#边权值为访问次数：x1*-27.7533+x2*3.2153+x3*5.5221+x4*216.9338+x5*474.9037-0.1486 f=open("d:\\data\\filter_testfeaturedata.txt",'r')
#边权值为lg（访问次数）：x1*97.9885+x2*85.4924+x3*2257.5582+x4*1316.0888+x5*24852.8477-0.0465 f=open("d:\\data\\second\\filter_log_testfeaturedata.txt",'r')
class Result:
    def __init__(self,result,label):
        self.result=result
        self.label=label
    def getResult(self):
        return self.result
    def getLabel(self):
        return self.label
    def setPara(self,result,label):
        self.result=result
        self.label=label
def printlist(rlist):
    i=0
    print "Len:",len(rlist)
    while i<len(rlist):
        print "[",i,"]:",rlist[i].getResult(),",",rlist[i].getLabel()
        i=i+1
def findMinIndex(rlist):
    minr=rlist[0].getResult()
    minIndex=0
    i=1
    while i<len(rlist):
        if minr>rlist[i].getResult():
            minIndex=i
            minr=rlist[i].getResult()
        i=i+1
    return minIndex
def calPostiveCountAtN(resultlist,N):
    if len(resultlist)==0:
        return -1
    i=0
    posCount=0
    if 20==N:
        while i<len(resultlist):
            if resultlist[i].getLabel()==1:
                posCount=posCount+1
            i=i+1
        return posCount
    else:#如果是找前5个或者前10个
        minIndex=-1
        rlist=[]
        while i<len(resultlist):#找到前N个
            if i<N:
                rlist.append(resultlist[i])
                minIndex=findMinIndex(rlist)
            else:
                if resultlist[i].getResult()>rlist[minIndex].getResult():
                    rlist[minIndex]=resultlist[i]
                    minIndex=findMinIndex(rlist)
            i=i+1
         #print "top-",N,":"
        i=0
        while i<N and i<len(resultlist):
            #print rlist[i].getResult(),",",rlist[i].getLabel()
            if rlist[i].getLabel()==1:
                posCount=posCount+1
            i=i+1
        return posCount
                
            
lastUserId=-1
resultlist=[]
minIndex=-1
postiveCount=0
i=0
alluserCount=0
alltop5=0
alltop10=0
alltop20=0
allPosCount=0
while True:
    i=i+1
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split(',');
    userId=int(arr[0])
    label=int(arr[5])
    if userId!=lastUserId:#deal with new user, calculate the last User's result 新用户
        #calculate recommendation result based on resultlist
        if lastUserId!=-1:#首先对上个用户结果进行整理
            print "resultlist"
            printlist(resultlist)
            print "======"
            t5=calPostiveCountAtN(resultlist,5)
            t10=calPostiveCountAtN(resultlist,10)
            t20=calPostiveCountAtN(resultlist,20)
            print lastUserId,":",postiveCount,",",t5,",",t10,",",t20
            alltop5=alltop5+t5
            alltop10=alltop10+t10
            alltop20=alltop20+t20
            alluserCount=alluserCount+1
            allPosCount=allPosCount+postiveCount
            print "Precision:",float(alltop5)/(5*alluserCount),",",float(alltop10)/(10*alluserCount),",",float(alltop20)/(20*alluserCount)
            print "Recall:",float(alltop5)/(allPosCount),",",float(alltop10)/(allPosCount),",",float(alltop20)/(allPosCount)
        #初始化
        lastUserId=userId
        resultlist=[]
        minIndex=-1
        postiveCount=0
    if len(resultlist)<20:
        resultlist.append(Result(calculateResult(float(arr[2]),float(arr[3]),float(arr[4])),label))
        minIndex=findMinIndex(resultlist)
    else:
        re=calculateResult(float(arr[2]),float(arr[3]),float(arr[4]))
        if(re>resultlist[minIndex].getResult()):
            #printlist(resultlist)
            #print "New max Element:",re,",",arr[7],"old min MaxElement:[",minIndex,"]:",resultlist[minIndex].getResult(),",",resultlist[minIndex].getLabel()
            resultlist[minIndex].setPara(re,label)
            minIndex=findMinIndex(resultlist)
            #print "new minIndex of max Element:",minIndex
    if label==1:
        postiveCount=postiveCount+1
f.close()
