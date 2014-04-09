# -*- coding: cp936 -*-
import time
import xlwt
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime


#"d:\\data\\fileter_processedChekins.txt"
#J:\\checkin\\filter_allCheckinNewYork.txt
fileName = "d:\\data\\fileter_processedChekins.txt"
f=open(fileName,'r')
#trainf=open("d:\\trainfilter2.txt",'w+')
#testf=open("d:\\testfilter2.txt",'w+')


#逐行处理数据

#第一行初始化,为位置关系等比较
lastline=f.readline()
arr=lastline.split('\t')
lasttime=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
lastuserid=arr[0]
lastlocid=arr[4]


timeDic={0:0}
i=0
while True:
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    #统计每个签到的时间分布
    yearmon = int(str(tm.tm_year)+str(tm.tm_mon))
    if yearmon in timeDic:
        timeDic[yearmon] = timeDic[yearmon] + 1
    else:
        timeDic[yearmon] =1

        

print "时间分布：",fileName
#统计及输出
for time_yearmon in timeDic:
    print str(time_yearmon)+":"+str(timeDic[time_yearmon])
allcount=0
for x in timeDic:
	allcount=allcount+timeDic[x]
print allcount

print 'end!'

