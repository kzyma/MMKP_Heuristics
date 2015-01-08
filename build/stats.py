##################################################################
#
# File: stats.py
# Author: Ken Zyma
#
# Py Version: 2.7
#
# Statistical anaysis for results from all heuristics on all
# 383 problems. These are hardcoded to be read from AllResults.xls.
#
# The MIT License (MIT)
#
# Copyright (c) <2015> <Kutztown University>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
##################################################################

#!/usr/bin/python

import os,sys
from mmap import mmap,ACCESS_READ
from xlrd import open_workbook
import math
from scipy import stats
import scipy as sp
import numpy as np
from xlwt import Workbook,easyxf

def mean_confidence_interval(data, confidence=0.99):
    a = 1.0*np.array(data)
    n = len(a)
    m, se = np.mean(a), stats.sem(a)
    h = se * sp.stats.t._ppf((1+confidence)/2., n-1)
    return m, m-h, m+h

if(__name__=='__main__'):
    
    '''
    Algoritm Indices
    0:tlbo
    1:tt-tlbo
    2:stoc-tt-tlbo
    3:mod-tlbo
    4:tt-mod-tlbo
    5:stoc-tt-mod-tlbo
    6:i-tlbo
    7:tt-t-tlbo
    8:stoc-tt-tlbo
    '''
    
    data = []
    algs = ['tlbo','tt-tlbo','stoc-tt-tlbo','mod-tlbo','tt-mod-tlbo',
            'stoc-tt-mod-tlbo','i-tlbo','tt-i-tlbo','stoc-tt-i-tlbo']

    wb = open_workbook(('results/AllResults.xls'))
    sheet = wb.sheet_by_index(0)
    
    for col in range(1,sheet.ncols):
        tempData = []
        for row in range(1,sheet.nrows):
            tempData.append(sheet.cell(row,col).value)
        data.append(tempData)


    data_file = Workbook()
    data_sheets = []
    data_sheets.append(data_file.add_sheet('anova',cell_overwrite_ok=True))
    currentSheet = data_sheets[-1]

    currentSheet.write(0,0,'One-way ANOVA')
    currentSheet.write(1,0,'p-value')
    currentSheet.write(len(algs)+3,0,'f-value')
    for i in range(len(algs)):
        currentSheet.write(2,i+2,algs[i])
        currentSheet.write(i+3,1,algs[i])
        currentSheet.write(len(algs)+4,i+2,algs[i])
        currentSheet.write(len(algs)+5+i,1,algs[i])

    for i in range(len(algs)):
        for j in range(len(algs)):
            f_val, p_val = stats.f_oneway(data[i],data[j])
            print p_val
            currentSheet.write(3+i,2+j,p_val)
            currentSheet.write(len(algs)+5+i,j+2,f_val)

    data_sheets.append(data_file.add_sheet('Kruskal',cell_overwrite_ok=True))
    currentSheet = data_sheets[-1]

    currentSheet.write(0,0,'Kruskal-Wallis-H-test')
    currentSheet.write(1,0,'p-value')
    currentSheet.write(len(algs)+3,0,'H-statistic')
    for i in range(len(algs)):
        currentSheet.write(2,i+2,algs[i])
        currentSheet.write(i+3,1,algs[i])
        currentSheet.write(len(algs)+4,i+2,algs[i])
        currentSheet.write(len(algs)+5+i,1,algs[i])

    for i in range(len(algs)):
        for j in range(len(algs)):
            z_stat, p_val = stats.kruskal(data[i],data[j])
            currentSheet.write(3+i,2+j,p_val)
            currentSheet.write(len(algs)+5+i,j+2,z_stat)

    z_stat, p_val = stats.mstats.kruskalwallis(data[0],data[1],data[2],data[3],data[4],
                                  data[5],data[6],data[7],data[8])
    print 'Kruskal Wallis H-test Results (all algs):'
    print 'h stat:',z_stat
    print 'p value:',p_val

    z_stat, p_val = stats.mstats.kruskalwallis(data[0],data[3],data[6])
    print 'Kruskal Wallis H-test Results (tlbo):'
    print 'h stat:',z_stat
    print 'p value:',p_val
    
    z_stat, p_val = stats.mstats.kruskalwallis(data[1],data[2],data[4],
                                               data[5],data[7],data[8])
    print 'Kruskal Wallis H-test Results (tt-tlbo):'
    print 'h stat:',z_stat
    print 'p value:',p_val

    data_sheets.append(data_file.add_sheet('RankSum',cell_overwrite_ok=True))
    currentSheet = data_sheets[-1]
    
    currentSheet.write(0,0,'Mann-Whinet-Wilcoxon Rank Sum test')
    currentSheet.write(1,0,'p-value')
    currentSheet.write(len(algs)+3,0,'z-statistic')
    for i in range(len(algs)):
        currentSheet.write(2,i+2,algs[i])
        currentSheet.write(i+3,1,algs[i])
        currentSheet.write(len(algs)+4,i+2,algs[i])
        currentSheet.write(len(algs)+5+i,1,algs[i])
    
    for i in range(len(algs)):
        for j in range(len(algs)):
            z_stat, p_val = stats.ranksums(data[i],data[j])
            currentSheet.write(3+i,2+j,p_val)
            currentSheet.write(len(algs)+5+i,j+2,z_stat)

    data_file.save('results/statisticalAnaysis.xls')

    '''
    print '### MEAN CONFIDENCE INTERVAL ###'
    for i in range(len(algs)):
        print algs[i],str(mean_confidence_interval(data[i]))

    print '### MEAN VALUE ###'
    for i in range(len(algs)):
        print algs[i],str(np.mean(data[i]))

    print '### MEDIAN VALUE ###'
    for i in range(len(algs)):
        print algs[i],str(np.median(data[i]))
    '''

