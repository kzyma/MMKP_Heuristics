/***************************************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * Teaching-Learning Based Optimization for the MMKP.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) [2015] [Kutztown University]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
     * TLBO teaching phase for discrete optimization.
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
     * include multiple teachers. Distribution of teachers is not even. Based on
     * method from Rao et al. in "An improved TLBO".
     */
    void teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                    int numberOfTeachers);
    
    /**
     * mod-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers. Distribution of teachers is even.
     */
    void teachingPhase_Modified(std::vector<MMKPSolution>& population,
                                    int numberOfTeachers);
    
    /**
     * Orthognal TLBO teaching phase. Uses a single teacher and weights the
     * selection of an 'item' in a 'class' based on the number of generations
     * that haved passed. As more generations pass the search is less
     * exploratory.
     */
    void teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
    /**
     * Based TLBO learning phase for discrete problems.
     */
    void learningPhase(std::vector<MMKPSolution>& population);
    
    /**
     * i-tlbo Learning Phase. Uses self motivated learning from Rao et al.
     * "an improved tlbo..."
     */
    void improvedLearningPhase(std::vector<MMKPSolution>& population);

    /**
     * mod-tlbo learning phase. Here self motivated learning is an extra
     * step, as opposed to i-tlbo, so this method is more expensive
     * computationally but may yield better results.
     */
    void modifiedLearningPhase(std::vector<MMKPSolution>& population);
    
    /**
     * Orthagnal learning phase takes into account the number of generations
     * to reduce exploration of the states space search as the number of
     * generations increases.
     */
    void learningPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
};

#endif /* defined(____MMKP_TLBO__) */
