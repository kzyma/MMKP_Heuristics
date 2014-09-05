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

build: TlboApp CoaApp GaApp BbaApp

TlboApp: MMKP_TLBO.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/TlboApp TlboApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_TLBO.o MMKP_MetaHeuristic.o

CoaApp: MMKP_COA.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/CoaApp CoaApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_COA.o MMKP_MetaHeuristic.o

GaApp: MMKP_GA.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/GaApp GaApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_GA.o MMKP_MetaHeuristic.o

BbaApp: MMKP_BBA.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/BbaApp BbaApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_BBA.o MMKP_MetaHeuristic.o

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

MMKP_MetaHeuristic.o:
	g++ $(FLAGS) -c MMKP_MetaHeuristic.cpp

mostlyclean:
	rm *.o

clean:
	rm -rf *.o;
	rm -rf build/TlboApp
	rm -rf build/CoaApp
	rm -rf build/GAApp
	rm -rf build/BbaApp
	rm -rf build/VerifySolution