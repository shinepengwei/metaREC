# -*- coding: cp936 -*-
#useful
###用于基于用户的协同过滤，将测试集分成十个数据集分别计算结果



path="E:\\checkin\\"
fname="gowalla_testdata2"
print path+fname+".txt"
f=open(path+fname+".txt",'r')
fout = {}
i=0

while i<11:
    fout[i] = open(path+fname+"_"+str(i)+".txt",'w')
    i = i+ 1

linecount = 0
i =0
while True:
    i = i +1
    if i%10000==0:
        print i
    linecount=linecount+1
    newline=f.readline()
    if newline=='':
        break;

print "linecount:"+str(linecount)
foutline = linecount / 10
i =0

f.seek(0)
while True:
    i=i+1
    if i%10000 ==0:
        print i
    newline=f.readline()
    if newline=='':
        break;
    index=int(i / foutline)
    fout[index].write(newline)

f.close()
i =0
while i<11:
    fout[i].close()
