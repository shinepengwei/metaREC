class Test:
    def __init__(self,num):
        self.num=num
    def getNum(self):
        return self.num
    def setNum(self,num):
        self.num=num
def printlist(rlist):
    i=0
    print "Len:",len(rlist)
    while i<len(rlist):
        print "[",i,"]:",rlist[i].getNum()
        i=i+1

i=0
listTe=[]
while i<10:
    listTe.append(Test(i))
    i=i+1
print"listTe"
printlist(listTe)
listSe=[]
listSe.append(listTe[1])
print"listSe"
printlist(listSe)
listSe[0].setNum(10)
print"listSe"
printlist(listSe)
print"listTe"
printlist(listTe)
