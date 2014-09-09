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
# Run Tlbo.
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
outKeys = ['file','number','initProfit','profit','runtime','numClasses']

#run TlboApp with all mods/popsize/gensize for a given problem.
def runTlbo(folderIndex,problem,problemNumber):
    #make sure file is empty to dump Tlbo out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()
    
    for classSize in TlboSettings.CLASSROOM_SIZE:
        for popSize in TlboSettings.POP_SIZE:
            for genSize in TlboSettings.GEN_SIZE:
                for modifier in TlboSettings.MODS:
                    for i in range(settings.REPEAT):
                        #randomness is based on time, so
                        #need to make sure time changes between
                        #runs.
                        
                        if(settings.REPEAT > 1):
                            sysTime.sleep(1)
                        if(settings.SEED == False):
                            runString = './TlboApp '+settings.FOLDERS[folderIndex]+\
                                ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                                    str(popSize)+' '+str(genSize)+' '+str(classSize)+\
                                ' >> tempContentDump.txt'
                        else:
                            runString = './TlboApp '+settings.FOLDERS[folderIndex]+\
                                ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                                    str(popSize)+' '+str(genSize)+' '+str(classSize)+\
                                ' ' +str(1432)+\
                                ' >> tempContentDump.txt'
                        
                        print 'Running: '+runString
                        #*NOTE* using shell=True incures a serious security risk, this
                        #module MUST NOT be used with an untrust
                        subprocess.call(runString,shell=True)

#run CoaApp with all mods/popsize/gensize for a given problem.
def runGa(folderIndex,problem,problemNumber):
    #make sure file is empty to dump Coa out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()
    
    for popSize in GaSettings.POP_SIZE:
        for genSize in GaSettings.GEN_SIZE:
            for modifier in GaSettings.MODS:
                for i in range(settings.REPEAT):
                    #randomness is based on time, so
                    #need to make sure time changes between
                    #runs.
                    if(settings.REPEAT > 1):
                        sysTime.sleep(1)
            
                    if(settings.SEED == False):
                        runString = './GaApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            str(GaSettings.C_PROB)+' '+str(GaSettings.M_PROB)+\
                            ' >> tempContentDump.txt'
                    else:
                        runString = './GAApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            str(GaSettings.C_PROB)+' '+str(GaSettings.M_PROB)+\
                            ' ' +str(1432)+' '+\
                            ' >> tempContentDump.txt'
                    print 'Running: '+runString
                    #*NOTE* using shell=True incures a serious security risk, this
                    #module MUST NOT be used with an untrust
                    subprocess.call(runString,shell=True)

def runCoa(folderIndex,problem,problemNumber):
    #make sure file is empty to dump Coa out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()
    
    for popSize in CoaSettings.POP_SIZE:
        for genSize in CoaSettings.GEN_SIZE:
            for modifier in CoaSettings.MODS:
                for i in range(settings.REPEAT):
                    #randomness is based on time, so
                    #need to make sure time changes between
                    #runs.
                    if(settings.REPEAT > 1):
                        sysTime.sleep(1)
                    
                    if(settings.SEED == False):
                        runString = './CoaApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+str(CoaSettings.V_PROB)+' '\
                            +str(CoaSettings.H_PROB)+\
                            ' >> tempContentDump.txt'
                    else:
                        runString = './CoaApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+str(CoaSettings.V_PROB)+' '\
                            +str(CoaSettings.H_PROB)+\
                            ' '+str(1432)+\
                            ' >> tempContentDump.txt'
                    print 'Running: '+runString
                    #*NOTE* using shell=True incures a serious security risk, this
                    #module MUST NOT be used with an untrust
                    subprocess.call(runString,shell=True)

def runBba(folderIndex,problem,problemNumber):
    #make sure file is empty to dump Coa out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()
    
    for popSize in BbaSettings.POP_SIZE:
        for genSize in BbaSettings.GEN_SIZE:
            for modifier in BbaSettings.MODS:
                for i in range(settings.REPEAT):
                    #randomness is based on time, so
                    #need to make sure time changes between
                    #runs.
                    if(settings.REPEAT > 1):
                        sysTime.sleep(1)
                    
                    if(settings.SEED == False):
                        runString = './BbaApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            ' >> tempContentDump.txt'
                    else:
                        runString = './BbaApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            ' ' +str(1432)+\
                            ' >> tempContentDump.txt'
                    print 'Running: '+runString
                    #*NOTE* using shell=True incures a serious security risk, this
                    #module MUST NOT be used with an untrust
                    subprocess.call(runString,shell=True)

