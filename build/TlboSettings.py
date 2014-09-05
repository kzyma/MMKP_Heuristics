##################################################################
#
# File: TlboSettings.cpp
# Author: Ken Zyma
#
# Py Version: 2.7
#
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Settings for TlboRunner.py
#
##################################################################

#!/usr/bin/python

import itertools

#Tlbo population size.
POP_SIZE = [40]

#Number of generations for Tlbo
GEN_SIZE = [60]

#size of tlbo classroom. Multiple teachers
#introduced as i-tlbo
CLASSROOM_SIZE = [0]

#Tlbo Modifications
MOD1 = ['1']
MOD2 = ['2']
MOD3 = ['1']
MOD4 = ['0']
MOD5 = ['0']
MOD_LIST = [MOD1,MOD2,MOD3,MOD4,MOD5]
MODS = []

#fill mods when imported with all permutations of modifiers.
MODS = list(itertools.product(*MOD_LIST))

for i in range(len(MODS)):
    MODS[i] = ''.join(MODS[i])
    
