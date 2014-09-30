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

class LocalSearch{
protected:
    MMKPDataSet dataSet;
public:
    LocalSearch(MMKPDataSet dataSet);
    virtual MMKPSolution run(MMKPSolution solution)=0;
    virtual MMKPSolution operator()(MMKPSolution solution)=0;
    virtual std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions)=0;
    virtual std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions)=0;
    
    /**
     *  Return suggested item to swap with in class @param class.
     */
    int localSwapProcedure(MMKPSolution solution,int classI);
};

class CompLocalSearch:public LocalSearch{
protected:
public:
    CompLocalSearch(MMKPDataSet dataSet);
    MMKPSolution run(MMKPSolution solution);
    MMKPSolution operator()(MMKPSolution solution);
    std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions);
    std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions);
};

class ReactiveLocalSearch:public LocalSearch{
protected:
    MMKPDataSet penalize(MMKPSolution solution,int delta, float pie);
public:
    ReactiveLocalSearch(MMKPDataSet dataSet);
    MMKPSolution run(MMKPSolution solution);
    MMKPSolution operator()(MMKPSolution solution);
    std::vector<MMKPSolution> run(std::vector<MMKPSolution> solutions);
    std::vector<MMKPSolution> operator()(std::vector<MMKPSolution> solutions);
};


#endif /* defined(____MMKP_LocalSearch__) */
