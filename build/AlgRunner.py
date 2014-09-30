##################################################################
#
# File: AlgRunner.py
# Author: Ken Zyma
#
# Py Version: 2.7
#
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Heuristic runner.
#
# Note: Due to a serious security risk this module is only to be
# used by trusted client at all times. For more information
# see python documentation:
# https://docs.python.org/2/library/subprocess.html#frequently-
# used-arguments
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


import TlboSettings
import CoaSettings
import GaSettings
import BbaSettings
import AcoSettings
import GeneralSettings as settings


#format and keys of data stored in problemSetData
outKeys = ['file','number','initProfit','profit','runtime','convCount','convData',
           'numClasses']

def runAlg(folderIndex,problem,problemNumber,algExecStr,alg,paramsStr):
    #make sure file is empty to dump Tlbo out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()

    if(algExecStr == str('./HeuristicApp')):
        for popSize in settings.POP_SIZE:
            for genSize in settings.GEN_SIZE:
                runString = algExecStr+' '+settings.FOLDERS[folderIndex]+\
                    ' '+problem+' '+str(problemNumber)+' '+alg+' '+settings.MODIFIER+' '+\
                        str(popSize)+' '+str(genSize)+' '+\
                    paramsStr+\
                    ' >> tempContentDump.txt'
                print 'Running: '+runString
                #*NOTE* using shell=True incures a serious security risk, this
                #module MUST NOT be used with an untrust
                subprocess.call(runString,shell=True)

    else:
        for popSize in settings.POP_SIZE:
            for genSize in settings.GEN_SIZE:
                runString = algExecStr+' '+settings.FOLDERS[folderIndex]+\
                    ' '+problem+' '+str(problemNumber)+' '+settings.MODIFIER+' '+\
                        str(popSize)+' '+str(genSize)+' '+\
                    paramsStr+\
                    ' >> tempContentDump.txt'
    
                print 'Running: '+runString
                #*NOTE* using shell=True incures a serious security risk, this
                #module MUST NOT be used with an untrust
                subprocess.call(runString,shell=True)

def readData(numberOfRuns, fileName, alg):
    
    fd = open(fileName)
    problemSetData = []
    convergeData = []
    
    for j in range(numberOfRuns):
        tempData = {}
    
        for k in outKeys:
            dummy = fd.readline()
            tempData[k] = fd.readline().strip('\n')
            if(k == 'numClasses'):
                for l in range(int(tempData['numClasses'])):
                    dummy = fd.readline()
            if(k == 'convData'):
                for l in range(int(tempData['convData'])):
                    temp = []
                    temp.append(int(fd.readline().strip('\n')))
                    temp.append(float(fd.readline().strip('\n')))
                    convergeData.append(temp)

        dummy = fd.readline()
        problemSetData.append(tempData)
    
    fd.close()
    return float(problemSetData[0]['profit']),float(problemSetData[0]['runtime']),\
            int(problemSetData[0]['convCount']),convergeData

#read exact results for given problem from first column of compare file.
#Element one is the name of result, in this case "Exact".
def readExactResults(dataSet):
    data = []
    wb = open_workbook(('compareData/'+dataSet+'.xls'))
    sheet = wb.sheet_by_index(0)
    data.append("Exact")
    for row in range(1,sheet.nrows):
        data.append(float(sheet.cell(row,0).value))
    return data

#get reaults for all after exact result in compare files. Element one
#of each column is the name of the Algorithm.
def readOtherResults(dataSet):
    data = []
    wb = open_workbook(('compareData/'+dataSet+'.xls'))
    sheet = wb.sheet_by_index(0)

    for col in range(1,sheet.ncols):
        tempData = []
        tempData.append(sheet.cell(0,col).value)
        for row in range(1,sheet.nrows):
            tempData.append(float(sheet.cell(row,col).value))
        data.append(tempData)
    return data

#print row of data
def printResults(dataSet,sheet,row,startColumn):
    for i in range(len(dataSet)):
        if(dataSet[i] != "NA"):
            sheet.write(row,startColumn+i,
                float((Decimal("{0:.3}".format(dataSet[i])))))

