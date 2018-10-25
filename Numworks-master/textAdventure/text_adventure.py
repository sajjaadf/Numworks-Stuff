import random
import math
currentMap = []
currentMapPlace = 'null'
def map (place, placeOld):
  if placeOld != place:
    if place == 'town':
      currentMap.clear()
      currentMapPlace = 'town'
      currentMap.append('')
      currentMap.append('============== ==============')
      currentMap.append('=    / \\                    =')
      currentMap.append('=   /_W_\\                   =')
      currentMap.append('=   |___|          / \\      =')
      currentMap.append('=                 /_S_\\     =')
      currentMap.append('                  |___|      ')
      currentMap.append('=   / \\              ___    =')
      currentMap.append('=  /_M_\\            | P |   =')
      currentMap.append('=  |___|            |___|   =')
      currentMap.append('=                           =')
      currentMap.append('============== ==============')
    if place == 'dungeon':
      currentMap.clear()
      currentMapPlace = 'dungeon'
      currentMap.append('')
      currentMap.append('============== ==============')
      currentMap.append('=                           =')
      currentMap.append('=    R                      =')
      currentMap.append('=                R          =')
      currentMap.append('=                           =')
      currentMap.append('                             ')
      currentMap.append('=        R                  =')
      currentMap.append('=                R          =')
      currentMap.append('=    R                      =')
      currentMap.append('=                           =')
      currentMap.append('============== ==============')
    if place == 'blank':
      currentMap.clear()
      currentMapPlace = 'blank'
      currentMap.append('')
      currentMap.append('============== ==============')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('                             ')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('=                           =')
      currentMap.append('============== ==============')
  for i in range(12):
    print(currentMap[i])

def moveTo (d):
    oldX = x
  oldY = y
  lineOutput = x
  #this is the numpad directions
  if (d == 8):
    y = y + 1
  if (d == 2):
    y = y - 1
  if (d == 4):
    x = x + 1
  if (d == 5):
    x = x - 1
  print("x: " + x + " y: " + y)
  #I'm using strings, so this converts to and from lists
  #If the x doesn't change, then no need to replace old string
  if (oldY != y):
    oldYString = currentMap[oldY]
    lineOutput = lineOutput + 1
  tempString = currentMap[y]
  tempMap = list(tempString)
  oldChar = tempMap[x]
  tempMap[x] = '@'
  currentMap[x] = ''.join(tempMap)
  currentMap[oldY] = oldYString[y]
  for j in range(12):
    print(currentMap[j])

def select ()

map('town','null')
y = 6
x = 14
cInput = input()
while (cInput != '**'):
    cInput = input()
    if (cInput == 8 or 6 or 4 or 2):
        moveTo(cInput)
    else if (cInput == 5):
