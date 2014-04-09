# -*- coding: cp936 -*-

'''
将Foursquare的csv friends数据，转为gowalla的格式，便于一下统一分析
'''



f=open("J:\\checkin\\edges_all_directional.csv",'r')

fout=open("J:\\checkin\\edges_all_directional.txt",'w')


i=0
while True:
    i=i+1
    if i%10000 ==0:
        print i
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split(',')
    userid1 = arr[0]
    userid2 = arr[1]
    

    nline=str(userid1)+'\t'+str(userid2)
    fout.write(nline)


fout.close()
print "end"
