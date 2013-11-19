# -*- coding: cp936 -*-
#f=open("d:\\data\\testdata2.txt",'r')
#tf=open("d:\\data\\filter_testdata2.txt",'w')

f=open("d:\\data\\traindata2.txt",'r')
tf=open("d:\\data\\filter_testdata2.txt",'w')

lastUseId=-1

curCount=0
curString=""
i=0

#统计过滤掉的信息数量
allUserCount=0
remainUserCount=0
deleteUserCount=0

while True:
    i=i+1
    if i%1000==0:
        print i
    #解析user id
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split('\t');
    userId=arr[0]
    curCount=curCount+1
    #如果是一个新的用户
    if userId!=lastUseId:
        allUserCount=allUserCount+1
        if curCount>=20:
            tf.write(curString)
            remainUserCount=remainUserCount+1
        else:
            deleteUserCount=deleteUserCount+1
        curString=""
        curCount=0;
        lastUseId=userId
    curString=curString+newline
f.close()
tf.close()
print "allUserCount:",allUserCount,"  remainUserCount:",remainUserCount,"  deleteUserCount:",deleteUserCount
