/***************************************************************************
 *
 * File: MMKP_BBA.cpp
 * Author: Ken Zyma
 *
 * Binary Bat Metaheuristic for MMKP.
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
 * knapsack problem. Adoption to MMMKP based on Sabba et al. paper 
 * "A discrete binary version of the bat algorithm for MMKP".
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
    
    
    /**
     * Initialize frequency, velocity, rate of pulse and loudnes. Frequency 
     * and rate of pulse take a random value from 0-1, velocity may take a 
     * value between -1 and 1 and loudness may take a value between 1-2. Uniform
     * distribution is used to randomly select in the domain's given.
     */
    void initBatParemeters(std::vector<MMKPBatSolution>& population);

    /**
     * Global Search phase of BBA. All solutions are modified based on the 
     * best solutions quality.
     */
    void globalSearch(std::vector<MMKPBatSolution>& population);

    /**
     * Update solution based on a local search strategy. This allows the
     * algorithm to generate a new best solution around one of the current best
     * solutions.
     */
    void localSearch(MMKPBatSolution& bestBat, MMKPBatSolution& population);
    
    /**
     * Randomly 'flip bits' of a solution.
     */
    void randomSearch(MMKPBatSolution& sol);
    
    void competitiveUpdateSol
    (MMKPBatSolution& sol, MMKPBatSolution& newSol);
};


#endif /* defined(____MMKP_BBA__) */
