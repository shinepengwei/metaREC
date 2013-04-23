# -*- coding: cp936 -*-
f=open("d:\\data\\filter_testfeaturedata.txt",'r')
ft=open("d:\\data\\half_testfeaturedata.txt",'w+')
while True:
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split(',');
    userId=int(arr[0])
    if userId!=int(0):
        break
    strs=str(arr[2]+","+arr[3]+","+arr[4]+","+arr[5]+","+arr[6]+","+arr[7])
    ft.write(strs)
f.close()
ft.close()
    
    
