# -*- coding: cp936 -*-
f=open("E:\\checkin\\result\\gowalla_featuredata_log_test_4hours.txt",'r')
ft=open("E:\\checkin\\result\\gowalla_featuredata_log_test_4hours_new.txt",'w+')
while True:
    newline=f.readline()
    if ""==newline:
        break;
    arr=newline.split(',');
    i=2
    while i<8:
        if arr[i] == '1.#INF':
            arr[i]=0
        i = i +1
    strs=str(arr[0]+','+arr[1]+','+str(arr[2])+","+str(arr[3])+","+str(arr[4])+","+str(arr[5])+","+str(arr[6])+","+str(arr[7])+","+arr[8])
    ft.write(strs)
f.close()
ft.close()
print "end"
    
    
