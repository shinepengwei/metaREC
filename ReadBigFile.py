f=open("E:\\checkin\\result\\foursquare_NewYork_featuredata_binary_test_2hours.txt",'r')
i=0
while True:
    i=i+1
    if i%20 ==0:
        name = input("-----\n")
    newline=f.readline()
    print newline




