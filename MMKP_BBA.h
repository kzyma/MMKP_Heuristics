/***************************************************************************
 *
 * File: MMKP_BBA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Binary Bat Metaheuristic for MMKP.
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
    float r_0;
}MMKPBatSolution;

/**
 * Binary Bat Algorithm for the multiple-choice, multiple-dimensional 
 * knapsack problem.
 */
class MMKP_BBA:public MMKP_MetaHeuristic{
private:
    BBA_parameters parameters;
public:

    MMKP_BBA(MMKPDataSet dataSet, BBA_parameters parameters);
    
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    void quickSort(std::vector<MMKPBatSolution>& input,int p, int r);
    
    int partition(std::vector<MMKPBatSolution>& input,int p, int r);
    
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);
    
    void initBatParemeters(std::vector<MMKPBatSolution>& population);

    void globalSearch(std::vector<MMKPBatSolution>& population);

    void localSearch(MMKPBatSolution& bestBat, MMKPBatSolution& population);
    
    void randomSearch(MMKPBatSolution& sol);
    
    void competitiveUpdateSol
    (MMKPBatSolution& sol, MMKPBatSolution& newSol);
};


#endif /* defined(____MMKP_BBA__) */
