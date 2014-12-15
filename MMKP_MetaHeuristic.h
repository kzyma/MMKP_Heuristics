/*********************************************************
 *
 * File: MMKP_MetaHeuristic.h
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Base class for MMKP metaheuristic package. MMKP_Metaheuristic
 * provides helper functions common to all metaheuristic's such as
 * making a solution feasible and sorting a population.
 *
 *********************************************************/

#ifndef ____MMKP_MetaHeuristic__
#define ____MMKP_MetaHeuristic__

#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <math.h>
#include <assert.h>
#include <iomanip>
#include <exception>
#include <cfloat>
#include <algorithm>
#include <random>
#include <tuple>

#include "MMKPSolution.h"
#include "MMKPDataSet.h"

/**
 * <p>Modification's to regain Multiple Choice Feasibility:
 * <lu>
 * <li>CH_FIXEDMAX = select based on surrogate resource/profit. If no items in a
 * class are selected, then choose the one with the highest surrogate value.</li>
 * <li>CH_FIXEDMAX_RANDOM = select based on surrogate resource/profit. If no items
 * in a class are selected, then choose the one with the highest profit. </li>
 * <li>CH_MAXPROFIT = select item with highest profit in class. </li>
 * <li>CH_MIX = randomly decide between using CH_FIXEDMAX and CH_FIXEDMAX_RANDOM. </li>
 * <li>CH_NONE = do nothing </lu></p>
 */
enum multipleChoiceMod{
    CH_FIXEDMAX = 0,
    CH_FIXEDMAX_RANDOM = 1,
    CH_MAXPROFIT = 2,
    CH_MIX = 3,
    CH_NONE = 4
};

/**
 * <p>Modifications to regain Multidimensional Feasbilitiy:
 * <lu>
 * <li>DIM_FIXEDMAX = Make multi-dim feasible based a surrogate constraint
 * created using all costs in an item.</li>
 * <li>DIM_VARIABLEMAX = Make multi-dim feasible based a surrogate constraint
 * created using violated constraints.</li>
 * <li>DIM_VARIABLEMIN = Make multi-dim feasible based a surrogate constraint
 * created using violated constraints. This is slower to regain feasbility 
 * because it attempts to keep a high profit solution, instead of focusing on
 * regraining feasibility as fast as possible.</li>
 * <li>DIM_MAXPROFIT = Similar to DIM_VARIABLEMIN, however instead of using
 * a surrogate profit, this uses actual item profit to keep a high 
 * profit solution. Again, this method is slower at regaining feasibility
 * becuase it attempts to keep a high profit solution.</li>
 * <li>DIM_MIX = Randomly choose between DIM_MAXPROFIT and DIM_VARIABLEMAX</li>
 * <li>DIM_NONE = do nothing</li>
 * </lu></p>
 */
enum multipleDimMod{
    DIM_FIXEDMAX = 0,
    DIM_VARIABLEMAX = 1,
    DIM_VARIABLEMIN = 2,
    DIM_MAXPROFIT = 3,
    DIM_MIX = 4,
    DIM_NONE = 5
};

/**
 * Parameters for customizing the MetaHeuristic algorithm. Pass as an 
 * argument to constructor of type MMKP_MetaHeuristic.
 */
class MetaHeuristic_parameters{
public:
    int populationSize;
    int numberOfGenerations;
    int multipleChoiceFeasibilityMod;
    int multipleDimFeasibilityMod;
    
    MetaHeuristic_parameters(){
        this->numberOfGenerations = 60;
        this->multipleChoiceFeasibilityMod = 2;
        this->multipleDimFeasibilityMod = 1;
    }
};

/**
 * Base class for MMKP metaheuristic package. MMKP_Metaheuristic
 * provides helper functions common to all metaheuristic's such as
 * making a solution feasible and sorting a population.
 */
