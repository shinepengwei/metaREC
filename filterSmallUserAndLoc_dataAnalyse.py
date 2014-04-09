# -*- coding: cp936 -*-
'''
根据将虚假用户过滤后的签到信息，过滤签到数量小于5的用户和小到数量小于10的位置，然后输出，并且进行签到数量分布的分析。
time：14-3-31
'''
import time
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime
    
import xlwt
def statistic(alist,N,tablename,name):
    print '统计在某一区间的签到数量的',name,'数量'
    print name,'总数量：',len(alist)
    coutlist=[0]*N
    for aid in alist:
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
    if userid in user_loc:
        if(locid in user_loc[userid]):
            user_loc[userid][locid]=user_loc[userid][locid]+1
        else:
            user_loc[userid][locid]=1
    else:
        user_loc[userid]={locid:1}


def calnetdensity(user_loc,usersize,locsize):
    print 'usersize:',usersize print 'locsize:',locsize
    allsize=usersize*locsize checkinsize=0 for uuid in user_loc:
        checkinsize=checkinsize+len(user_loc[uuid])
    print 'density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize

'''
xlsfileName = "J:\\checkin\\foursquare_data_analyse.xls"
fName="J:\\checkin\\allCheckinNewYork.txt"
ffilterName = "J:\\checkin\\filter_allCheckinNewYork.txt"


xlsfileName = "D:\\data\gowalla_data_analyse.xls"
fName="D:\\data\\processedChekins.txt"
ffilterName = "D:\\data\\fileter_processedChekins.txt"
'''

fName="E:\\checkin\\foursquare_allCheckin.txt"
ffilterName = "E:\\checkin\\filter_foursquare_allCheckin.txt"
xlsfileName = "E:\\checkin\\foursquare_allCheckin_data_analyse.xls"


f = open(fName,'r')
ffilter=open(ffilterName,'w+')

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
    locid=int(locidstr)

    #统计每个用户的签到次数
    if userid in userckins:
        userckins[userid]=userckins[userid]+1
    else:
        userckins[userid]=1
    
    #每个位置的访问次数统计
    if locid in locckins:
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
print "第二论"  
f.seek(0)
newlocckins={0:0}
newuserckins={0:0}
rawlinecount=0
newlinecount=0
newcheckinsize=0
newuser_loc={0:{0:0}}
timeDic={0:0}

while True:
    rawlinecount=rawlinecount+1
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr)
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    
    #过滤掉用户签到次数不足10，位置签到次数不足5的节点。
    if userckins[userid]>10 and locckins[locid]>5:
        newlinecount=newlinecount+1
        ffilter.write(newline)
        user_flag=0##判断用户ID是否曾经出现
        loc_flag=0##判断位置ID是否曾经出现
        #统计每个用户的签到次数
        if userid in newuserckins:
            newuserckins[userid]=newuserckins[userid]+1
        else:
            user_flag=1##用户ID没有出现过
            newuserckins[userid]=1
       #每个位置的访问次数统计
        if locid in newlocckins:
            newlocckins[locid]=newlocckins[locid]+1
        else:
            loc_flag=1##位置ID没有出现过
            newlocckins[locid]=1

        setuser_loc(newuser_loc,userid,locid)

            #统计每个签到的时间分布
        yearmon = int(str(tm.tm_year)+str(tm.tm_mon))
        if yearmon in timeDic:
            timeDic[yearmon] = timeDic[yearmon] + 1
        else:
            timeDic[yearmon] =1




newloc_checkin_table=xlsfile.add_sheet('newloc_checkin_count')
newuser_checkin_table=xlsfile.add_sheet('newuser_checkin_count')
statistic(newlocckins,2000,newloc_checkin_table,'位置')
statistic(newuserckins,2000,newuser_checkin_table,'用户')
print "过滤后的网络密度："
calnetdensity(newuser_loc,len(newuserckins),len(newlocckins))
print "时间分布："
#统计及输出
for time_yearmon in timeDic:
    print str(time_yearmon)+":"+str(timeDic[time_yearmon])
allcount=0
for x in timeDic:
	allcount=allcount+timeDic[x]
print allcount
#显示统计结果
print 'rawlinecount:',rawlinecount
print 'newlinecount:',newlinecount
print 'float(checkinsize)',float(checkinsize)
print 'float(newcheckinsize)',float(newcheckinsize)

calnetdensity(rawuser_loc,len(userckins),len(locckins))


xlsfile.save(xlsfileName)



    
