# -*- coding: cp936 -*-
file1=open("d:\\data\\trainfeaturedata.txt",'r')
file2=open("d:\\data\\trainfeaturedata2.txt",'w+')
poCount=0#正例数量
neCount=0#负例数量
while True:
    newline=file1.readline()
    if newline=='':
        break;
    arr=newline.split(',')
    flag=int(arr[7])
    if flag==1:
        if arr[2]!='0' or arr[3]!='0' or arr[4]!='0' or arr[5]!='0' or arr[6]!='0':
            poCount+=1;
    else:
        neCount=0;
