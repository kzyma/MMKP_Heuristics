/***************************************************************************
 *
 * File: MMKP_COA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * COA MMKP Problem
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
    /**
     * Construct MMKP_COA object. Param: parameters can customize
     * the tlbo algorithm according to struct COA_parameters.
     */
    MMKP_COA(MMKPDataSet dataSet, COA_parameters parameters);
    
    //overloaded operators
    /**
     * Run coa algorithm and return the result, which is the best objective
     * function.
     */
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
     */
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    /**
     *
     */
    void horizontalCrossover(std::vector<MMKPSolution>& population);
    
    /**
     *
     */
    void verticalCrossover(std::vector<MMKPSolution>& population);
};

#endif /* defined(____MMKP_COA__) */
