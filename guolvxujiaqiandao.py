# -*- coding: cp936 -*-
import time
'''
过滤掉用户的虚假签到，即5分钟之内的连续签到。

'''
#import strptime
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

f=open("d:\\1totalCheckins.txt",'r')
out=file("d:\\1out.txt","w")

#第一行，把时间值存储在lastt中,初始化
lastline=f.readline()
arr=lastline.split('\t')
print arr
lastt=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
lastflag=True
i=0
#后面的每行与前一行判断，如果user相同且时间在五分钟之内，则放弃，否则写入到out。txt中
#（默认为不同用户之间最早的和最晚的签到时间不可能在五分钟之内）
while True:
    line=f.readline()
    arr=line.split('\t')
    i=i+1
    if i%1000==0:
        print i,":",arr
    #获得时间值
    intt=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
    
    if lastt-intt>300:
        if lastflag:
            out.write(lastline)
        lastflag=True
    else:
        lastflag=False
    lastt=intt;
    lastline=line;
    if("xx" in arr[4]):
        print "end:",arr[4],"len:",len(arr[4] and "xx")
        break
out.flush();
out.close();
