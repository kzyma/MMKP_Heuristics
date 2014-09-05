##################################################################
#
# File: TlboRunner.py
# Author: Ken Zyma
#
# Py Version: 2.7
#
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Analysis the r-value for each MMKP problem.
#
##################################################################

#!/usr/bin/python
import os,sys
from xlwt import Workbook,easyxf
from mmap import mmap,ACCESS_READ
from xlrd import open_workbook
import time as sysTime
import math
from decimal import Decimal
import tempfile
import subprocess
import random

import settings as settings

#read problem data and return multi dim array of:
#[item][profit,C_1...C_j,AverageCost]
def readProblemData(folder,file,problemNumber):
    try:
        fd = open(folder+"/"+file)
    except:
        print "file "+(folder+"/"+file)+" failed to open."

    problemSetData = []
    fd.seek(0,0)
    if(folder == 'orlib_data'):
        problemSetData = []
        line = fd.readline()
        line = fd.readline()
        classes = int(line.split()[0])
        items = int(line.split()[1])
        constraints = int(line.split()[2])
        line = fd.readline()
        for i in range(classes):
            line = fd.readline()    #dummy line
            for j in range(items):
                temp = []
                data = fd.readline()
                temp.append(float(data.split()[0]))
                for k in range(1,(constraints+1)):
                    data = data.strip("\n")
                    temp.append(float(data.split()[k]))
                temp.append(float(sum(temp[2:])/items))
                problemSetData.append(temp)
    else:
        counter=0
        while(problemNumber != counter):
            problemSetData = []
            if(counter != 0):
                line = fd.readline()
            line = fd.readline()
            classes = int(line.split()[1])
            items = int(line.split()[2])
            constraints = int(line.split()[3])
            line = fd.readline()
            for i in range(classes):
                line = fd.readline()    #dummy line
                for j in range(items):
                    temp = []
                    data = fd.readline()
                    temp.append(float(data.split()[0]))
                    for k in range(1,(constraints+1)):
                        data = data.strip("\n")
                        temp.append(float(data.split()[k]))
                    temp.append(sum(temp[2:])/items)
                    problemSetData.append(temp)
            counter += 1

    return problemSetData

#return maximum number of constraints in all problems in fileGroup
def findMaxNumberOfConstraints(folder,fileGroup):
    maxConstraints = 0
    for fileTuple in fileGroup:
        for problemNumber in range(1,(fileTuple[1]+1)):
            try:
                fd = open(folder+"/"+fileTuple[0])
            except:
                print "file "+(folder+"/"+fileTuple[0])+" failed to open."
            
            problemSetData = []
            fd.seek(0,0)
            if(folder == 'orlib_data'):
                problemSetData = []
                line = fd.readline()
                line = fd.readline()
                classes = int(line.split()[0])
                items = int(line.split()[1])
                constraints = int(line.split()[2])
                if(constraints > maxConstraints):
                    maxConstraints = constraints
                line = fd.readline()
                for i in range(classes):
                    line = fd.readline()    #dummy line
                    for j in range(items):
                        temp = []
                        data = fd.readline()
                        temp.append(float(data.split()[0]))
                        for k in range(1,(constraints+1)):
                            data = data.strip("\n")
                            temp.append(float(data.split()[k]))
                        temp.append(float(sum(temp[2:])/items))
                        problemSetData.append(temp)
            else:
                counter=0
                while(problemNumber != counter):
                    problemSetData = []
                    if(counter != 0):
                        line = fd.readline()
                    line = fd.readline()
                    classes = int(line.split()[1])
                    items = int(line.split()[2])
                    constraints = int(line.split()[3])
                    if(constraints > maxConstraints):
                        maxConstraints = constraints
                    line = fd.readline()
                    for i in range(classes):
                        line = fd.readline()    #dummy line
                        for j in range(items):
                            temp = []
                            data = fd.readline()
                            temp.append(float(data.split()[0]))
                            for k in range(1,(constraints+1)):
                                data = data.strip("\n")
                                temp.append(float(data.split()[k]))
                            temp.append(sum(temp[2:])/items)
                            problemSetData.append(temp)
                    counter += 1
    return maxConstraints

