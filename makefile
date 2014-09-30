######################################################
#
# File: makefile
# Author: Ken Zyma
# 
# @All rights reserved
# Kutztown University, PA, U.S.A
#
# Build TLBO problem test suite.
#
######################################################

FLAGS = -std=c++11

all: build
	make mostlyclean

build: Heuristic Mmhph

Heuristic: MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o MMKP_TLBO.o MMKP_COA.o MMKP_GA.o MMKP_BBA.o \
	MMKP_ACO.o MMKP_ABC.o MMKP_LocalSearch.o
	g++ $(FLAGS) -o build/HeuristicApp HeuristicApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_MetaHeuristic.o \
	MMKP_TLBO.o MMKP_COA.o MMKP_GA.o MMKP_BBA.o MMKP_ACO.o MMKP_ABC.o \
	MMKP_LocalSearch.o

Mmhph: 	MMKP_GA.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_TLBO.o MMKP_COA.o MMKP_MetaHeuristic.o MMKP_LocalSearch.o
	g++ $(FLAGS) -o build/MmhphApp MmhphApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_TLBO.o \
	MMKP_COA.o MMKP_GA.o MMKP_MetaHeuristic.o MMKP_LocalSearch.o

MMKPDataSet.o:
	g++ $(FLAGS) -c MMKPDataSet.cpp

MMKPSolution.o:
	g++ $(FLAGS) -c MMKPSolution.cpp

MMKPPopulationGenerators.o:
	g++ $(FLAGS) -c MMKPPopulationGenerators.cpp

MMKP_TLBO.o:
	g++ $(FLAGS) -c MMKP_TLBO.cpp

MMKP_COA.o:
	g++ $(FLAGS) -c MMKP_COA.cpp

MMKP_GA.o:
	g++ $(FLAGS) -c MMKP_GA.cpp

MMKP_BBA.o:
	g++ $(FLAGS) -c MMKP_BBA.cpp

MMKP_ACO.o:
	g++ $(FLAGS) -c MMKP_ACO.cpp

MMKP_ABC.o:
	g++ $(FLAGS) -c MMKP_ABC.cpp

MMKP_MetaHeuristic.o:
	g++ $(FLAGS) -c MMKP_MetaHeuristic.cpp

MMKP_LocalSearch.o:
	g++ $(FLAGS) -c MMKP_LocalSearch.cpp

mostlyclean:
	rm *.o

clean:
	rm -rf *.o;
	rm -rf build/HeuristicsApp
	rm -rf build/MmhphApp
	rm -rf build/VerifySolution
	rm -rf build/AcoSettings.pyc
	rm -rf build/CoaSettings.pyc
	rm -rf build/TlboSettings.pyc
	rm -rf build/GaSettings.pyc
	rm -rf build/GeneralSettings.pyc
	rm -rf build/BbaSettings.pyc