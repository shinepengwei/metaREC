# -*- coding: cp936 -*-
#useful！！！！！
#对测试集中的用户进行推荐，选择TOP-N的推荐结果，判断是否去过，然后去计算精确率和召回率
#

'''
f=open("e:\\data\\output\\gowalla_featuredata_binary_test_2hours.txt",'r')
weight=[30.2944, 51.8266,981.3731,8.2467,159.4689,20.7453,0.2522]

f=open("e:\\data\\output\\gowalla_featuredata_binary_test_4hours.txt",'r')
weight=[30.2809, 51.818,981.3944,8.2335,159.4421,20.7149,0.2521]

f=open("e:\\data\\output\\gowalla_featuredata_binary_test_6hours.txt",'r')
weight=[30.2809, 51.818,981.3944,8.2335,149.4421,20.7149,0.2521]
'''

'''
right:
f=open("e:\\data\\output\\friendREC_gowalla_featuredata_binary_test_4hours.txt",'r')
weight=[32.3062,162.1386,39.3333,163.7769,70.0707,30.3546,0.6199]

f=open("e:\\data\\output\\friendREC_gowalla_featuredata_log_test_4hours.txt",'r')
weight=[55.2417,129.1402,55.3706,63.8615,2.8359,8.2175,0.5667]
'''

#foursquare-newYrok 位置推荐
'''
f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_normal_test_4hours.txt",'r')
weight=[20.7812,37.0776,519.0136,145.5352,29.0935,78.6069,0.1254]

f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_log_test_4hours_new.txt",'r')
weight=[0.7979,1.8155,133.7793,30.6517,92.3578,27.5262,0.0842]

f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_4hours.txt",'r')
weight=[ 46.9529,114.0446,1021.8946,230.0309,304.527,108.9359,0.1586]
#no - ll
f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_4hours.txt",'r')
weight=[ 49.8306,125.5997,898.876,0,0,0,0.1594] 
#no - ll
f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_4hours.txt",'r')
weight=[ 115.5997,0.7979,698.876,0,0,0,0.1594]



#Foursquare-ALL
f=open("E:\\checkin\\result\\foursquare_all_featuredata_normal_test_4hours.txt",'r')
weight=[ 33.9048,56.3842,592.2153,48.5725,349.328,20.3929, 0.1512]
f=open("E:\\checkin\\result\\foursquare_all_featuredata_binary_test_4hours.txt",'r')
weight=[ 63.2306,112.764,1060.1232,68.3359,276.8421,46.9454, 0.2135]
f=open("E:\\checkin\\result\\foursquare_all_featuredata_log_test_4hours.txt",'r')
weight=[ 1.8949,1.4017,322.5896,22.9564,72.0895,9.9466, 0.0957]


#Gowalla
f=open("E:\\checkin\\result\\gowalla_featuredata_binary_test_4hours.txt",'r')
weight=[  8.8539,25.8855,487.6918,2.0132,167.649,28.1172, 0.3214]
f=open("E:\\checkin\\result\\gowalla_featuredata_log_test_4hours.txt",'r')
weight=[  3.9347,16.2318,132.7977,20.4024,30.5372,6.9817, 0.1159]
f=open("E:\\checkin\\result\\gowalla_featuredata_normal_test_4hours.txt",'r')
weight=[ 1.496,
         9.0263,
         275.2982,
         4.5761,
         215.8752,
         7.9142,
          0.2393]
'''

f=open("E:\\checkin\\result\\gowalla_featuredata_binary_test_4hours.txt",'r')
weight=[ 2.8056,17.5661,143.2343,0,0,0,0.3044]


def calculateResult(x1,x2,x3,x4,x5,x6):
    return x1*weight[0] + x2*weight[1] + x3*weight[2] + x4*weight[3] + x5*weight[4] + x6*weight[5] + weight[6]

    #以下没用
    #return x1*42.6836+x2*16.7155+x3*766.2384+0.1656 f=open("d:\\data\\output\\featuredata_binary_test_foursquare_4hours.txt",'r')
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

i=0
alluserCount=[0,1,1]
alltop=[0,0,0]
allPosCount=[0,1,1]
user_postiveCount=0
while True:
    i=i+1
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split(',');
    userId=int(arr[0])
    label=int(arr[8])
    if userId!=lastUserId:#deal with new user, calculate the last User's result 新用户
        #TODO calculate recommendation result based on resultlist
        if lastUserId!=-1:#首先对上个用户结果进行整理
            #print "resultlist"
            #printlist(resultlist)
            #print "======"
            t5=-1
            t10=-1
            t20=-1

            
            if user_postiveCount >= 5 :
                t5=calPostiveCountAtN(resultlist,5)
                alltop[0]=alltop[0]+t5
                alluserCount[0] = alluserCount[0] + 1
                allPosCount[0]=allPosCount[0]+user_postiveCount
                
            if user_postiveCount >= 10 : 
                t10=calPostiveCountAtN(resultlist,10)
                alluserCount[1] = alluserCount[1] + 1
                alltop[1]=alltop[1]+t10
                allPosCount[1]=allPosCount[1]+user_postiveCount

                

            if user_postiveCount >= 20 : 
                t20=calPostiveCountAtN(resultlist,20)
                alluserCount[2] = alluserCount[2] + 1
                alltop[2]=alltop[2]+t20
                allPosCount[2]=allPosCount[2]+user_postiveCount


                
            print lastUserId,":",user_postiveCount,",",t5,",",t10,",",t20
            
            
            
            
            
            print "Precision:",float(alltop[0])/(5*alluserCount[0]),",",float(alltop[1])/(10*alluserCount[1]),",",float(alltop[2])/(20*alluserCount[2])
            print "Recall:",float(alltop[0])/(allPosCount[0]),",",float(alltop[1])/(allPosCount[1]),",",float(alltop[2])/(allPosCount[2])

            
        #初始化
        lastUserId=userId
        resultlist=[]
        minIndex=-1
        user_postiveCount = 0

    #处理当前的一条记录   
    if len(resultlist)<20:
        resultlist.append(Result(calculateResult(float(arr[2]),float(arr[3]),float(arr[4]),float(arr[5]),float(arr[6]),float(arr[7])),label))
        minIndex=findMinIndex(resultlist)
    else:
        re=calculateResult(float(arr[2]),float(arr[3]),float(arr[4]),float(arr[5]),float(arr[6]),float(arr[7]))
        if(re>resultlist[minIndex].getResult()):
            #printlist(resultlist)
            #print "New max Element:",re,",",arr[7],"old min MaxElement:[",minIndex,"]:",resultlist[minIndex].getResult(),",",resultlist[minIndex].getLabel()
            resultlist[minIndex].setPara(re,label)
            minIndex=findMinIndex(resultlist)
           # print "new minIndex of max Element:",minIndex
    if label==1:
        user_postiveCount = user_postiveCount+1
f.close()
