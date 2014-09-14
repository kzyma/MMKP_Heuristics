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
class BBA_parameters:public MetaHeuristic_parameters{
public:
    BBA_parameters(){}
};

typedef struct{
    MMKPSolution solution;
    float fmax;     //frequency max
    float fmin;     //frequency min
    float v;        //velocity
    float r;        //rate of pulse
    float a;        //loudness
}MMKPBatSolution;

/**
 * Crisscross optimization algrithm for the
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_BBA:MMKP_MetaHeuristic{
private:
    BBA_parameters parameters;
public:
    /**
     * Construct MMKP_BBA object. Param: parameters can customize
     * the tlbo algorithm according to struct BBA_parameters.
     */
    MMKP_BBA(MMKPDataSet dataSet, BBA_parameters parameters);
    
    //overloaded operators
    /**
     * Run coa algorithm and return the result, which is the best objective
     * function.
     */
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    /**
     * Sort a population of MMKPSolution's using quicksort. Param p is
     * the starting array element and r is the last. Note that r is not
     * equal to the size of input, but the index of the last element to
     * be sorted (in most cases size-1).
     */
    void quickSort(std::vector<MMKPBatSolution>& input,int p, int r);
    
    /**
     * Quicksort helper funtion.
     */
    int partition(std::vector<MMKPBatSolution>& input,int p, int r);
    
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
     */
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);
    
    /**
     *
     */
    void initBatParemeters(std::vector<MMKPBatSolution>& population);
    
    /**
     *
     */
    void globalSearch(std::vector<MMKPBatSolution>& population);
};


#endif /* defined(____MMKP_BBA__) */
