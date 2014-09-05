/***************************************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Problem
 *
 *  ::Tlbo parameters::
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
 *    Add Randomized Teacher mod:
 *
 *       0: do not include mod
 *       1: include mod
 *
 *       Teacher is chosen at random (uniform distribution) from the top
 *       10% of a population.
 *
 *     Add Simulated Annealing mod:
 *
 *       0: do not include mod
 *       1: include threshold annealing.
 *
 *  Size of Classroom:
 *          Introduced as i-tlbo, the number of teachers is determined by
 *          popluation size / size of classroom. For one teacher this value
 *          should equal zero.
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

#include "MMKP_MetaHeuristic.h"
#include "MMKPSolution.h"
#include "MMKPDataSet.h"

enum simulatedHealingMod{
    SIMHEAL_NONE = 0,
    SIMHEAL_THRESHOLD = 1,
    SIMHEAL_ADAPTIVE_TEACH = 2
};

/**
 * Parameters for customizing the TLBO algorithm.
 */
typedef struct{
    int populationSize;
    int numberOfGenerations;
    int generationsOfNOP;
    int classroomSize;  //i.e. i-tlbo number of teachers
    int multipleChoiceFeasibilityMod;
    int multipleDimFeasibilityMod;
    bool addRandomTeacher;
    bool addSimAnnealing;
}TLBO_parameters;

/**
 * Default parameter settings.
 * Example usage:
 *  TLBO_parameters param = DEFAULT_TLBO_PARAMETERS;
 */
const TLBO_parameters DEFAULT_TLBO_PARAMETERS = {
    30,60,10,0,0,0,0,0
};

/**
 * Teaching-learning-based optimization algrithm for the 
 * multiple-choice, multiple-dimensional knapsack problem.
 */
class MMKP_TLBO{
private:
    MMKPDataSet dataSet;
    TLBO_parameters parameters;
    int currentGeneration;
    /*
     * Update solution based on 'competetive strategy'. A feasible
     * solution is always accepted over an infeasible one. If both
     * are feasible, choose the one with the highest profit. If
     * both are infeasible, choose the one with the highest profit.
     */
    void competitiveUpdateSol(MMKPSolution& sol, MMKPSolution& newSol);
    /*
     * Update solution based on a threshhold for simulated
     * annealing style acceptance. Threshold will be reduced
     * as number of generations increase.
     * Based on Vasko et al. methodin "hybrid GA's for set covering".
     * If XPRIME-ROUND((XPRIME)(T/N)) >= XBEST
     */
    void threshholdAnnealingUpdateSol(MMKPSolution& sol, MMKPSolution& newSol);
public:
    /**
     * Construct MMKP_TLBO object. Param: parameters can customize
     * the tlbo algorithm according to struct TLBO_parameters.
     */
    MMKP_TLBO(MMKPDataSet dataSet, TLBO_parameters parameters);
    
    //overloaded operators
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
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
    void quickSort(std::vector<MMKPSolution>& input,int p, int r);
    
    /**
     * Quicksort helper funtion.
     */
    int partition(std::vector<MMKPSolution>& input,int p, int r);
    
    /**
     * Run tlbo algorithm and return the result, which is the 'teaching'
     * solution after a stopping criterion is met.
     */
    MMKPSolution run(std::vector<MMKPSolution> initialPopulation);
    
    /**
     * TLBO teaching phase. See Vasko et al. paper for more.
     */
    void teachingPhase(std::vector<MMKPSolution>& population);
    
    /**
     * i-TLBO teaching phase. The teaching phase has been modified to
     * include multiple teachers.
     */
    void teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                    int classSize);
    
    /**
     * O-TLBO teaching phase. The teaching phase has been modified to
     * include...
     */
    void teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
    /**
     *
     */
    void RandomSelectPathRelink(std::vector<MMKPSolution>& population,float mutateRate);
    
    /**
     *
     */
    void RandomizedMutation(std::vector<MMKPSolution>& population,float mutateRate);
    
    /**
     * TLBO learning phase. See Vasko et al. paper for more.
     */
    void learningPhase(std::vector<MMKPSolution>& population);
    
    /**
     * O-TLBO learning phase. The learning phase has been modified...
     */
    void learningPhase_Orthognal(std::vector<MMKPSolution>& population,
                                 int iteration);
    
    /**
     * Wrapper function for feasibility routines.
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

#endif /* defined(____MMKP_TLBO__) */
