/*********************************************************
 *
 * File: MMKP_GA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * COA MMKP Problem
 *
 *********************************************************/

#include "MMKP_GA.h"

/* MMKP_GA */

//constructors
MMKP_GA::MMKP_GA(MMKPDataSet dataSet, GA_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

MMKP_GA::MMKP_GA(MMKPDataSet dataSet)
:MMKP_MetaHeuristic(dataSet),parameters(){}

//overloaded operators
MMKPSolution MMKP_GA::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_GA::run(initialPopulation);
}

MMKPSolution MMKP_GA::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    currentFuncEvals = 0;
    
    quickSort(population,0,(population.size()-1));

    
    //main loop
    while(!terminationCriterion){
        
        MMKPSolution p1;
        MMKPSolution p2;
        MMKP_GA::parentSelection(p1,p2,population);
        MMKPSolution newSol = MMKP_GA::Crossover(p1,p2);
        MMKP_GA::Mutate(newSol);
        
        float feas = MMKP_GA::makeFeasible(newSol);
        if(feas){
            population[(population.size()-1)] = newSol;
        }
        
        MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
        
        for(int i=0;i<population.size();i++){
            if(this->dataSet.isFeasible(population[i])){
                if(population[i].getProfit() > this->bestSolution.getProfit()){
                    this->bestSolution = population[i];
                    this->convergenceIteration = (currentGeneration/population.size());
                }
            }
        }
        
        this->currentFuncEvals += 1;
        if(currentGeneration%population.size() == 0){
            std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
            this->convergenceData.push_back(temp);
        }
        
        if(currentGeneration >=
           (this->parameters.numberOfGenerations*population.size())){
            terminationCriterion = true;
        }
        
        currentGeneration++;
    }
    
    return this->bestSolution;
}

std::vector<MMKPSolution> MMKP_GA::runOneGeneration
(std::vector<MMKPSolution> population){

    quickSort(population,0,(population.size()-1));
    
    //main loop
    for(int i=0;i<population.size();i++){
        MMKPSolution p1;
        MMKPSolution p2;
        MMKP_GA::parentSelection(p1,p2,population);
        MMKPSolution newSol = MMKP_GA::Crossover(p1,p2);
        MMKP_GA::Mutate(newSol);
        
        float feas = MMKP_GA::makeFeasible(newSol);
        if(feas){
            population[(population.size()-1)] = newSol;
        }
        
        MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
        
    }
    return population;
}

void MMKP_GA::parentSelection
(MMKPSolution& p1, MMKPSolution& p2, std::vector<MMKPSolution>& population){
    
    int highP1Index = 0;
    float highP1 = 0;
    
    int highP2Index = 0;
    float highP2 = 0;
    
    //create 2 pools
    for(int i=0;i<this->parameters.parentPoolSizeT;i++){
        int pool1 = rand() % population.size();
        int pool2 = rand() % population.size();
        
        if(population[pool1].getProfit() > highP1){
            highP1 = population[pool1].getProfit();
            highP1Index = pool1;
        }
        if(population[pool2].getProfit() > highP2){
            highP2 = population[pool2].getProfit();
            highP2Index = pool2;
        }
    }
    
    p1 = population[highP1Index];
    p2 = population[highP2Index];
    
}

MMKPSolution MMKP_GA::Crossover(MMKPSolution p1,MMKPSolution p2){
    MMKPSolution newSol= p1;
    
    for(int i=0;i<p1.size();i++){
        for(int j=0;j<p1[i].size();j++){
            int r = rand() % 2;
            if(r != 0){
                newSol[i][j] = p2[i][j];
            }//if r == 0, p1 bit's are already copied to newSol.
        }
    }
    return newSol;
}

void MMKP_GA::Mutate(MMKPSolution& solution){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dif(0, 1);
    
    for(int i=0;i<solution.size();i++){
        for(int j=0;j<solution[i].size();j++){
            float r = dif(gen);
            if(r < this->parameters.mutateProb){
                if(solution[i][j] == 0){
                    solution[i][j] = 1;
                }else{
                    solution[i][j] = 0;
                }
            }
        }
    }
}


