f=open("J:\\checkin\\allCheckinNewYork.csv",'r')
i=0
while True:
    i=i+1
    if i%10 ==0:
        name = input("-----\n")
    newline=f.readline()
    print newline