#read problem data and return multi-dim array of:
#[problem][item][problem,profit,C_1...C_j,AverageCost]
def getFileGroupData(folder,fileGroup):
    fileGroupData = []
    for fileTuple in fileGroup:
        for problemNumber in range(1,(fileTuple[1]+1)):
            t = []
            if(folder == 'orlib_data'):
                t.append(fileTuple[0])
            else:
                t.append(fileTuple[0]+str(problemNumber))
            q = readProblemData(folder,fileTuple[0],problemNumber)
            t += calculatePearsonsCoef(q)
            fileGroupData.append(t)
    return fileGroupData

#input profit and list of constraints (including average as last)
#return list of pearsons coeffient, last element is average
def calculatePearsonsCoef(data):
    temp = []
    for i in range(len(data[0])-1):
        temp.append([0,0,0,0,0])

    for row in range(len(data)):
        for colI in range(1,len(data[row])):
            x = data[row][0]
            y = data[row][colI]
            temp[colI-1][0] += x       #E_x
            temp[colI-1][1] += y       #E_y
            temp[colI-1][2] += x*y     #E_xy
            temp[colI-1][3] += x*x     #E_xx
            temp[colI-1][4] += y*y     #E_yy

    r = []
    for x in range(len(temp)):
        l1 = ((len(data)*temp[x][2])-(temp[x][0]*temp[x][1]))
        l2 = (math.sqrt(((len(data)*temp[x][3])-(temp[x][0]*temp[x][0]))*
                        (len(data)*temp[x][4]-(temp[x][1]*temp[x][1]))))
        rVal = l1 / l2
        r.append(rVal)

    return r

if(__name__=='__main__'):
    num_argv = len(sys.argv)
    
    if num_argv > 1:
        print 'usage: filename <settings>'
        exit(0)
    
    data_file = Workbook()
    data_sheets = []

    for folderIndex in range(len(settings.FILES)):
        for fileGroup in settings.FILES[folderIndex]:
            
            print "starting "+(fileGroup[0])[0]+" ..."

            #naming convention differs for each set
            if(settings.FOLDERS[folderIndex] == 'orlib_data'):
                groupName = 'orlib'
            elif(settings.FOLDERS[folderIndex] == 'HiremathHill_data'):
                groupName = (fileGroup[0])[0].strip('.txt')
            else:
                groupName = 'default'

            sheetName = str(groupName)
            data_sheets.append(data_file.add_sheet(sheetName,cell_overwrite_ok=True))
            currentSheet = data_sheets[-1]
            currentSheet.write(0,0,groupName)
        
            startRow = 2
            startCol = 1

            maxNumberOfConstraintsByGroup \
                = findMaxNumberOfConstraints(settings.FOLDERS[folderIndex],fileGroup)
            s_header = ['Problem']
            for i in range(maxNumberOfConstraintsByGroup):
                s_header.append(str("C_"+str(i)))
            s_header.append("E_C/n")
            
            for i in range(len(s_header)):
                currentSheet.write(startRow,startCol+i,s_header[i])
        
            fileGroupData = getFileGroupData(settings.FOLDERS[folderIndex],fileGroup)

            for i in range(len(fileGroupData)):
                currentSheet.write(startRow+1+i,startCol,fileGroupData[i][0])
            
            for i in range(len(fileGroupData)):
                for j in range(1,len(fileGroupData[i])-1):
                    currentSheet.write(startRow+1+i,startCol+j,
                                       float(Decimal("{0:.3}".format(fileGroupData[i][j]))))
                        
            for i in range(len(fileGroupData)):
                currentSheet.write(startRow+1+i,startCol+1+maxNumberOfConstraintsByGroup,
                                   float(Decimal("{0:.3}".format((fileGroupData[i][-1])))))

            print "finished."
            data_file.save('results/ProblemAnalysis.xls')
    data_file.save('results/ProblemAnalysis.xls')

