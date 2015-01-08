/***************************************************************************
 *
 * File: MMKP_ABC.cpp
 * Author: Ken Zyma
 *
 * Artificial Bee Colony metaheuristic.
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
