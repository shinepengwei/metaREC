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

def timeStr(tm):
    return time.strftime('%Y-%m-%dT%H:%M:%S', tm) 
    '''
    monStr=str(tm.tm_mon)
    if tm.tm_mon<10:
        monStr = "0"+str(tm.tm_mon)
        
    dayStr = str(tm.tm_mday)
    if tm.tm_mday<10:
        dayStr = "0"+ str(tm.tm_mday)
        
    hourStr = str(tm.tm_hour)
    if tm.tm_hour<10:
        dayStr = "0"+ str(tm.tm_hour)
        
    minStr = str(tm.tm_min)
    if tm.tm_min<10:
        dayStr = "0"+ str(tm.tm_min)
        
    secStr = str(tm.tm_sec)
    if tm.tm_sec<10:
        dayStr = "0"+ str(tm.tm_sec)

    return str(tm.tm_year)+'-'+monStr+'-'+dayStr+'T'+hourStr+':'+minStr+":"+secStr
'''

path="D:\\data\\"
outpath="E:\\checkin\\"
f=open(path+"fileter_processedChekins.txt",'r')

trainf1=open(outpath+"gowalla_traindata1.txt",'w')
trainf2=open(outpath+"gowalla_traindata2.txt",'w')
testf1=open(outpath+"gowalla_testdata1.txt",'w')
testf2=open(outpath+"gowalla_testdata2.txt",'w')

#foursquare-all:
#t1 = 201306
#t2 = 201308
#foursquare-NEWYORK:
#t1 = 201305
#t2 = 201307

#gowalla
t1 = 201009
t2 = 201009


testc1=0
testc2=0
trainc1=0
trainc2=0
#逐行处理数据
i=0
while True:
    i=i+1
    if i%10000 ==0:
        print i
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)

    timenum=timeNum(tm.tm_year,tm.tm_mon)


    newline=arr[0]+'\t'+timeStr(tm)+'Z\t'+arr[2]+'\t'+arr[3]+'\t'+ arr[4]

   
    #基于每个签到的时间分布
    if timenum<t1:
        #train1
        #test1
        trainf1.write(newline)
        trainc1+=1
        testf1.write(newline)
        testc1+=1
    else:
        if timenum<=t2:
            trainf2.write(newline)
            trainc2+=1
            testf1.write(newline)
            testc1+=1
        else:
            testf2.write(newline)
            testc2+=1

print "Train1Count:",trainc1,"     Train2Count:",trainc2,"   Test1Count:",testc1,"   Test2Count:",testc2
