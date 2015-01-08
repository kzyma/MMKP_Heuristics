##################################################################
#
# File: GeneralSettings.cpp
# Author: Ken Zyma
#
# Py Version: 2.7
#
# The MIT License (MIT)
#
# Copyright (c) [2015] [Kutztown University]
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
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
         [[('TestSet1.5GP10IT5KP.txt',2)],[('TestSet.50GP10IT10KP.txt',1)]]]
'''

#random number seed, True: use default settings (for same initial population) or
#false for random number seed based on time.
SEED = True

POP_SIZE = [90]

GEN_SIZE = [60]

'''
Algorithm Modifications
MOD1 : Popululation creation
0 = Generate a feasible, randomized population based on MMKPDataSet for
    class/item size, feasilbility, profit and cost constraint calculations.
1 = Generate a feasible, randomized population based on MMKPDataSet for
    class/item size, feasilbility, profit and cost constraint calculations.
    This population does not allow for repeated solutions. Note this does not 
    currently validate that more solutions exist than the size of population being 
    generated. This case could cuase an infinite loop, however, will never occur
    in any of the benchmark problems currently supported.

MOD2 : Modification's to regain Multiple Choice Feasibility
0 = select based on surrogate resource/profit. If no items in a
    class are selected, then choose the one with the highest surrogate value.
1 = select based on surrogate resource/profit. If no items
    in a class are selected, then choose the one with the highest profit.
2 = select item with highest profit in class.
3 = randomly decide between using 0 and 2 each time function invoked.
4 = do nothing.

MOD 3 = Modifications to regain Multidimensional Feasbilitiy
0 = Make multi-dim feasible based a surrogate constraint
    created using all costs in an item.
1 = Make multi-dim feasible based a surrogate constraint
    created using violated constraints.
2 = Make multi-dim feasible based a surrogate constraint
    created using violated constraints. This is slower to regain feasbility
    because it attempts to keep a high profit solution, instead of focusing on
    regraining feasibility as fast as possible.
3 = Similar to DIM_VARIABLEMIN, however instead of using
    a surrogate profit, this uses actual item profit to keep a high
    profit solution. Again, this method is slower at regaining feasibility
    becuase it attempts to keep a high profit solution.
4 = Randomly choose between 0 and 2 each time function is invoked.
5 = do nothing

MOD 4 = Local search added AFTER the heuristic is ran.
0 = OFF
1 = Complementary Local Search is added
2 = Reactive Local Search is added
'''
MOD1 = ['1']
MOD2 = ['2']
MOD3 = ['1']
MOD4 = ['0']
MOD_LIST = [MOD1,MOD2,MOD3,MOD4]
MODS = []

MODIFIER = MOD1[0]+MOD2[0]+MOD3[0]+MOD4[0]




