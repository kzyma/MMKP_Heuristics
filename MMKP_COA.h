/***************************************************************************
 *
 * File: MMKP_COA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Criss-cross optimization for the MMKP.
 *
 *******************************************************************/

#ifndef ____MMKP_COA__
#define ____MMKP_COA__

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
 * Parameters for customizing the COA algorithm.
 */
class COA_parameters:public MetaHeuristic_parameters{
public:
    float horizCrossProb;
    float verticalCrossProb;
    
    COA_parameters(){
        this->horizCrossProb = 1.0;
        this->verticalCrossProb = 0.8;
    }
};

/**
 * Crisscross optimization algrithm for the
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_COA:public MMKP_MetaHeuristic{
private:
    COA_parameters parameters;
public:

    MMKP_COA(MMKPDataSet dataSet, COA_parameters parameters);
    
    MMKP_COA(MMKPDataSet dataSet);
    
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);

    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);
    
    /**
     * Horizontal crossover matches each solution with another randomly
     * choosen solution and creates a new solution as a function of these
     * and randomly generated probabilities.
     */
    void horizontalCrossover(std::vector<MMKPSolution>& population);
    
    /**
     * Mutate solution by swapping items in a class between two randomly
     * choosen solutions.
     */
    void verticalCrossover(std::vector<MMKPSolution>& population);
};

#endif /* defined(____MMKP_COA__) */
