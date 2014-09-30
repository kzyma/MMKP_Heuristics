##################################################################
#
# File: GeneralSettings.cpp
# Author: Ken Zyma
#
# Py Version: 2.7
#
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Settings for GaRunner.py
#
##################################################################

#files housed in multi-dim list, where each row is a different
#problem set (folder), and the cols list all problems.
#ex: files[folder][problem#]
#Additionally, each 'file' is a tuple consisting of the name of
#the file and number of problems in the file.
#*note*, HiremathHill problem's go from small -> large


FOLDERS = ['orlib_data','HiremathHill_data']

FILES = [[[('I01',1),('I02',1),('I03',1),('I04',1)
           ,('I05',1),('I06',1),('I07',1),('I08',1)
           ,('I09',1),('I10',1),('I11',1),('I12',1),('I13',1)]],
         
         [[('TestSet1.5GP10IT5KP.txt',30)],[('TestSet2.10GP10IT5KP.txt',30)],
          [('TestSet3.25GP10IT5KP.txt',30)],[('TestSet4.5GP10IT10KP.txt',30)],
          [('TestSet5.10GP10IT10KP.txt',30)],[('TestSet6.25GP10IT10KP.txt',30)],
          [('TestSet7.5GP10IT25KP.txt',30)],[('TestSet8.10GP10IT25KP.txt',30)],
          [('TestSet9.25GP10IT25KP.txt',30)],
          [('TestSet.30GP10IT10KP.txt',10)],[('TestSet.50GP10IT10KP.txt',10)],
          [('TestSet.70GP10IT10KP.txt',10)],[('TestSet.80GP10IT10KP.txt',10)],
          [('TestSet.100GP10IT10KP.txt',10)],[('TestSet.150GP10IT10KP.txt',10)],
          [('TestSet.200GP10IT10KP.txt',10)],[('TestSet.250GP10IT10KP.txt',10)],
          [('TestSet.300GP10IT10KP.txt',10)],[('TestSet.350GP10IT10KP.txt',10)],
          [('TestSet.400GP10IT10KP.txt',10)]]]


'''
FOLDERS = ['orlib_data','HiremathHill_data']

FILES = [[[('I01',1),('I02',1)]],
         [[('TestSet1.5GP10IT5KP.txt',2)],[('TestSet.50GP10IT10KP.txt',3)]]]
'''


#random number seed, True: use default settings (for same initial pop) or
#false for random number seed based on time.
SEED = True

POP_SIZE = [90]

GEN_SIZE = [60]

#COA Modifications
MOD1 = ['1']
MOD2 = ['2']
MOD3 = ['1']
MOD4 = ['0']
MOD_LIST = [MOD1,MOD2,MOD3,MOD4]
MODS = []

MODIFIER = MOD1[0]+MOD2[0]+MOD3[0]+MOD4[0]




