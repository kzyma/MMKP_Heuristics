/***************************************************************************
 *
 * File: MMKP_ABC.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Artificial Bee Colony metaheuristic.
 *
 *******************************************************************/

#ifndef ____MMKP_ABC__
#define ____MMKP_ABC__

#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <math.h>
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
 * Parameters for customizing the BBA algorithm.
 */
class ABC_parameters:public MetaHeuristic_parameters{
public:
    float r;
    int abandonmentCriterion;
    ABC_parameters(){
        r = 0.3;
        abandonmentCriterion = 5;
    }
};

/**
 * Solution representation for Bee Colony.
 */
typedef struct{
    MMKPSolution solution;
    int noImproveCount;
}MMKPBeeSolution;

/**
 * Artificial Bee Colony adapted to the multiple-choice, multiple-dimensional 
 * knapsack problem. Based on method by Jia et al. in "Binary Artificial 
 * Bee Colony Optimization Using Bitwise Operation".
 */
class MMKP_ABC:public MMKP_MetaHeuristic{
private:
    ABC_parameters parameters;
public:
    /**
     * Construct MMKP_ABC object. Use Param: parameters to customize
     * the aco algorithm according to struct ABC_parameters.
     */
    MMKP_ABC(MMKPDataSet dataSet, ABC_parameters parameters);
    
    //overloaded operators
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);
    
    /**
     * Modify solution based on equation 3 in "Binary Artificial Bee Colony 
     * Optimization Using Bitwise Operation" by Jia et al.
     */
    std::vector<MMKPBeeSolution> employeedBeePhase
    (std::vector<MMKPBeeSolution> employeedBees);
    
    /**
     * Using roulette style selection an employeed bee is matched with
     * onlooker bee for solution mutation based on equation 3 in 
     * "Binary Artificial Bee Colony Optimization Using Bitwise 
     * Operation" by Jia et al.
     */
    std::vector<MMKPSolution>
    onLookerBeePhase(std::vector<MMKPSolution> onLookerBees,
                     std::vector<MMKPBeeSolution> employeedBees);
    
    /**
     * If an employeed bee's fitness is not improved by a number of 
     * iterations (noImproveCount parameter) the solution is 
     * reset randomly.
     */
    void scoutBeePhase(std::vector<MMKPBeeSolution>& employeedBees);
};


#endif /* defined(____MMKP_ABC__) */
