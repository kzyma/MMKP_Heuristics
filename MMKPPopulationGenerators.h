/*********************************************************
 *
 * File: MMKPPopulationGenerators.h
 * Author: Ken Zyma
 *
 * Classes for generating an initial population for a given
 * problem (dataSet).
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
 *********************************************************/

#ifndef ____MMKPPopulationGenerators__
#define ____MMKPPopulationGenerators__

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>

#include "MMKPSolution.h"
#include "MMKPDataSet.h"

class PopulationGenerator{
public:
    virtual std::vector<MMKPSolution> operator()
    (MMKPDataSet dataSet,int populationSize) = 0;
    bool notIncluded
    (MMKPSolution solution,std::vector<MMKPSolution> population);
    virtual ~PopulationGenerator();
};

/**
 * Generate a feasible, randomized population based on MMKPDataSet for
 * class/item size, feasilbility, profit and cost constraint calculations.
 */
class GenerateRandomizedPopulation:public PopulationGenerator{
private:
    unsigned int seed;
public:
    /**
     * Constuct function object GenerateRandomizedPopulation.
     */
    GenerateRandomizedPopulation();
    
    /**
     * Construct function object GenerateRandomizedPopulation 
     * with a seed for a predicable population.
     */
    GenerateRandomizedPopulation(unsigned int seed);
    
    /**
     * Run generator and return a vector of solutions (MMKPSolution).
     */
    std::vector<MMKPSolution> operator()(MMKPDataSet dataSet,int populationSize);
};

/**
 * Generate a feasible, randomized population based on MMKPDataSet for
 * class/item size, feasilbility, profit and cost constraint calculations.
 * This population does not allow for repeated solutions and does not validate
 * that more solutions exist than the size of population being generated. This
 * would cuase an infinite loop as it is in the best interest of portability
 * that we do not dictate the stopping criterion.
 */
class GenerateRandomizedPopulationNoDups:public PopulationGenerator{
private:
    unsigned int seed;
public:
    /**
     * Constuct function object GenerateRandomizedPopulationNoDups.
     */
    GenerateRandomizedPopulationNoDups();
    
    /**
     * Construct function object GenerateRandomizedPopulationNoDups
     * with a seed for a predicable population.
     */
    GenerateRandomizedPopulationNoDups(unsigned int seed);
    
    /**
     * Run generator and return a vector of solutions (MMKPSolution).
     */
    std::vector<MMKPSolution> operator()(MMKPDataSet dataSet,int populationSize);
};


/**
 * Generate a feasible, randomized population based on MMKPDataSet for
 * class/item size, feasilbility, profit and cost constraint calculations.
 * This population does not allow for repeated solutions and does not validate
 * that more solutions exist than the size of population being generated. This
 * would cuase an infinite loop as it is in the best interest of portability
 * that we do not dictate the stopping criterion. This population may also
 * violate multiple dimension feasibility.
 */
class GenerateRandomizedPopulationNoDups_Infeasible:public PopulationGenerator{
private:
    unsigned int seed;
public:
    /**
     * Constuct function object GenerateRandomizedPopulationNoDups_Infeasible.
     */
    GenerateRandomizedPopulationNoDups_Infeasible();
    
    /**
     * Construct function object GenerateRandomizedPopulationNoDups_Infeasible
     * with a seed for a predicable population.
     */
    GenerateRandomizedPopulationNoDups_Infeasible(unsigned int seed);
    
    /**
     * Run generator and return a vector of solutions (MMKPSolution).
     */
    std::vector<MMKPSolution> operator()(MMKPDataSet dataSet,int populationSize);
};

/**
 * A semi random greedy approach. This chooses an item to add to a class
 * based on 1 of 5 possible functions for calculating a surrogate profit.
 */
class GenerateRandomizedPopulationGreedyV1:public PopulationGenerator{
private:
    unsigned int seed;
public:
    /**
     * Constuct function object GenerateRandomizedPopulationGreedyV1.
     */
    GenerateRandomizedPopulationGreedyV1();
    
    /**
     * Construct function object GenerateRandomizedPopulationGreedyV1
     * with a seed for a predicable population.
     */
    GenerateRandomizedPopulationGreedyV1(unsigned int seed);
    
    /**
     * Run generator and return a vector of solutions (MMKPSolution).
     */
    std::vector<MMKPSolution> operator()(MMKPDataSet dataSet,int populationSize);

};

#endif /* defined(____MMKPPopulationGenerators__) */
