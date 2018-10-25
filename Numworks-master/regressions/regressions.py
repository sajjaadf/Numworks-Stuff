import math
def linTable (inputList,outputList) :
    outputList.append(inputList[:len(inputList)//2])
    if len(inputList)% 2 == 0 :
        outputList.append(inputList[len(inputList)//2:])
    else :
        outputList.append(inputList[(len(inputList)//2)+1:])
print("============")
print("Enter X list")
xList = [float(x) for x in input(":").split(",")]
print("Enter Y list")
yList = [float(x) for x in input(":").split(",")]
print("============")
xList1 = []
yList1 = []
linTable(xList,xList1)
linTable(yList,yList1)
xAvgLow = sum(xList1[0])/float(len(xList1[0]))
xAvgHigh = sum(xList1[1])/float(len(xList1[1]))
yAvgLow = sum(yList1[0])/float(len(yList1[0]))
yAvgHigh = sum(yList1[1])/float(len(yList1[1]))
slope = float((yAvgHigh-yAvgLow)/(xAvgHigh-xAvgLow))
b = yAvgLow - (xAvgLow * slope)
print("Linear:")
print("y =",slope,"x +",b)
print("============")
if len(yList)% 2 == 0 :
    tMiddle = int(len(yList) / 2)
    yMiddle = float((yList[tMiddle]) + .5)
else :
    tMiddle = int(((len(yList) + 1) / 2)-1)
    yMiddle = float(yList[tMiddle])
if len(xList)% 2 == 0 :
    tMiddle = int(len(xList) / 2)
    xMiddle = float((xList[tMiddle]) + .5)
else :
    tMiddle = int(((len(xList) + 1) / 2)-1)
    xMiddle = float(xList[tMiddle])
equ1 = [yAvgLow,(xAvgLow**2),xAvgLow]
equ2 = [-yMiddle,-(xMiddle**2),-xMiddle]
equ3 = [yAvgHigh,(xAvgHigh**2),xAvgHigh]
equ1 = [sum(x) for x in zip(equ1, equ2)]
equ3 = [sum(x) for x in zip(equ2, equ3)]
equ3 = [i*((-equ1[2])/equ3[2]) for i in equ3]
equ2 = equ1
equ1 = [sum(x) for x in zip(equ1, equ3)]
a = equ1[1]/equ1[0]
b = (equ2[0]-(equ2[1] * a))/equ2[2]
c = yAvgLow-((a*(xAvgLow**2))+(b*xAvgLow))
print("Quadratic:")
print("y =",a,"x^2 +",b,"x +",c)
