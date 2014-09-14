##################################################################
#
# File: CoaSettings.cpp
# Author: Ken Zyma
#
# Py Version: 2.7
#
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Settings for CoaRunner.py
#
##################################################################

#!/usr/bin/python

import itertools

#Tlbo population size.
POP_SIZE = [60]

#Number of generations
GEN_SIZE = [60]


#COA Modifications
MOD1 = ['1']
MOD2 = ['2']
MOD3 = ['1']
MOD_LIST = [MOD1,MOD2,MOD3]
MODS = []

#Set horizontal and verical probabilities
V_PROB = 1.0
H_PROB = 0.8

#fill mods when imported with all permutations of modifiers.
MODS = list(itertools.product(*MOD_LIST))

for i in range(len(MODS)):
    MODS[i] = ''.join(MODS[i])
    
