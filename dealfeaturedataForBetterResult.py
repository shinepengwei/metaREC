f=open("E:\\checkin\\result\\friendREC_foursquare_all_featuredata_binary_test_4hours.txt",'r')
fout=open("E:\\checkin\\result\\friendREC_foursquare_all_featuredata_binary_test_4hours_out.txt",'w+')
i=0
stop = False
while stop !=True:
    i=i+1
    if i%100000 ==0:
        print i
    newline=f.readline()
    arr=newline.split(',');
    userId=int(arr[0])
    if userId ==61782731:
        stop = True
        print "out"
    else:
        fout.write(newline)
            
