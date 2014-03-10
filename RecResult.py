# -*- coding: cp936 -*-
#useful！！！！！
#对测试集中的用户进行推荐，选择TOP-N的推荐结果，判断是否去过，然后去计算精确率和召回率
#



f=open("d:\\data\\output\\featuredata_binary_test_foursquare_4hours.txt",'r')
def calculateResult(x1,x2,x3,x4,x5):
    return x1*42.6836+x2*16.7155+x3*766.2384+0.1656
    #return x1*24.5396+x2*2.8798+x3*596.2351+x4*45.1052+x5*28.0792+ 0.1511#f=open("d:\\data\\output\\featuredata_normal_test_4hours.txt",'r')
    #return x1*42.6274+x2*16.8803+x3*710.334+x4*60.9917+x5*133.292+0.1671#x1* 21.8995+x2*45.4047+x3*1001.1349+x4*17.2657+x5*158.6714+  0.259f=open("d:\\data\\output\\featuredata_binary_test_4hours.txt",'r')
    #return x1*43.2774+x2*14.4035+x3*724.1983+x4*61.5731+x5*8.7314+0.1683#f=open("d:\\data\\output\\featuredata_binary_test_foursquare_2hours.txt",'r')

    #return x1*44.7582+x2*2.0329+x3*690.4533+x4*57.8568+x5*18.1732+0.1688#f=open("d:\\data\\output\\featuredata_binary_test_foursquare_12hours.txt",'r')
    #return x1*41.3699+x2*21.5608+x3*722.7382+x4*84.2294+x5*22.7099+0.1695#f=open("d:\\data\\output\\featuredata_binary_test_foursquare_8hours.txt",'r')
    #return x1*42.6473+x2* 16.919+x3*710.7946+x4*62.7087+x5*132.6799+ 0.1671#f=open("d:\\data\\output\\featuredata_binary_test_foursquare_6hours.txt",'r')
    #return x1*42.6274+x2*16.8803+x3*710.334+x4*60.9917+x5*133.292+0.1671#f=open("d:\\data\\output\\featuredata_binary_test_foursquare_4hours.txt",'r')
    #return x1*-21.8147+x2*-45.9338+x3*1008.2965+x4*19.847+x5*144.9907-0.2577 #f=open("d:\\data\\output\\featuredata_binary_test.txt",'r')
    #return x1*5.4772-0.1123+x2*13.4922+x3* 220.7017#log f=open("d:\\data\\output\\featuredata_log_test_LL4hours.txt",'r')
    #return x1*5.5916+x2*13.4818+x3*182.0857+x4*20.9178+x5*69.249-0.1265# log 考虑LL边f=open("d:\\data\\output\\featuredata_log_test_LL4hours.txt",'r')
    #return #binary no ll
    #return x1*-21.8147+x2*-45.9338+x3*1008.2965+x4*19.847+x5*144.9907-0.2577#binary
    #return x1*-7.7742+x2*-19.6375+x3*646.011+x4*5.0097+x5*162.2821-0.1989 #normal 24小时 f=open("d:\\data\\output\\featuredata_normal_test.txt",'r')
    #return x1*-7.978+x2*-17.882+x3*839.9654-0.1882 #normal
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
    label=int(arr[7])
    if userId!=lastUserId:#deal with new user, calculate the last User's result 新用户
        #TODO calculate recommendation result based on resultlist
        if lastUserId!=-1:#首先对上个用户结果进行整理
            #print "resultlist"
            #printlist(resultlist)
            #print "======"
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
        resultlist.append(Result(calculateResult(float(arr[2]),float(arr[3]),float(arr[4]),float(arr[5]),float(arr[6])),label))
        minIndex=findMinIndex(resultlist)
    else:
        re=calculateResult(float(arr[2]),float(arr[3]),float(arr[4]),float(arr[5]),float(arr[6]))
        if(re>resultlist[minIndex].getResult()):
            #printlist(resultlist)
            #print "New max Element:",re,",",arr[7],"old min MaxElement:[",minIndex,"]:",resultlist[minIndex].getResult(),",",resultlist[minIndex].getLabel()
            resultlist[minIndex].setPara(re,label)
            minIndex=findMinIndex(resultlist)
           # print "new minIndex of max Element:",minIndex
    if label==1:
        postiveCount=postiveCount+1
f.close()
