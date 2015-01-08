##############################################################
#
# MMHPH Heuristics
# Author: Ken Zyma 
#
##############################################################

This directory contains source for testing heuristics on the Multiple-Choice Multidimensional Knapsack problem (MMKP).

Supported problem instances are: 

1) Legacy, 13 problem, Or-Lib library
2) MMKP library of 370 problems by Hiremath and Hill.

Heuristic’s currently supported are:

1)Teaching-Learning Based optimization (TLBO)
2)Artificial Bee Colony (ABC)
3)Ant Colony Optimization (ACO)
4)Criss-cross optimization(COA)
5)Binary Bat Algorithm (BBA)
6)Genetic Algorithm (GA)

For source documentation please visit http://bill.kutztown.edu/~kzyma650/MMKP_DOCS

To compile and run:

1)Download a copy of source code and navigate into the main MMKP_Heuristics folder.
2)In order to use the test driver you will also need python and dependent libraries
  shown bellow under “Dependencies”.
3)Run ‘make’ to compile source. Check below for compiler versions and build
  environments that have been tested to be working.
4)You may now want to configure which problems you would like to run or specific
  algorithm settings. For this navigate to the “build” folder and edit 
  _Settings.py files. For example the file GeneralSettings.py allows you to
  choose while problems to run, the population size of heuristics, ect.
5)Run the algorithm by invoking “python AlgRunner.py [algorithm]” in the build
 folder. Possible [algorithm] arguments include bba, tlbo, abc, coa and ga.
6)Results will be output in the subfolder “results” as excel files:
    results/[alg]_OverviewResults.xls
    results/[alg]_FuncEvals.xls
    results/graphs/[alg]_ConvResults.xls

*note* you make also directly run the executable file ./HeuristicApp, in which
case results will be directly output to the console.

./HeuristicApp [data folder] [problem] [problem #] [algorithm] 
               [algorithm modifications] [population size] [# of generations] 
               [algorithm specific parameters]

ex:

./HeuristicApp orlib_data I01 1 ga 1210 90 60 2 0.05

Dependencies:

- Python version 2.7 or higher (note that using 3.0+ may not work depending on 
  other library support dependent — check the vendor pages for python 3.0 support).

- If you are going to use stats.py script then SciPy is needed for the statistical
  analysis. Get at http://www.scipy.org/install.html . This should also bring in
  another dependency automatically, the numpy library.

- xlwt: used for writing to excel files. https://pypi.python.org/pypi/xlwt

- xlrt: used for reading from excel files. https://pypi.python.org/pypi/xlrd

Compilation has been tested for the following:

- GNU compiler collection. g++. Apple LLVM version 6.0 (clang-600.0..56)
  target: x86_64-apple-darwin13.4.0
  Thread model: posix

This repository is available under the terms of the MIT open source license:

The MIT License (MIT)

Copyright (c) [2015] [Kutztown University]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.