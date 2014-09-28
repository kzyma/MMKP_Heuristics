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

build: TlboApp CoaApp GaApp BbaApp AcoApp Mmhph Abc

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

AcoApp: MMKP_ACO.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/AcoApp AcoApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_ACO.o MMKP_MetaHeuristic.o

Mmhph: 	MMKP_GA.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_TLBO.o MMKP_COA.o MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/MmhphApp MmhphApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_TLBO.o \
	MMKP_COA.o MMKP_GA.o MMKP_MetaHeuristic.o

Abc:	MMKP_ABC.o MMKPSolution.o MMKPDataSet.o MMKPPopulationGenerators.o \
	MMKP_MetaHeuristic.o
	g++ $(FLAGS) -o build/AbcApp AbcApp.cpp MMKPSolution.o \
	MMKPDataSet.o MMKPPopulationGenerators.o MMKP_ABC.o MMKP_MetaHeuristic.o

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

mostlyclean:
	rm *.o

clean:
	rm -rf *.o;
	rm -rf build/TlboApp
	rm -rf build/CoaApp
	rm -rf build/GaApp
	rm -rf build/BbaApp
	rm -rf build/AcoApp
	rm -rf build/MmhphApp
	rm -rf build/AbcApp
	rm -rf build/VerifySolution
	rm -rf build/AcoSettings.pyc
	rm -rf build/CoaSettings.pyc
	rm -rf build/TlboSettings.pyc
	rm -rf build/GaSettings.pyc
	rm -rf build/GeneralSettings.pyc
	rm -rf build/BbaSettings.pyc