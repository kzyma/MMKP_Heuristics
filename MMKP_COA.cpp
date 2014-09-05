/*********************************************************
 *
 * File: MMKP_COA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * COA MMKP Problem
 *
 *********************************************************/

#include "MMKP_COA.h"

/* MMKP_COA */

//constructors
MMKP_COA::MMKP_COA(MMKPDataSet dataSet, COA_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

//overloaded operators
MMKPSolution MMKP_COA::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_COA::run(initialPopulation);
}

MMKPSolution MMKP_COA::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    
    //quickSort(population,0,(population.size()-1));
    
    //main loop
    while(!terminationCriterion){
        
        MMKP_COA::horizontalCrossover(population);
        MMKP_COA::verticalCrossover(population);

        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        currentGeneration++;
    }
    //return best, feasible solution
    quickSort(population,0,(population.size()-1));
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            return population[i];
        }
    }
    return population[0];
}

void MMKP_COA::horizontalCrossover(std::vector<MMKPSolution>& population){
    
    int M = population.size();
    
    //Get B, a random permuation of M.
    int temp[M];
    for(int i=0;i<M;i++){
        temp[i] = i;
    }
    std::random_shuffle(temp,temp+M);
    
    for(int i=0;i<(M/2);i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        float p = dis(gen);
        
        if (p<this->parameters.horizCrossProb){
            float no1 = temp[2*i];
            float no2 = temp[2*i+1];
            int D = population[no1].size();
            
            //our two solutions
            MMKPSolution Xi = population[no1];
            MMKPSolution Xj = population[no2];
            
            for(int j=0;j<D;j++){
                for(int k=0;k<population[no1][j].size();k++){
                    //generate uniform number
                    std::uniform_int_distribution<> rs(0, 1);
                    std::uniform_int_distribution<> cs(-1, 1);
                    int r1 = rs(gen);
                    int r2 = rs(gen);
                    int c1 = cs(gen);
                    int c2 = cs(gen);
                    
                    //functions to create new solutions
                    int tempXi =
                    r1*Xi[j][k]+(1-r1)*Xj[j][k]+c1*(Xi[j][k]-Xj[j][k]);
                    int tempXj =
                    r2*Xj[j][k]+(1-r2)*Xi[j][k]+c2*(Xi[j][k]-Xj[j][k]);
                    if(tempXi<1){
                        Xi[j][k] = 0;
                    }else{
                        Xi[j][k] = 1;
                    }
                    if(tempXj<1){
                        Xj[j][k] = 0;
                    }else{
                        Xj[j][k] = 1;
                    }
                }
            }
            //regain feasibility in new solutions
            MMKP_MetaHeuristic::makeFeasible(Xi);
            MMKP_MetaHeuristic::makeFeasible(Xj);
            
            //run competetive operator
            MMKP_MetaHeuristic::competitiveUpdateSol(population[no1],Xi);
            MMKP_MetaHeuristic::competitiveUpdateSol(population[no2],Xj);
        }
    }
    
}

void MMKP_COA::verticalCrossover(std::vector<MMKPSolution>& population){
    
    int M = population.size();
    int D = population[0].size();
    
    //Get B, a random permuation of M.
    int temp[D];
    for(int i=0;i<D;i++){
        temp[i] = i;
    }
    std::random_shuffle(temp,temp+D);
    
    for(int i=0;i<(D/2);i++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);
        float p = dis(gen);
        
        if (p<this->parameters.verticalCrossProb){
            
            int no1 = temp[2*i];
            int no2 = temp[2*i+1];
            
            for(int j=0;j<M;j++){
                
                std::uniform_int_distribution<> rs(0, 1);
                int r = rs(gen);
                
                MMKPSolution Xi = population[j];
                
                for(int k=0;k<population[j][no1].size();k++){
                    int tempXi =r*Xi[no1][k]+(1-r)*Xi[no2][k];
                    
                    if(tempXi<1){
                        Xi[no1][k] = 0;
                    }else{
                        Xi[no1][k] = 1;
                    }
                }
                
                //regain feasibility in new solutions
                MMKP_MetaHeuristic::makeFeasible(Xi);
                
                //run competetive operator
                MMKP_MetaHeuristic::competitiveUpdateSol(population[j],Xi);
            }
        }
    }
    
}
