/***************************************************************************
 *
 * File: MMKP_BBA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Binary Bat Alg MMKP Problem
 *
 *  ::Bba parameters::
 *
 *  multipleChoiceMod:
 *      FixedMaxSurrogate/0:
 *         If one item is selected in a class, continue. If more than one
 *         item selected, choose the one with highest v/[E_r%/n] value. If
 *         none selected, choose item from class with highest v/[E_r%/n].
 *
 *      FixedMaxRandomSurragate/1:
 *         Same as 0, except if no items are selected, then randomly choose
 *         an item from the class.
 *
 *      HighProfit/2:
 *          If one item is selcted in a class, continue. If more than one
 *          item is selected choose the one with the highest profit. If
 *          none are selected choose the item in the class which
 *          maximizes profit.
 *
 *
 *  multipleDimMod:
 *      FixedMaxSurrogate/0:
 *         Use n, max diff, fixed size surrogate constraint to obtain
 *         multiple dimension feasibility, where n is the
 *         number of constraints of a problem. Additionally, maximum
 *         difference of surrogate values used to quickly obtain
 *         feasbility.
 *
 *      VariableMaxSurrogate/1:
 *         Use variable, max diff, surrogate constraint to obtain
 *         multiple dimension feasbility. Similar to (0), the max
 *         difference of surrogate values are used to quickly
 *         obtain feasilibty. However, this method calculates
 *         it's surrogate constraint values based not on all problem
 *         consraints, but only the ones violated.
 *
 *      VariableMinSurrogate/2:
 *         Similar to #1, this method uses a variable size surrogate
 *         constraint equal to the number of violated constraints.
 *         However, this method uses the minimum difference to slowly
 *         approach feasibility, in the hopes of getting a better solution.
 *
 *      MaxProfit/3:
 *         Make multi-dim feasible based on maximizing profit. This
 *         procedure is more expensive then others becuase it
 *         attempts to gain feasibility while staying at the
 *         highest profit via local search heuristic.
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
typedef struct{
    int populationSize;
    int numberOfGenerations;
    int multipleChoiceFeasibilityMod;
    int multipleDimFeasibilityMod;
}BBA_parameters;

typedef struct{
    MMKPSolution solution;
    float fmax;     //frequency max
    float fmin;     //frequency min
    float v;        //velocity
    float r;        //rate of pulse
    float a;        //loudness
}MMKPBatSolution;

/**
 * Default parameter settings.
 * Example usage:
 *  BBA_parameters param = DEFAULT_BBA_PARAMETERS;
 */
const BBA_parameters DEFAULT_BBA_PARAMETERS = {
    30,60,2,1
};

/**
 * Crisscross optimization algrithm for the
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_BBA{
private:
    MMKPDataSet dataSet;
    BBA_parameters parameters;
    int currentGeneration;
    /*
     * Update solution based on 'competetive strategy'. A feasible
     * solution is always accepted over an infeasible one. If both
     * are feasible, choose the one with the highest profit. If
     * both are infeasible, choose the one with the highest profit.
     */
    void competitiveUpdateSol(MMKPSolution& sol, MMKPSolution& newSol);
    
