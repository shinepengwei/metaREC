f=open("E:\\checkin\\filter_foursquare_allCheckin.txt",'r')
i=0

while True:
    i=i+1
    if i%10000000 ==0:
        print i
    newline=f.readline()

print i
            
