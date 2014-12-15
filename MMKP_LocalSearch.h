/***************************************************************************
 *
 * File: MMKP_LocalSearch.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Local Search heuristics for the MMKP.
 *
 *******************************************************************/

#ifndef ____MMKP_LocalSearch__
#define ____MMKP_LocalSearch__

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

#include "MMKPSolution.h"
#include "MMKPDataSet.h"

/**
 * Base/Helper class for all local search procedures.
 */
class LocalSearch{
protected:
    MMKPDataSet dataSet;
    int funcEvals;
public:
    LocalSearch(MMKPDataSet dataSet);
    virtual MMKPSolution run(MMKPSolution solution)=0;
    virtual MMKPSolution operator()(MMKPSolution solution)=0;
    virtual std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions)=0;
    virtual std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions)=0;
    
    
    int getFuncEvals();
    /**
     *  Return suggested item to swap with in class @param class.
     */
    int localSwapProcedure(MMKPSolution solution,int classI);
};

/**
 * A Complementary Local Search Procedure based on the following paper:
 * Mhand Hifi et al. Heuristic algorithms for the multiple-choice
 * multidimensional knapsack problem.
 */
class CompLocalSearch:public LocalSearch{
protected:
public:
    /**
     * Constuct instance of CompLocalSearch.
     */
    CompLocalSearch(MMKPDataSet dataSet);
    
    /**
     * Run a complementary local search on solution.
     */
    MMKPSolution run(MMKPSolution solution);
    
    /**
     * Run a complementary local search on solution.
     */
    MMKPSolution operator()(MMKPSolution solution);
    
    /**
     * Run a complementary local search on all solutions in vector solutions.
     */
    std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions);
    
    /**
     * Run a complementary local search on all solutions in vector solutions.
     */
    std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions);
};

/**
 * A Reactive Local Search Procedure based on the following paper:
 * Mhand Hifi et al. Heuristic algorithms for the multiple-choice
 * multidimensional knapsack problem.
 */
class ReactiveLocalSearch:public LocalSearch{
protected:
    MMKPDataSet penalize(MMKPSolution solution,int delta, float pie);
public:
    
    /**
     * Constuct instance of ReactiveLocalSearch.
     */
    ReactiveLocalSearch(MMKPDataSet dataSet);
    
    /**
     * Run a reactive local search on a solution.
     */
    MMKPSolution run(MMKPSolution solution);
    
    /**
     * Run a reactive local search on a solution
     */
    MMKPSolution operator()(MMKPSolution solution);
    
    /**
     * Run a reactive local search on all solutions in vector solutions.
     */
    std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions);
    
    /**
     * Run a reactive local search on all solutions in vector solutions.
     */
    std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions);
};


#endif /* defined(____MMKP_LocalSearch__) */
