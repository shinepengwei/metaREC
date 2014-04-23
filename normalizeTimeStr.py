# -*- coding: cp936 -*-
#useful
###规范化时间字符串

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

path="E:\\checkin\\"
outpath="E:\\checkin\\"
f=open(path+"filter_foursquare_CheckinNewYork.txt",'r')
outf=open(outpath+"foursquare_NewYork_allcheckin.txt",'w')

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
    outf.write(newline)

f.close()
outf.close()
