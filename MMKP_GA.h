/***************************************************************************
 *
 * File: MMKP_GA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Genetic Algorithm for the MMKP.
 *
 *******************************************************************/

#ifndef ____MMKP_GA__
#define ____MMKP_GA__

#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <assert.h>
#include <iomanip>
#include <exception>
#include <cfloat>
#include <algorithm> //for "random shuffle"
#include <random>

#include "MMKP_MetaHeuristic.h"
#include "MMKPSolution.h"
#include "MMKPDataSet.h"

/**
 * Parameters for customizing the GA algorithm.
 */
class GA_parameters:public MetaHeuristic_parameters{
public:
    int parentPoolSizeT;
    float mutateProb;
    
    GA_parameters(){
        this->parentPoolSizeT = 2;
        this->mutateProb = 0.05;
    }
};

/**
 * Generic algrithm for the multiple-choice, multiple-dimensional 
 * knapsack problem.
 */
class MMKP_GA:public MMKP_MetaHeuristic{
private:
    GA_parameters parameters;
    MMKPSolution bestSolution;
    
public:

    MMKP_GA(MMKPDataSet dataSet, GA_parameters parameters);
    
    MMKP_GA(MMKPDataSet dataSet);

    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);

    void parentSelection
    (MMKPSolution& p1, MMKPSolution& p2,std::vector<MMKPSolution>& population);

    MMKPSolution Crossover(MMKPSolution p1,MMKPSolution p2);

    void Mutate(MMKPSolution& solution);
    
};

#endif /* defined(____MMKP_GA__) */
