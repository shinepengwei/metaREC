# -*- coding: cp936 -*-
import xlwt
def statistic(alist,N,tablename,name):
    print '统计在某一区间的签到数量的',name,'数量'
    print name,'总数量：',len(alist)
    coutlist=[0]*N
    for aid in alist.keys():
        dex=alist[aid]
        if dex>=N:dex=N-1
        coutlist[dex]=coutlist[dex]+1
    for i in range(0,N):
        tmpstr=i
        tablename.write(i,0,tmpstr)
        tablename.write(i,1,coutlist[i])
'''
def getmaritxdensity(userlist,loclist)
    allsize=len(userlist)*len(userlist)
    checkinsize=0
    for uuid in user_loc.keys():
        checkinsize=checkinsize+len(user_loc[uuid])
    print 'raw_density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize
'''
def setuser_loc(user_loc,userid,locid):
    if user_loc.has_key(userid):
        if(user_loc[userid].has_key(locid)):
            user_loc[userid][locid]=user_loc[userid][locid]+1
        else:
            user_loc[userid][locid]=1
    else:
        user_loc[userid]={locid:1}


def calnetdensity(user_loc,usersize,locsize):
    print 'usersize:',usersize
    print 'locsize:',locsize
    allsize=usersize*locsize
    checkinsize=0
    for uuid in user_loc.keys():
        checkinsize=checkinsize+len(user_loc[uuid])
    print 'density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize


f=open("d:\\filter1.txt",'r')
ffilter=open("d:\\filter2.txt",'w+')
xlsfile=xlwt.Workbook()

locckins={0:0}
userckins={0:0}
checkinsize=0
rawuser_loc={0:{0:0}}

while True:
    #i=i+1
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr[:-2])

    #统计每个用户的签到次数
    if userckins.has_key(userid):
        userckins[userid]=userckins[userid]+1
    else:
        userckins[userid]=1
    
    #每个位置的访问次数统计
    if locckins.has_key(locid):
        locckins[locid]=locckins[locid]+1
    else:
        locckins[locid]=1
        
    setuser_loc(rawuser_loc,userid,locid)

loc_checkin_table=xlsfile.add_sheet('loc_checkin_count')
user_checkin_table=xlsfile.add_sheet('user_checkin_count')
statistic(locckins,2000,loc_checkin_table,'位置')
statistic(userckins,2000,user_checkin_table,'用户')

calnetdensity(rawuser_loc,len(userckins),len(locckins))


#第二轮
    
f.seek(0)
newlocckins={0:0}
newuserckins={0:0}
rawlinecount=0
newlinecount=0
newcheckinsize=0
newuser_loc={0:{0:0}}

while True:
    rawlinecount=rawlinecount+1
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr[:-2])
    if userckins[userid]>10 and locckins[locid]>5:
        newlinecount=newlinecount+1
        ffilter.write(newline)
        user_flag=0##判断用户ID是否曾经出现
        loc_flag=0##判断位置ID是否曾经出现
        #统计每个用户的签到次数
        if newuserckins.has_key(userid):
            newuserckins[userid]=newuserckins[userid]+1
        else:
            user_flag=1##用户ID没有出现过
            newuserckins[userid]=1
       #每个位置的访问次数统计
        if newlocckins.has_key(locid):
            newlocckins[locid]=newlocckins[locid]+1
        else:
            loc_flag=1##位置ID没有出现过
            newlocckins[locid]=1
        setuser_loc(newuser_loc,userid,locid)




newloc_checkin_table=xlsfile.add_sheet('newloc_checkin_count')
newuser_checkin_table=xlsfile.add_sheet('newuser_checkin_count')
statistic(newlocckins,2000,newloc_checkin_table,'位置')
statistic(newuserckins,2000,newuser_checkin_table,'用户')
calnetdensity(newuser_loc,len(newuserckins),len(newlocckins))


#显示统计结果
print 'rawlinecount:',rawlinecount
print 'newlinecount:',newlinecount
print 'float(checkinsize)',float(checkinsize)
print 'float(newcheckinsize)',float(newcheckinsize)

calnetdensity(rawuser_loc,len(userckins),len(locckins))


xlsfile.save('d:\\data3.xls')



    
