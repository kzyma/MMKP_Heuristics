/*********************************************************
 *
 * File: MMKPDataSet.h
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 * 
 * TLBO MMKP Problem
 *
 *********************************************************/

#ifndef _MMKPDataSet_h
#define _MMKPDataSet_h

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <assert.h>

#include "MMKPSolution.h"

/**
 * Item in MMKP, which consists of a profit and cost
 * for each constraint.
 */
class ItemData{

private:
    float profit;
    std::vector<float> costs;
    float profitConstraintUsePercent;
    float constraintUsePercentSum;
    std::vector<float> constraintUsePercent;
    std::vector<float> profitConstraintRatio;
public:
    /**
     * Construct empty ItemData.
     */
    ItemData();
    
    /**
     * Construct ItemData. Param: ProblemResources used
     * in analytic functions, can also use setAnalytics
     * instead of this constructor.
     */
    ItemData(float profit, std::vector<float> costs,
             std::vector<float> problemResources);
    
    //accessors
    /**
     * Return profit of ItemData.
     */
    float getProfit() const;
    
    /**
     * Return costs of constraints for ItemData.
     */
    std::vector<float> getCosts() const;
    
    /**
     * Return cost of a constraint for ItemData. Parameter
     * index corresponds to the cost of one constraint, 
     * starting a index 0.
     */
    float getCost(int index) const;
    
    /**
     * Return the number of constraints for ItemData.
     */
    int getNumberOfCosts() const;
    
    /**
     * Return v/[Er%/n] for a ItemData.
     */
    float getProfitConstraintUsePercent() const;
    
    /**
     * Return v/[Er%/n] for a ItemData, using 
     * only certain indices.
     */
    float getProfitConstraintUsePercent(std::vector<int> indices) const;
    
    /**
     * Return Er%/n for ItemData.
     */
    float getConstraintUsePercent() const;
    
    /*
     * Return Er%/n for one constraint of an ItemData.
     */
    float getConstraintUsePercent(int index);
    
    /*
     * Return v/[Er%/n] for one constraint of an ItemData.
     */
    float getProfitConstraintUsePercent(int index);
    
    /*
     * Return Er%/n for select constraints of an ItemData.
     * Param: indices are indices to get summation, order
     * does not matter.
     */
    float getConstraintUsePercent(std::vector<int> indices);
    
    //mutators
    /**
     * Set profit for an ItemData.
     */
    void setProfit(const float profit);
    
    /**
     * Set Costs of all constraints for an ItemData. This
     * will override all costs previously set.
     */
    void setCosts(const std::vector<float> costs);
    
    /**
     * Set all ItemData analytic measures. This includes
     * Er%/n and v/[Er%/n] values.
     */
    void setAnalytics(const std::vector<float> problemResources);
};

//non-member functions
std::ostream& operator<<(std::ostream& os, const ItemData& obj);


/**
 * MMKP data consisting of classes with items (class: ItemData) 
 * and available resources.
 */
class MMKPDataSet{

private:
    std::vector<std::vector<ItemData> > classList;
    std::vector<float> resources;
    std::vector<int> numberOfItemsInClasses;
public:
    /**
     * Construct empty MMKPDataSet.
     */
    MMKPDataSet();
    
    /**
     * Construct MMKPDataSet and set size of class, 
     * items in each class, and resource constraints.
     */
    MMKPDataSet(std::vector<int> numberOfItemsInClasses,
                std::vector<float> constraints);
    
    //operator overloading
    /**
     * Return reference to a class of items (vector<ItemData>). 
     * Usage is similar to that of a multi-dimensional array:
     * DataSetName[classIndex][ItemIndex]
     */
    std::vector<ItemData>& operator[](int index);
    
    friend std::ostream& operator<<(std::ostream& os, MMKPDataSet& obj);
    
    //accessors
    /**
     * Return number of classes.
     */
    std::size_t size();
    
    /**
     * Return vector containing the number of items in each class.
     */
    std::vector<int> getSizeOfEachClass();
    
    /**
     * Return the number of problem constraints.
     */
    int getNumberOfResources();
    
    /**
     * Return vector containing resource constraints.
     */
    std::vector<float> getResources();
    
    /**
     * Return one resource
     */
    float getResource(int index);
    
    //mutators
    /**
     * Set problem resource constraints.
     */
    void setResources(const std::vector<float> resources);
    
    //solution helpers
    /**
     * Return true if param: solution is feasible, false
     * otherwise.
     */
    bool isFeasible(MMKPSolution solution)const;
    
    /**
     * Return indices (if any) corresponding to resources that have
     * been over-used.
     */
    std::vector<int> getOffendingIndices(MMKPSolution solution) const;
    
    /**
     * Return true if param: solution is feasible by
     * multiple-choice constraints.
     */
    bool isMultiChoiceFeasible(MMKPSolution solution)const;
    
    /**
     * Return true if param: solution is feasible by
     * multiple-dimension constaints.
     */
    bool isMultiDimFeasible(MMKPSolution solution)const;
    
    /**
     * Update solution analytics.
     */
    void updateSolution(MMKPSolution& solution);
    
    /**
     * Return the percent difference of contraint use, comparing
     * a solution and resources. Calculated using only the constraints
     * which are violated in a solution. Additionally, the input solution
     * must be multiple-choice feasible.
     */
    float getVariableConstraintUseDiff(MMKPSolution& solution);
};

//non-member functions
std::ostream& operator<<(std::ostream& os, const MMKPDataSet& obj);


/**
 * OrLib_Read Function Object reads from orLib_data and
 * converts to common format MMKPDataSet. Data can
 * be found at ftp://cermsem.univ-paris1.fr/pub/CERMSEM/hifi/MMKP/ .
 */
class OrLib_Read{
    
public:
    /**
     * Convert input file to MMKPDataSet.
     */
    MMKPDataSet operator()(std::ifstream& file);
};

/**
 * HiremathHill Function Object reads from HiremathHill problem
 * sets and converts to common format MMKPDataSet.
 */
class HiremathHill_Read{
    
public:
    /**
     * Convert input file to MMKPDataSet. Since each file in the
     * Hiremath/Hill problem set has multuple problems, a problem
     * to run may be specified (starting at index 1).
     */
    MMKPDataSet operator()(std::ifstream& file, int problemToRun);
};


#endif



