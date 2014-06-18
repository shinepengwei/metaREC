f=open("E:\\checkin\\result\\friendREC_foursquare_all_featuredata_binary_testWithWindowTime_4hours.txt",'r')
fout=open("E:\\checkin\\result\\friendREC_foursquare_all_featuredata_binary_testWithWindowTime_4hours_out.txt",'w+')
i=0
stop = False
while stop !=True:
    i=i+1
    if i%10000000 ==0:
        print i
    newline=f.readline()
    arr=newline.split(',');
    userId=int(arr[0])
    if userId ==403566:
        stop = True
        print "out"
    else:
        fout.write(newline)
f.close()
fout.close()
