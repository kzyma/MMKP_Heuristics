/*********************************************************
 *
 * File: MMKPPopulationGenerators.h
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Problem
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
 *
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