public:
    /**
     * Construct MMKP_BBA object. Param: parameters can customize
     * the tlbo algorithm according to struct BBA_parameters.
     */
    MMKP_BBA(MMKPDataSet dataSet, BBA_parameters parameters);
    
    //overloaded operators
    /**
     * Run coa algorithm and return the result, which is the best objective
     * function.
     */
    MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation);
    
    //mutators
    
    //other
    /**
     * Add to unqiue solution set. If Solution exists
     * this function does nothing, and if not it will be
     * added to unique solutions list.
     */
    void addToUniqueSolutionSet(MMKPSolution sol);
    
    /**
     * Sort a population of MMKPSolution's using quicksort. Param p is
     * the starting array element and r is the last. Note that r is not
     * equal to the size of input, but the index of the last element to
     * be sorted (in most cases size-1).
     */
    void quickSort(std::vector<MMKPBatSolution>& input,int p, int r);
    
    /**
     * Quicksort helper funtion.
     */
    int partition(std::vector<MMKPBatSolution>& input,int p, int r);
    
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
     */
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    /**
     *
     */
    void initBatParemeters(std::vector<MMKPBatSolution>& population);
    
    /**
     *
     */
    void globalSearch(std::vector<MMKPBatSolution>& population);
    
    /**
     * Wrapper function for feasibility routines. Return's false
     * if solution was unable to be made feasible.
     */
    bool makeFeasible(MMKPSolution& sol);
    
    /**
     * Wrapper function for feasiblity routines. Uses function parameters
     * instead of Parameters struct to dictate which feasibilty algorithm
     * to apply.
     */
    bool makeFeasible(MMKPSolution& sol,int mcFeas,int mdFeas);
    
    /**
     * Make multi-choice feasible based a surrogate constraint 
     * created using all costs in an item. If one item of a class
     * is selected, continue, else if more than one is selected
     * the item with the highest profit-resource ratio
     * is used. If none are selected choose the item in that class with
     * the hightest profit-resource ratio. Return's true if
     * successful, false otherwise.
     */
    bool makeMultiChoiceFeasFixedSurrogate(MMKPSolution& sol);
    
    /**
     * Make multi-choice feasible based a surrogate constraint
     * created using all costs in an item. If one item of a class
     * is selected, continue, else if more than one is selected
     * the item with the highest profit-resource ratio
     * is used. If none are selected then randomly choose an item
     * from that class. Return's true if successful, false otherwise.
     */
    bool makeMultiChoiceFeasFixed_Rand_Surrogate(MMKPSolution& sol);
    
    /**
     * Make multi-choice feasible based on profit alone. 
     * If one item of a class is selected, continue, else if 
     * more than one is selected the item with the highest profit
     * is used. If none are selected choose highest profit. 
     * Return's true if successful, false otherwise.
     */
    bool makeMultiChoiceFeasMaxProfit(MMKPSolution& sol);
    
    /**
     * Make multi-dim feasible based a surrogate constraint
     * created using all costs in an item. Feasibility is
     * constructed using highest difference of Er%/n. 
     * Return's true if successful, false otherwise.
     *
     * Precondition: there must only be one item of each
     *      class selected, correctness is not garenteed 
     *      otherwise.
     */
    bool makeMultiDimFeasFixedSurrogate(MMKPSolution& sol);
    
    /**
     * Make multi-dim feasible based a surrogate constraint
     * created using variable number of item's costs's (depending
     * on which violate the problem's resources. Feasibility is
     * constructed using highest difference of Er%/n. Return's 
     * true if successful, false otherwise.
     *
     * Precondition: there must only be one item of each
     *      class selected, correctness is not garenteed
     *      otherwise.
     */
    bool makeMultiDimFeasVariableSurrogate(MMKPSolution& sol);
    
    /**
     * Make multi-dim feasible based a surrogate constraint
     * created using variable number of item's costs's (depending
     * on which violate the problem's resources. Feasibility is
     * constructed using lowest difference of Er%/n. Return's
     * true if successful, false otherwise.
     *
     * Precondition: there must only be one item of each
     *      class selected, correctness is not garenteed
     *      otherwise.
     */
    bool makeMultiDimFeasVarMinSurrogate(MMKPSolution& sol);
    /**
     * Make multi-dim feasible based on maximizing profit. This
     * procedure is more expensive then others becuase it
     * attempts to gain feasibility while staying at the
     * highest profit via local search heuristic.
     *
     * Precondition: there must only be one item of each
     *      class selected, correctness is not garenteed
     *      otherwise.
     */
    bool makeMultiDimFeasVarMaximizeProfit(MMKPSolution& sol);
};


#endif /* defined(____MMKP_BBA__) */
