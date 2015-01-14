/*********************************************************
 *
 * File: MMKP_PSO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Genetic Algorithm Implementation.
 *
 *********************************************************/

#include "MMKP_PSO.h"

/* MMKP_PSO */

//constructors
MMKP_PSO::MMKP_PSO(MMKPDataSet dataSet, PSO_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

MMKP_PSO::MMKP_PSO(MMKPDataSet dataSet)
:MMKP_MetaHeuristic(dataSet),parameters(){}

//overloaded operators
MMKPSolution MMKP_PSO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_PSO::run(initialPopulation);
}

MMKPSolution MMKP_PSO::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    this->currentFuncEvals = 0;
    
    MMKPSolution bestSolution;
    bestSolution.setProfit(0);
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            if(population[i].getProfit() > bestSolution.getProfit()){
                bestSolution = population[i];
                this->convergenceIteration = currentGeneration;
            }
        }
    }
    
    //main loop
    while(!terminationCriterion){
        
        //this is where specific code for PSO goes.
        
        /*make sure in your function which executes the equations for PSO,
         *that you do this->currentFuncEvals += 1 for every iteration of
         *the loop. This way the function evaluations will be output correctly
         *in the excel spreadsheet.
         *...
         *So, for example
         *FOR every class in a solution:
         *    FOR every item in that class:
         *         this->currentFuncEvals +=1;
         *          DO some equation on the solution to create a new solution.
         */
        
        /*
         *another note. For the values of c_1 and c_2 in the equations in the
         *papers use this->parameters.learningFactor. I made this an parameter
         *that is passed to the program, for ease of trying other values.
         *You can change this by going to the file "PsoSettings" under the
         *build directory and changing it to whatever value you choose.
         */
        
        /*
         * Also at some point you will need to implement a local search
         * into PSO. The following is how you will set this up...but do not
         * worry about this right now.
         *
         * ReactiveLocalSearch RLS(dataSet);
         * solution = RLS(solution);
         * this->currentFuncEvals += RLS.getFuncEvals();
         */
        
        
        for(int i=0;i<population.size();i++){
            if(this->dataSet.isFeasible(population[i])){
                if(population[i].getProfit() > bestSolution.getProfit()){
                    bestSolution = population[i];
                    this->convergenceIteration = currentGeneration;
                }
            }
        }
        
        if((currentGeneration % 2 == 0)){
            std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
            this->convergenceData.push_back(temp);
        }
        
        if(currentGeneration >= (this->parameters.numberOfGenerations*2)){
            terminationCriterion = true;
        }
        
        currentGeneration++;
    }
    
    return bestSolution;
}

std::vector<MMKPSolution> MMKP_PSO::runOneGeneration
(std::vector<MMKPSolution> population){

    

    return population;
}



