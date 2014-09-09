/***************************************************************************
 *
 * File: MMKP_BBA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Binary Bat Alg MMKP Problem
 *
 *******************************************************************/

#ifndef ____MMKP_BBA__
#define ____MMKP_BBA__

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
class ACO_parameters:public MetaHeuristic_parameters{
public:
    float B;    //B is the weighting of pheramone to heuristic
    float p;    //p and e in pheramone update rule
    float e;
    ACO_parameters(){
        this->B = 25.0;
        this->p = 0.98;
        this->e = 0.005;
    }
};

/**
 * Any Colony Optimization adapted to the
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_ACO:MMKP_MetaHeuristic{
private:
    ACO_parameters parameters;
public:
    /**
     * Construct MMKP_ACO object. Use Param: parameters to customize
     * the aco algorithm according to struct ACO_parameters.
     */
    MMKP_BBA(MMKPDataSet dataSet, ACO_parameters parameters);
    
    //overloaded operators
    /**
     *
     */
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    /**
     *
     */
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
};


#endif /* defined(____MMKP_BBA__) */
