# -*- coding: cp936 -*-
import time
import xlwt
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

    
xlsfile=xlwt.Workbook()
loc_checkin_table=xlsfile.add_sheet('loc_checkin_count')
user_checkin_table=xlsfile.add_sheet('user_checkin_count')
locr_table=xlsfile.add_sheet('locr_count')

f=open("J:\\checkin\\filter_allCheckinNewYork.txt",'r')

locr={0:{0:0}}
user_loc={0:{0:0}}
checkintime=[0]*21
x=0
locr_max=0#位置边的最大权值
trainc=0
testc=0
locckins={0:0}
userckins={0:0}

#逐行处理数据

#第一行初始化,为位置关系等比较
lastline=f.readline()
arr=lastline.split(',')
lasttime=time.mktime(strptime(arr[2],"%d/%m/20%y %H:%M:%S"))
lastuserid=arr[1]
lastlocid=arr[3]

i=0

time={0:0}
while True:
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split(',')
    tm=strptime(arr[2],"%d/%m/20%y %H:%M:%S")
    newtime=time.mktime(tm)
    userid=int(arr[1])
    locidstr=arr[3]

    '''#统计每个签到的时间分布
    if tm.tm_year==2010 and tm.tm_mon>=10:
        testf.write(newline)
        testc=testc+1
    else:
        trainf.write(newline)
        trainc=trainc+1
    '''


    #统计每个用户的签到次数
    
    if userckins.has_key(userid):
        userckins[userid]=userckins[userid]+1
    else:
        userckins[userid]=1
    
   #每个位置的访问次数统计
    if locckins.has_key(locidstr):
        locckins[locidstr]=locckins[locidstr]+1
    else:
        locckins[locidstr]=1

    #存储用户对位置的访问矩阵
    if user_loc.has_key(userid):
        if(user_loc[userid].has_key(locidstr)):
            user_loc[userid][locidstr]=user_loc[userid][locidstr]+1
        else:
            user_loc[userid][locidstr]=1
    else:
        user_loc[userid]={locidstr:1}


#统计及输出

#print 'trainset:',trainc
#print 'testSet:',testc



##统计数据集矩阵密度

allsize=len(userckins)*len(locckins)
print 'userckins:',len(userckins),'  locckins:',len(locckins)
checkinsize=0
for uuid in user_loc.keys():
    checkinsize=checkinsize+len(user_loc[uuid])
print 'raw_density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize

'''
#位置关系统计
print '位置关系统计',locr_max
locrcountlist=[0]*1000
for flid in locr.keys():
    for slid in locr[flid].keys():
        locrcount=locr[flid][slid]
        if(locrcount>=1000): locrcount=999
        locrcountlist[locrcount]=locrcountlist[locrcount]+1
for i in range(0,1000):
    locr_table.write(i,0,i)
    locr_table.write(i,1,locrcountlist[i])


#统计在某一区间的签到数量的位置数量，R为range，N为取样点数量
print '统计在某一区间的签到数量的位置数量'
print '位置的总数量：',len(locckins)
R=1
N=5000
locckincoutlist=[0]*N
for lid in locckins.keys():
    dex=locckins[lid]
    if dex>=N:dex=N-1
    locckincoutlist[dex]=locckincoutlist[dex]+1
for i in range(0,N):
    tmpstr=i
    loc_checkin_table.write(i,0,tmpstr)
    loc_checkin_table.write(i,1,locckincoutlist[i])

    
#统计签到位置数量
print '统计在某一区间的签到数量的用户数量'
print '用户的总数量：',len(userckins)
N=5000
userckincountlist=[0]*N
for uid in userckins.keys():
    dex=userckins[uid]
    if dex>=N:dex=N-1
    userckincountlist[dex]=userckincountlist[dex]+1
for i in range(0,N):
    tmpstr=i
    user_checkin_table.write(i,0,tmpstr)
    user_checkin_table.write(i,1,userckincountlist[i])

xlsfile.save('d:\\data1.xls')
print 'end!'

'''
