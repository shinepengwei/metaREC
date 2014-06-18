f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_12hours.txt",'r')
fout=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_12hours_out.txt",'w+')
i=0
begin = False
while True:
    i=i+1
    if i%10000000 ==0:
        print i
    newline=f.readline()
    arr=newline.split(',');
    userId=int(arr[0])
    if userId ==31613218 and begin == False:
        begin = True
        print "out"
    if begin:
        fout.write(newline)
            
