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
##################################################################

#!/usr/bin/python

import itertools

'''
ALG_TYPE:
0: Normal TLBO
1: TLBO w/ 4 teachers,self motivated learning, and learning
    through tutorial(i-tlbo)
2: TLBO w/ 4 teachers,self motivated learning, and learning
    through tutorial (i'-tlbo)
3: Modified TLBO

i'-tlbo, adaption by vasko, lu, and zyma
which uses even distribution for teachers amonst
population size.

RLS_ON:
0: no reactive local search on teacher's
1: reactive local serach on teachers
'''

ALG_TYPE = 3
RLS_ON = 1
