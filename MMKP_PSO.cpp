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

    std::vector<Particle> particles;
    for(int i=0;i<population.size();i++){
        Particle temp;
        temp.solution = population[i];
        temp.localBest = population[i];
        for(int j=0;j<temp.solution.size();j++){
            std::vector<float> tempV;
            for(int k=0;k<temp.solution[j].size();k++){
                float v = (rand() % 4) - 2;
                tempV.push_back(v);
            }
            temp.v.push_back(tempV);
        }
        particles.push_back(temp);
    }
    
    std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
    this->convergenceData.push_back(temp);
    
    //main loop
    while(!terminationCriterion){
        
        particleUpdate(particles,bestSolution);
        particleUpdate(particles,bestSolution);
        
        std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
        this->convergenceData.push_back(temp);
        
        if(currentGeneration >= (this->parameters.numberOfGenerations)){
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

void MMKP_PSO::particleUpdate(std::vector<Particle>& particles,MMKPSolution& bestSolution){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dib(0, 1);
    
    for(int i=0;i<particles.size();i++){

        MMKPSolution oldSolution = particles[i].solution;
        
        for (int j=0;j<particles[i].solution.size();j++){
            for(int k=0;k<particles[i].solution[j].size();k++){

                this->currentFuncEvals++;
                //update velocity
                particles[i].v[j][k] = particles[i].v[j][k] +
                (this->parameters.learningFactor *
                 (particles[i].localBest[j][k] - particles[i].solution[j][k])) +
                (this->parameters.learningFactor *
                 (bestSolution[j][k] - particles[i].solution[j][k]));
                
                //normalize velocity- between -2 and 2
                if(particles[i].v[j][k] > 2.0){
                    particles[i].v[j][k] = 2.0;
                }
                if(particles[i].v[j][k] < -2.0){
                    particles[i].v[j][k] = -2.0;
                }

                float normalizedV = 1/(1+(exp(-(particles[i].v[j][k]))));
                float r = dib(gen);
                
                //update position
                if(r <= normalizedV){
                    particles[i].solution[j][k] = 1;
                }else{
                    particles[i].solution[j][k] = 0;
                }
            }
        }
        
        bool temp = MMKP_MetaHeuristic::makeFeasible(particles[i].solution);
        
        //check if new particles is better than local best
        if ((particles[i].solution.getProfit() > particles[i].localBest.getProfit())&&
            (temp)){
            particles[i].localBest = particles[i].solution;
        }
    }
    
    //check if any new particles are better than global best
    for(int i=0;i<particles.size();i++){
            if(particles[i].localBest.getProfit() > bestSolution.getProfit()){
                bestSolution = particles[i].localBest;
                this->convergenceIteration = currentGeneration;
            }
    }
}