#run CoaApp with all mods/popsize/gensize for a given problem.
def runAco(folderIndex,problem,problemNumber):
    #make sure file is empty to dump Coa out to
    try:
        os.remove('tempContentDump.txt')
    except OSError:
        pass
    (open('tempContentDump.txt','w+')).close()
    
    for popSize in CoaSettings.POP_SIZE:
        for genSize in CoaSettings.GEN_SIZE:
            for modifier in CoaSettings.MODS:
                for i in range(settings.REPEAT):
                    #randomness is based on time, so
                    #need to make sure time changes between
                    #runs.
                    if(settings.REPEAT > 1):
                        sysTime.sleep(1)
                    
                    if(settings.SEED == False):
                        runString = './AcoApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            ' >> tempContentDump.txt'
                    else:
                        runString = './AcoApp '+settings.FOLDERS[folderIndex]+\
                            ' '+problem+' '+str(problemNumber)+' '+modifier+' '+\
                            str(popSize)+' '+str(genSize)+' '+\
                            ' ' +str(1432)+' >> tempContentDump.txt'
                        
                    print 'Running: '+runString
                    #*NOTE* using shell=True incures a serious security risk, this
                    #module MUST NOT be used with an untrust
                    subprocess.call(runString,shell=True)


#read data from fileName. param numberOfRuns is the number
#of problems to read from this file.
def readData(numberOfRuns, fileName, alg):
    
    fd = open(fileName)
    problemSetData = []
    
    for j in range(numberOfRuns):
        tempData = {}
    
        for k in outKeys:
            dummy = fd.readline()
            tempData[k] = fd.readline().strip('\n')
        for k in range(int(tempData['numClasses'])):
            dummy = fd.readline()

        dummy = fd.readline()
        problemSetData.append(tempData)
    
    fd.close()
    return float(problemSetData[0]['profit']),float(problemSetData[0]['runtime'])

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
    data_sheets = []
    problemNames = []
    problemDeviations = []
    problemRuntimes = []

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
            currentSheet = data_sheets[-1]
            currentSheet.write(0,0,groupName)
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
            averageDeviations = []
            TlboDeviations = []
            algRuntimes = []
            totalRuntimes = []
        
            for fileTuple in fileGroup:
                for problemNumber in range(1,(fileTuple[1]+1)):
                    #run Tlbo and get profit
                    print 'Problem:'+str(fileTuple[0])+'- '+str(problemNumber)+' executing...'
                    
                    if alg == 'tlbo':
                        pass
                        runTlbo(folderIndex,fileTuple[0],problemNumber)
                    elif alg == 'coa':
                        runCoa(folderIndex,fileTuple[0],problemNumber)
                    elif alg == 'ga':
                        runGa(folderIndex,fileTuple[0],problemNumber)
                    elif alg == 'bba':
                        runBba(folderIndex,fileTuple[0],problemNumber)
                    elif alg == 'aco':
                        runAco(folderIndex,fileTuple[0],problemNumber)
                    else:
                        print 'error: algorithm unavaliable'
                        exit(0)

                    BestObjFunc,runTime = readData(1,'tempContentDump.txt',alg)
                        #BestObjFunc,runTime = (float(exact_results[problemCounter+1]),
                        #                  float(1))
                    print 'Profit:: '+str(BestObjFunc)
                    #write problem name
                    if(groupName == 'orlib'):
                        currentSheet.write((hRow+1)+problemCounter, hCol,str(fileTuple[0]))
                    else:
                        currentSheet.write((hRow+1)+problemCounter, hCol,str(problemNumber))
                    #write results for other algs
                    otherAlgs = [algs_deviations[x][problemCounter+1] \
                                for x in range(len(algs_deviations))]
                    
                    printResults(otherAlgs,currentSheet,
                                 TlboStartRow+problemCounter,(hCol+1))
                    #write results for Tlbo
                    TlboDev = (abs((BestObjFunc-exact_results[problemCounter+1]))/
                               exact_results[problemCounter+1])*100
                    TlboDeviations.append(TlboDev)
                    algRuntimes.append(runTime)
                    currentSheet.write(TlboStartRow+problemCounter,TlboStartCol,
                                       float((Decimal("{0:.3}".format(TlboDev)))))
                    currentSheet.write(TlboStartRow+problemCounter,TlboStartCol+1,
                                       float((Decimal("{0:.3}".format(runTime)))))
                    problemCounter += 1
                    
                    print 'Problem complete.'
                    data_file.save('results/'+alg+'_OverviewResults.xls')
    
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
            problemRuntimes.append(total)
            data_file.save('results/'+alg+'_OverviewResults.xls')

    #summary of results page
    data_sheets.append(data_file.add_sheet("Results",cell_overwrite_ok=True))
    currentSheet = data_sheets[-1]
    currentSheet.write(0,0,"Overview Of Results")
    
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
    for i in range(len(problemDeviations)):
        currentSheet.write(TlboStartRow+i,hCol,problemNames[i])
        for j in range(len(problemDeviations[i])):
            if(problemDeviations[i][j] == "NA"):
                currentSheet.write(TlboStartRow+i,(hCol+1)+j,"")
            else:
                currentSheet.write(TlboStartRow+i,(hCol+1)+j,
                               Decimal("{0:.3}".format(problemDeviations[i][j])))
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
                      
    data_file.save('results/'+alg+'_OverviewResults.xls')

if(__name__=='__main__'):
    num_argv = len(sys.argv)
    
    if (num_argv == 2):
        alg = str(sys.argv[1])
        run(alg)
    else:
        run('bba')
        run('ga')
        run('tlbo')
        run('coa')
        run('aco')


