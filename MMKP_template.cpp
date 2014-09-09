/*********************************************************
 *
 * File: MMKP_ACO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * BBA MMKP Problem
 *
 *********************************************************/

#include "MMKP_ACO.h"

/* MMKP_ACO */

//constructors
MMKP_ACO::MMKP_ACO(MMKPDataSet dataSet, BBA_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

//overloaded operators
MMKPSolution MMKP_ACO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_ACO::run(initialPopulation);
}

MMKPSolution MMKP_ACO::run(std::vector<MMKPSolution> initialPopulation){
    
    //init initial parameters
    //init MMKPBatSolution vector
    std::vector<MMKPBatSolution> population(initialPopulation.size());
    for(int i=0;i<initialPopulation.size();i++){
        population[i].solution = initialPopulation[i];
    }
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    
    //main loop
    while(!terminationCriterion){
        
        
        

        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        currentGeneration++;
    }
    
    MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
    //return best, feasible solution
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            return population[i];
        }
    }
    return population[0];
}
