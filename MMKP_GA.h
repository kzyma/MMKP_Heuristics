/***************************************************************************
 *
 * File: MMKP_GA.cpp
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

#ifndef ____MMKP_GA__
#define ____MMKP_GA__

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

/**
 * Parameters for customizing the GA algorithm.
 */
class GA_parameters:public MetaHeuristic_parameters{
public:
    int parentPoolSizeT;
    float mutateProb;
    
    GA_parameters(){
        this->parentPoolSizeT = 2;
        this->mutateProb = 0.05;
    }
};

/**
 * Generic algrithm for the multiple-choice, multiple-dimensional 
 * knapsack problem. Adoption based on paper by Chu and Beasley "A genetic
 * algorithm for the multidimensioanl knapsack problem".
 */
class MMKP_GA:public MMKP_MetaHeuristic{
private:
    GA_parameters parameters;
    MMKPSolution bestSolution;
    
public:

    MMKP_GA(MMKPDataSet dataSet, GA_parameters parameters);
    
    MMKP_GA(MMKPDataSet dataSet);

    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population);

    /**
     * Parent selection is done in competetive approach. Two pools are created
     * of 2 (or more depending on the parameters set) solutiosn from the population.
     * Then the best of each are selected to be returned as p1 and p2.
     */
    void parentSelection
    (MMKPSolution& p1, MMKPSolution& p2,std::vector<MMKPSolution>& population);

    /**
     * Basic ga crossover. Half of the bits of p1 and half of p2 are selected
     * to make a new solution. The bits selected are randomly choosen.
     */
    MMKPSolution Crossover(MMKPSolution p1,MMKPSolution p2);

    /**
     * Flip a solutions bit's based on a probability, namely mutation rate 
     * (default set to 5%). A higher mutation rate will promote state space 
     * exploration.
     */
    void Mutate(MMKPSolution& solution);
    
};

#endif /* defined(____MMKP_GA__) */