class MMKP_MetaHeuristic{
protected:
    MMKPDataSet dataSet;
    MetaHeuristic_parameters parameters;
    std::vector<std::tuple<int,float> > convergenceData;
    int currentFuncEvals;
    int convergenceIteration;
    int currentGeneration;
    
    /*
     * Update solution based on 'competetive strategy'. A feasible
     * solution is always accepted over an infeasible one. If both
     * are feasible, choose the one with the highest profit. If
     * both are infeasible, choose the one with the highest profit.
     */
    void competitiveUpdateSol(MMKPSolution& sol, MMKPSolution& newSol);
    
    /**
     * Add to unqiue solution set. If Solution exists
     * this function does nothing, and if not it will be
     * added to unique solutions list.
     */
    void addToUniqueSolutionSet(MMKPSolution sol);
    
    /**
     * Quicksort helper funtion.
     */
    int partition(std::vector<MMKPSolution>& input,int p, int r);

public:
    /**
     * Construct MMKP_MetaHeuristic object. Param: parameters can customize
     * the algorithm according to an instance of MetaHeuristic_parameters.
     */
    MMKP_MetaHeuristic(MMKPDataSet dataSet, MetaHeuristic_parameters parameters);
    
    /**
     * Construct MMKP_MetaHeuristic object. Uses default parameters.
     */
    MMKP_MetaHeuristic(MMKPDataSet dataSet);
    
    virtual ~MMKP_MetaHeuristic();
    
    //overloaded operators
    /**
     * Run heuristic and return the best objective function. Must be overriden,
     * I would suggest calling the "run" function and placing all code there.
     */
    virtual MMKPSolution operator()(std::vector<MMKPSolution> initialPopulation)=0;
    
    //accessor
    /**
     * Return convergence data, a vector containing the count of function evaluation 
     * and current best objective function. This can be done after running an
     * instance of metaheuristic, else an empty std::vector will be returned.
     */
    std::vector<std::tuple<int,float> > getConvergenceData();
    
    int getConvergenceGeneration();
    
    //other
    /**
     * Sort a population of MMKPSolution's using quicksort. Param p is
     * the starting array element and r is the last. Note that r is not
     * equal to the size of input, but the index of the last element to
     * be sorted (in most cases size-1).
     */
    void quickSort(std::vector<MMKPSolution>& input,int p, int r);
    
    /**
     * Run metaheuristic and return the result, the best objective
     * function.
     */
    virtual MMKPSolution run(std::vector<MMKPSolution> initialPopulation)=0;
    
    /**
     * Run a single iteration of heuristic.
     */
    virtual std::vector<MMKPSolution> runOneGeneration
    (std::vector<MMKPSolution> population)=0;
    
    /**
     * Wrapper function of both feasibility routines.
     */
    bool makeFeasible(MMKPSolution& sol);
    
    /**
     * Wrapper function of both feasiblity routines. Uses function parameters
     * instead of Parameters struct to dictate which feasibilty algorithm
     * to apply.
     */
    bool makeFeasible(MMKPSolution& sol,int mcFeas,int mdFeas);
    
    /**
     * Make multi-choice feasible based on a surrogate constraint
     * created using all costs of an item. If one item of a class
     * is selected, continue, else if more than one is selected
     * the item with the highest profit-resource ratio
     * is used. If none are selected choose the item in that class with
     * the hightest profit-resource ratio. Return's true if
     * successful, false otherwise.
     */
    bool makeMultiChoiceFeasFixedSurrogate(MMKPSolution& sol);
    
    /**
     * Make multi-choice feasible based on a surrogate constraint
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
     * created using a variable number of item's costs's (depending
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

/**
 * Operation not supported/implemented exception.
 */
class OpNotSupported: public std::exception{
    
private:
    std::string message;
public:
    OpNotSupported();
    OpNotSupported(std::string message);
    ~OpNotSupported() throw();
    virtual const char* what() const throw();
};


#endif /* defined(____MMKP_MetaHeuristic__) */