def run(alg):

    data_file = Workbook()
    graph_file = Workbook()
    graph_sheets = []
    data_sheets = []
    problemNames = []
    problemDeviations = []
    problemRuntimes = []
    convAll = []
    genAll = []

    print 'Starting test runner'

    #for each 'file group', print results on one sheet of workbook
    for folderIndex in range(len(settings.FILES)):
        for fileGroup in settings.FILES[folderIndex]:
            
            #naming convention differs for each set
            if(settings.FOLDERS[folderIndex] == 'orlib_data'):
                groupName = 'orlib'
            elif(settings.FOLDERS[folderIndex] == 'HiremathHill_data'):
                groupName = (fileGroup[0])[0].strip('.txt')
            else:
                groupName = 'default'
            
            #read results for comparing and compute standard dev. from exact
            exact_results = readExactResults(groupName)
            algs_results = readOtherResults(groupName)
            algs_deviations = []
            problemCounter = 0
            
            for algs in algs_results:
                tempData = []
                tempData.append(algs[0])
                tempData += [float((abs((algs[x]-exact_results[x]))/exact_results[x])*100) \
                            if (algs[x] != -1) else "NA" for x in range(1,(len(algs)))]
                algs_deviations.append(tempData)
            
            sheetName = str(groupName)
            data_sheets.append(data_file.add_sheet(sheetName,cell_overwrite_ok=True))
            graph_sheets.append(graph_file.add_sheet(sheetName,cell_overwrite_ok=True))
            currentSheet = data_sheets[-1]
            currentGraph = graph_sheets[-1]
            currentSheet.write(0,0,groupName)
            currentGraph.write(0,0,groupName)
            problemNames.append(groupName)
            
            s_header = ['Problem']
            s_header += [str(algs_deviations[x][0]) for x in range(0,len(algs_deviations))]
            hRow = 2    #staring row
            hCol = 1    #starting column
            hRef=currentSheet.row(hRow)
            TlboStartRow = hRow + 1
            TlboStartCol = hCol + len(s_header)
            for i in range(len(s_header)):
                hRef.write((i+hCol),s_header[i])
            #write current algorithm header
            currentSheet.write(hRow,TlboStartCol,str(alg.upper()))
            #write runtime
            currentSheet.write(hRow,TlboStartCol+1,"Runtime(sec)")
            currentSheet.write(hRow,TlboStartCol+2,"Generations")
            averageDeviations = []
            TlboDeviations = []
            algRuntimes = []
            totalRuntimes = []
            convAverage = []
            generationAve = []
        
            for fileTuple in fileGroup:
                for problemNumber in range(1,(fileTuple[1]+1)):
                    #run Tlbo and get profit
                    print 'Problem:'+str(fileTuple[0])+'- '+str(problemNumber)+' executing...'
                
                    execStr = './HeuristicApp'
                    paramStr = ''
                    
                    if alg == 'tlbo':
                        pass
                    elif alg == 'coa':
                        paramStr = str(CoaSettings.V_PROB)+' '+str(CoaSettings.H_PROB)
                    elif alg == 'ga':
                        paramStr = str(GaSettings.C_PROB)+' '+str(GaSettings.M_PROB)
                    elif alg == 'bba':
                        pass
                    elif alg == 'aco':
                        paramStr = str(AcoSettings.B)+' '+str(AcoSettings.p)+' '+str(AcoSettings.e)
                    elif alg == 'mmhph':
                        execStr = './MmhphApp'
                    elif alg == 'abc':
                        pass
                    else:
                        print 'error: algorithm unavaliable'
                        exit(0)

                    runAlg(folderIndex,fileTuple[0],problemNumber,execStr,alg,paramStr)

                    BestObjFunc,runTime,\
                    ConvIter,ConvData = readData(1,'tempContentDump.txt',alg)

                    print 'Profit:: '+str(BestObjFunc)
                    #write problem name
                    if(groupName == 'orlib'):
                        currentSheet.write((hRow+1)+problemCounter, hCol,str(fileTuple[0]))
                        currentGraph.write((1)+(problemCounter*5), 1,str(fileTuple[0]))
                    else:
                        currentSheet.write((hRow+1)+problemCounter, hCol,str(problemNumber))
                        currentGraph.write((1)+(problemCounter*5), 1,str(fileTuple[0]))
                    
                    #write results for other algs
                    otherAlgs = [algs_deviations[x][problemCounter+1] \
                                for x in range(len(algs_deviations))]
                    
                    printResults(otherAlgs,currentSheet,
                                 TlboStartRow+problemCounter,(hCol+1))
                    #write results for alg
                    TlboDev = (abs((BestObjFunc-exact_results[problemCounter+1]))/
                               exact_results[problemCounter+1])*100
                    TlboDeviations.append(TlboDev)
                    algRuntimes.append(runTime)
                    
                    currentSheet.write(TlboStartRow+problemCounter,TlboStartCol,
                                       float((Decimal("{0:.3}".format(TlboDev)))))
                    currentSheet.write(TlboStartRow+problemCounter,TlboStartCol+1,
                                       float((Decimal("{0:.3}".format(runTime)))))
                    currentSheet.write(TlboStartRow+problemCounter,TlboStartCol+2,
                                       ConvIter)
                                       
                    generationAve.append(ConvIter)
                                       
                    #write convergence data to graph file
                    convDevData = []
                    currentGraph.write((2)+(problemCounter*5), 1,"Gen best found:")
                    currentGraph.write((2)+(problemCounter*5), 2,str(ConvIter))
                    currentGraph.write((3)+(problemCounter*5), 1,"Func Eval:")
                    currentGraph.write((4)+(problemCounter*5), 1,"Best Sol Dev:")
                    for i in range(len(ConvData)):
                        g_dev = (abs((ConvData[i][1]-exact_results[problemCounter+1]))/
                                   exact_results[problemCounter+1])*100
                        currentGraph.write((3)+(problemCounter*5), 2+i,ConvData[i][0])
                        temp = float(Decimal("{0:.3}".format(g_dev)))
                        convDevData.append(temp)
                        currentGraph.write((4)+(problemCounter*5), 2+i,temp)
                    
                    if not convAverage:
                        for i in range(len(convDevData)):
                            convAverage.append(convDevData[i])
                    else:
                        for i in range(len(convDevData)):
                            temp = (convAverage[i] + convDevData[i]) / 2
                            convAverage[i] = temp
                
                    problemCounter += 1
                    
                    print 'Problem complete.'
                    data_file.save('results/'+alg+'_OverviewResults.xls')
                    graph_file.save('results/graphs/'+alg+'_ConvResults.xls')
    
            #calculate deviations
            for i in range(len(algs_deviations)):
                counter = 0
                temp = 0
                for j in range(1,len(algs_deviations[i])):
                    if(algs_deviations[i][j]!="NA"):
                        temp += algs_deviations[i][j]
                        counter += 1
                if(counter != 0):
                            averageDeviations.append((temp/counter))
                else:
                            averageDeviations.append("NA")

            temp = 0
            counter = 0
            for i in TlboDeviations:
                if(i != "NA"):
                    counter += 1
                    temp += i
            averageDeviations.append((temp/counter))
            problemDeviations.append(averageDeviations)
            
            genA = 0.0
            for i in range(len(generationAve)):
                genA += generationAve[i]
            genA = genA / len(generationAve)
            
            genAll.append(genA)

            #write total deviations and runtime
            for i in range(len(averageDeviations)):
                if(averageDeviations[i] != "NA"):
                    currentSheet.write((TlboStartRow+fileTuple[1]*len(fileGroup)),
                    ((hCol+1)+i),float((Decimal("{0:.3}".format(averageDeviations[i])))))
                else:
                    currentSheet.write((TlboStartRow+fileTuple[1]*len(fileGroup)),
                                       ((hCol+1)+i),averageDeviations[i])
            total = 0
            for i in algRuntimes:
                total += i
            currentSheet.write((TlboStartRow+fileTuple[1]*len(fileGroup)),
                   (TlboStartCol+1),float((Decimal("{0:.3}".format(total)))))
            currentSheet.write((TlboStartRow+fileTuple[1]*len(fileGroup)),
                               (TlboStartCol+2),float((Decimal("{0:.3}".format(genA)))))
            problemRuntimes.append(total)
            data_file.save('results/'+alg+'_OverviewResults.xls')
            
            currentGraph.write((2)+(problemCounter*5), 1,"Gen:")
            currentGraph.write((3)+(problemCounter*5), 1,"Best Sol Dev:")

            for i in range(len(convAverage)):
                currentGraph.write((2)+(problemCounter*5),2+i,i)
                currentGraph.write((3)+(problemCounter*5),
                                   2+i,float(Decimal("{0:.3}".format(convAverage[i]))))
            convAll.append(convAverage)
            graph_file.save('results/graphs/'+alg+'_ConvResults.xls')

    #summary of results page
    data_sheets.append(data_file.add_sheet("Results",cell_overwrite_ok=True))
    graph_sheets.append(graph_file.add_sheet("Results",cell_overwrite_ok=True))
    currentSheet = data_sheets[-1]
    currentSheet.write(0,0,"Overview Of Results")
    currentGraph = graph_sheets[-1]
    
    #write header
    s_header = ['Problem']
    s_header += [str(algs_deviations[x][0]) for x in range(0,len(algs_deviations))]
    hRow = 2    #staring row
    hCol = 1    #starting column
    hRef=currentSheet.row(hRow)
    TlboStartRow = hRow + 1
    TlboStartCol = hCol + len(s_header)
    for i in range(len(s_header)):
        hRef.write((i+hCol),s_header[i])
    #write Tlbo header
    currentSheet.write(hRow,TlboStartCol,alg.upper())
    currentSheet.write(hRow,TlboStartCol+1,'Runtime(sec)')
    currentSheet.write(hRow,TlboStartCol+2,'Generations')

    averageAllDev = []
    #compute avarege Dev
    for i in range(len(problemDeviations[0])):
        tempSum = 0
        counter = 0
        for j in range(len(problemDeviations)):
            if(problemDeviations[j][i] != "NA"):
                tempSum += problemDeviations[j][i]
                counter += 1
        averageAllDev.append(tempSum/counter)
        
    #print filenames and deviations.
    allTemp = 0.0
    for i in range(len(genAll)):
        allTemp += genAll[i]
    allTemp = allTemp / len(genAll)

    for i in range(len(problemDeviations)):
        currentSheet.write(TlboStartRow+i,hCol,problemNames[i])
        for j in range(len(problemDeviations[i])):
            if(problemDeviations[i][j] == "NA"):
                currentSheet.write(TlboStartRow+i,(hCol+1)+j,"")
            else:
                currentSheet.write(TlboStartRow+i,(hCol+1)+j,
                               Decimal("{0:.3}".format(problemDeviations[i][j])))
        currentSheet.write(TlboStartRow+i,TlboStartCol+2,
                           Decimal("{0:.3}".format(genAll[i])))

    total = 0
    for i in range(len(problemRuntimes)):
        total += problemRuntimes[i]
        currentSheet.write(TlboStartRow+i,TlboStartCol+1,
                           Decimal("{0:.3}".format(problemRuntimes[i])))

    #print average of all deviations and runtimes
    for i in range(len(averageAllDev)):
        if(averageAllDev[i] == "NA"):
            currentSheet.write((TlboStartRow+len(problemDeviations)),(hCol+1)+i,"")
        else:
            currentSheet.write((TlboStartRow+len(problemDeviations)),(hCol+1)+i,
                               Decimal("{0:.3}".format(averageAllDev[i])))

                           
    currentSheet.write(TlboStartRow+len(problemRuntimes),TlboStartCol+1,
                      Decimal("{0:.3}".format(total)))
    currentSheet.write(TlboStartRow+len(problemRuntimes),TlboStartCol+2,
                        Decimal("{0:.3}".format(allTemp)))

    dAll = []
    for i in range(len(convAll)):
        if not dAll:
            dAll = convAll[i]
        for j in range(len(convAll[i])):
            temp = (dAll[j] + convAll[i][j]) / 2
            dAll[j] = temp
        currentGraph.write(TlboStartRow+i,hCol,problemNames[i])
        for j in range(len(convAll[i])):
            currentGraph.write(TlboStartRow+i,hCol+1+j,
                               Decimal("{0:.3}".format(convAll[i][j])))

    for i in range(len(dAll)):
        currentGraph.write(TlboStartRow+1+len(convAll),hCol+1+i,Decimal("{0:.3}".format(dAll[i])))
    
    data_file.save('results/'+alg+'_OverviewResults.xls')
    graph_file.save('results/graphs/'+alg+'_ConvResults.xls')

if(__name__=='__main__'):
    num_argv = len(sys.argv)
    
    if (num_argv == 2):
        alg = str(sys.argv[1])
        run(alg)
    else:
        run('tlbo')
        run('mmhph')
        run('abc')


