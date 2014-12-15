/***************************************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Teaching-Learning Based Optimization for the MMKP.
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
#include <queue>

#include "MMKP_MetaHeuristic.h"
#include "MMKPSolution.h"
#include "MMKPDataSet.h"
#include "MMKP_LocalSearch.h"

/**
 * Parameters for customizing the TLBO algorithm.
 */
class TLBO_parameters:public MetaHeuristic_parameters{
public:
    int alg_Type;
    int rls_on;
    TLBO_parameters():alg_Type(0),rls_on(0){}
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
     * i'-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers. Distribution of teachers is even.
     */
    void teachingPhase_MultiTeacherEvenDist(std::vector<MMKPSolution>& population,
                                    int numberOfTeachers);
    
    /**
     * i-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers. Distribution of teachers is even.
     */
    void teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                    int numberOfTeachers);
    
    /**
     * mod-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers. Distribution of teachers is even.
     */
    void teachingPhase_Modified(std::vector<MMKPSolution>& population,
                                    int numberOfTeachers);
    
    void teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
    void learningPhase(std::vector<MMKPSolution>& population);
    
    void improvedLearningPhase(std::vector<MMKPSolution>& population);

    void modifiedLearningPhase(std::vector<MMKPSolution>& population);
    
    void learningPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
};

#endif /* defined(____MMKP_TLBO__) */
