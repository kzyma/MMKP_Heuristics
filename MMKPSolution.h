/*********************************************************
 *
 * File: MMKPSolution.h
 * Author: Ken Zyma
 *
 * MMKP Solution representation.
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

#ifndef ____MMKPSolution__
#define ____MMKPSolution__

#include <iostream>
#include <vector>
#include <assert.h>
#include <iomanip>
#include <functional>

/**
 * Solution to MMKP problem consisting of class/item
 * selection, the summation of all classes' profit and
 * summation of all classes' cost's. An item of a given
 * class's inclusion in the solution is represented by
 * boolean true (1), and exlusion false (0). This class
 * does not maintain a responisbility to ensure feasibility
 * or automatically update profit and cost summation when 
 * item selection/deselection occurs.
 */
class MMKPSolution{
    
private:
    std::vector<std::vector<bool> > solution;
    float profitSummation;
    std::vector<float> costsSummation;
public:
    
    /**
     * Construct empty MMKPSolution. Uninitialized structure, only 
     * use for separate declaration and initialization.
     */
    MMKPSolution();
    /**
     * Construct MMKPSolution with class/item data initialized
     * to 0. Constraints are undefined.
     */
    MMKPSolution(std::vector<int> numberOfItemsPerClass);
    
    /**
     * Construct MMKPSolution with class/item data initialized
     * to 0. Constraints (costs) are initialized to 0 as well.
     */
    MMKPSolution(std::vector<int> numberOfItemsPerClass,
                 int numberOfConstraints);
    
    //operator overloading
    /**
     * Return a reference to a class of items. Usage is similar to
     * MMKPDataSet in class/item representation is similar to a
     * multi-dimensional array. ex: SolutionName[classNumber][itemNumber].
     * Again, indexing begins at 0.
     */
    std::vector<bool>& operator[](int index);
    
    /**
     * Compare two MMKPSolutions, equality based on the values of
     * a solutions. If two solutions have the same bit's 'set', they
     * are equal, else otherwise.
     */
    bool operator==(MMKPSolution &other) const;
    
    friend std::ostream& operator<<(std::ostream& os,MMKPSolution& obj);
    
    //accessors
    /**
     * Return number of class's in a MMKPSolution.
     */
    int size();
    
    /**
     * Return profit of a MMKPSolution. *note* this is not updated
     * automatically when a solution changes and must be updated using
     * setProfit. This desision was for efficiency.
     */
    float getProfit();
    
    /**
     * Return costs of constraints for MMKPSolution. *note* this is not 
     * updated automatically when a solution changes and must be updated
     * using setCosts. This desision was for efficiency.
     */
    std::vector<float> getCostsSummation();
    
    /**
     * Return cost summation for the cost contratint at corresponding index.
     */
    float getCost(int index);
    
    
    //mutators
    /**
     * Set cost constraints for a solution.
     */
    void setCosts(const std::vector<float> costsSummation);
    
    /**
     * Set a single cost constraint for solution given by param: index.
     */
    void setCost(const std::size_t index, float costSummation);
    
    /**
     * Set profit for a solution.
     */
    void setProfit(const float profitSummation);
};

std::ostream& operator<<(std::ostream& os,MMKPSolution& obj);

#endif /* defined(____MMKPSolution__) */
