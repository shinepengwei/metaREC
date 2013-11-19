# -*- coding: cp936 -*-
f=open("d:\\data\\second\\log_testfeaturedata.txt",'r')
tf=open("d:\\data\\second\\filter_log_testfeaturedata.txt",'w')
lastUseId=-1
lastLabel=0
postiveCount=0
postiveString=""
i=0
userCount=0
#统计过滤掉的信息数量
allUserCount=0
remainUserCount=0
deleteUserCount=0
remainPosCount=0
deletePosCount=0
remainPosCount=0
deleteNegCount=0

while True:
    i=i+1
    if i%1000==0:
        print i
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split(',');
    userId=arr[0]
    label=int(arr[7])
    if userId!=lastUseId:
        allUserCount=allUserCount+1
        postiveCount=0
        postiveString=""
    lastUseId=userId
    if 1==label:
        postiveCount=postiveCount+1
        postiveString=postiveString+newline
        lastLabel=1
    if 0==label :
        if postiveCount>=20:
            if 1==lastLabel:
                print postiveString
                tf.write(postiveString)
                userCount=userCount+1
            tf.write(newline)
            lastLabel=0
        else:
            continue
f.close()
tf.close()
            
