/***************************************************************************
 *
 * File: MMKP_PSO.cpp
 * Author: Ken Zyma
 *
 * Genetic Algorithm for the MMKP.
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

#ifndef ____MMKP_PSO__
#define ____MMKP_PSO__

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
 * Parameters for customizing the PSO algorithm.
 */
class PSO_parameters:public MetaHeuristic_parameters{
public:
    float learningFactor;
    
    PSO_parameters(){
        this->learningFactor = 2;
    }
};

class Particle{
public:
    MMKPSolution solution;
    MMKPSolution localBest;
    std::vector< std::vector<float> > v;
};

/**
 * Particle Swarm Optimization for the multiple-choice, multiple-dimensional
 * knapsack problem.
 */
class MMKP_PSO:public MMKP_MetaHeuristic{
private:
    PSO_parameters parameters;
    MMKPSolution bestSolution;
    
public:

    MMKP_PSO(MMKPDataSet dataSet, PSO_parameters parameters);
    
    MMKP_PSO(MMKPDataSet dataSet);

    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);

    

    
};

#endif /* defined(____MMKP_PSO__) */
