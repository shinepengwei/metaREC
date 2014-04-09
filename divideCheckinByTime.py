# -*- coding: cp936 -*-
#useful
###根据签到的时间分为不同的签到集合，通过比较，进一步获得训练集和测试集

import time
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

def timeNum(year,month):
    return year*100+month






path="E:\\checkin\\"
f=open(path+"filter_foursquare_allCheckin.txt",'r')

trainf1=open(path+"foursquare_all_traindata1.txt",'w')
trainf2=open(path+"foursquare_all_traindata2.txt",'w')
testf1=open(path+"foursquare_all_testdata1.txt",'w')
testf2=open(path+"foursquare_all_testdata2.txt",'w')

#foursquare-all:
t1 = 201306
t2 = 201308


testc1=0
testc2=0
trainc1=0
trainc2=0

littleTime=201311
largeTime=0
#逐行处理数据
timeCountStatc=[0,0,0,0,0,0,0,0,0,0,0]

#将string类型的locId改为int型的
locStrIdDic={}
locIdNum=0

while True:
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split(',')#arr=newline.split('\t')
    tm=strptime(arr[2],"%d/%m/20%y %H:%M:%S")#tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    userid=int(arr[1])
    locidstr=arr[3]
    locid=locIdNum
    if locidstr in locStrIdDic.keys():
        locid=locStrIdDic[locidstr]
    else:
        locStrIdDic[locidstr]=locIdNum
        locIdNum+=1
            
    timenum=timeNum(tm.tm_year,tm.tm_mon)
    '''
    if timenum>largeTime: largeTime=timenum
    if timenum<littleTime :littleTime=timenum
    if timenum<=201301:timeCountStatc[0]+=1
    if timenum<=201302:timeCountStatc[1]+=1
    if timenum<=201303:timeCountStatc[2]+=1
    if timenum<=201304:timeCountStatc[3]+=1
    if timenum<=201305:timeCountStatc[4]+=1
    if timenum<=201306:timeCountStatc[5]+=1
    if timenum<=201307:time
    if timenum<=201308:timeCountStatc[7]+=1
    if timenum<=201309:timeCountStatc[8]+=1
    if timenum<=201310:timeCountStatc[9]+=1
    if timenum<=201311:timeCountStatc[10]+=1
    '''

    
    #基于每个签到的时间分布
    nline=str(userid)+'\t20'+str(tm.tm_year)+'-'+str(tm.tm_mon)+'-'+str(tm.tm_mday)+'T'+str(tm.tm_hour)+':'\
    +str(tm.tm_min)+":"+str(tm.tm_sec)+'Z\t20\t20\t'+ str(locid)+'\n'

    if timenum<=201303:
        #train1
        #test1
        trainf1.write(nline)
        trainc1+=1
        testf1.write(nline)
        testc1+=1
    else:
        if timenum<=201305:
            trainf2.write(nline)
            trainc2+=1
            testf1.write(nline)
            testc1+=1
        else:
            testf2.write(nline)
            testc2+=1


print "Train1Count:",trainc1,"     Train2Count:",trainc2,"   Test1Count:",testc1,"   Test2Count:",testc2


print timeCountStatc
