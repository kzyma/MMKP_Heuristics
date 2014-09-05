##################################################################
#
# File: GaSettings.cpp
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

#!/usr/bin/python

import itertools

#Tlbo population size.
POP_SIZE = [40]

#Number of generations
GEN_SIZE = [60]


#COA Modifications
MOD1 = ['1']
MOD2 = ['2']
MOD3 = ['1']
MOD_LIST = [MOD1,MOD2,MOD3]
MODS = []



#fill mods when imported with all permutations of modifiers.
MODS = list(itertools.product(*MOD_LIST))

for i in range(len(MODS)):
    MODS[i] = ''.join(MODS[i])
    
