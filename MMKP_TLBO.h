/***************************************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Problem
 *
 *  ::Tlbo parameters::
 *
 *  Add Randomized Teacher mod:
 *
 *       0: do not include mod
 *       1: include mod
 *
 *       Teacher is chosen at random (uniform distribution) from the top
 *       10% of a population.
 *
 *  Size of Classroom:
 *          Introduced as i-tlbo, the number of teachers is determined by
 *          popluation size / size of classroom. For one teacher this value
 *          should equal zero.
 *
 *******************************************************************/

#ifndef ____MMKP_TLBO__
#define ____MMKP_TLBO__

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
#include "MMKP_LocalSearch.h"

/**
 * Parameters for customizing the TLBO algorithm.
 */
class TLBO_parameters:public MetaHeuristic_parameters{
public:
    TLBO_parameters(){
    }
};

/**
 * Teaching-learning-based optimization algrithm for the 
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_TLBO:public MMKP_MetaHeuristic{
private:
    TLBO_parameters parameters;
public:
    /**
     * Construct MMKP_TLBO object. Param: parameters can customize
     * the tlbo algorithm according to struct TLBO_parameters.
     */
    MMKP_TLBO(MMKPDataSet dataSet, TLBO_parameters parameters);
    
    MMKP_TLBO(MMKPDataSet dataSet);
    
    //overloaded operators
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
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
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);
    
    /**
     * TLBO teaching phase. See Vasko et al. paper for more.
     */
    void teachingPhase(std::vector<MMKPSolution>& population);
    
    /**
     * i-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers.
     */
    void teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                    int classSize);
    
    /**
     * O-TLBO teaching phase. The teaching phase has been modified to
     * include...
     */
    void teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
    /**
     *
     */
    void RandomSelectPathRelink(std::vector<MMKPSolution>& population,float mutateRate);
    
    /**
     *
     */
    void RandomizedMutation(std::vector<MMKPSolution>& population,float mutateRate);
    
    /**
     * TLBO learning phase. See Vasko et al. paper for more.
     */
    void learningPhase(std::vector<MMKPSolution>& population);
    
    /**
     * O-TLBO learning phase. The learning phase has been modified...
     */
    void learningPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
};

#endif /* defined(____MMKP_TLBO__) */
