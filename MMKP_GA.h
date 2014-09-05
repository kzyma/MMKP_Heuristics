/***************************************************************************
 *
 * File: MMKP_GA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * GA MMKP Problem
 *
 *  ::Ga parameters::
 *
 *  Parent Pool Size, Mutation Probability
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
 * Crisscross optimization algrithm for the
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_GA:public MMKP_MetaHeuristic{
private:
    GA_parameters parameters;
    MMKPSolution bestSolution;
    
public:
    /**
     * Construct MMKP_GA object. Param: parameters can customize
     * the tlbo algorithm according to struct GA_parameters.
     */
    MMKP_GA(MMKPDataSet dataSet, GA_parameters parameters);
    
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
    void parentSelection
    (MMKPSolution& p1, MMKPSolution& p2,std::vector<MMKPSolution>& population);
    
    /**
     *
     */
    MMKPSolution Crossover(MMKPSolution p1,MMKPSolution p2);
    
    /**
     *
     */
    void Mutate(MMKPSolution& solution);
    
};

#endif /* defined(____MMKP_GA__) */
