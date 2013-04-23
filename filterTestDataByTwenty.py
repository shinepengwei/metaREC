f=open("d:\\data\\testfeaturedata.txt",'r')
tf=open("d:\\data\\filter_testfeaturedata.txt",'w')
lastUseId=-1
lastLabel=0
postiveCount=0
postiveString=""
i=0
userCount=0
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
                tf.write(postiveString)
                userCount=userCount+1
            tf.write(newline)
            lastLabel=0
        else:
            continue
f.close()
tf.close()
            
