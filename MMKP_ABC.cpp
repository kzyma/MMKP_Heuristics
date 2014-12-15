/*********************************************************
 *
 * File: MMKP_ABC.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Artificial Bee Colony implementation.
 *
 *********************************************************/

#include "MMKP_ABC.h"

/* MMKP_ABC */

//constructors
MMKP_ABC::MMKP_ABC(MMKPDataSet dataSet, ABC_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

//overloaded operators
MMKPSolution MMKP_ABC::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_ABC::run(initialPopulation);
}

MMKPSolution MMKP_ABC::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    //find best solution and return
    MMKPSolution bestSolution = population[0];
    bestSolution.setProfit(0);
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    this->currentFuncEvals = 0;

    //split population
    std::size_t const half_size = population.size() / 2;
    std::vector<MMKPSolution> empBees
    (population.begin(),population.begin() + half_size);
    std::vector<MMKPBeeSolution>employeedBees;
    
    for(int i=0;i<empBees.size();i++){
        MMKPBeeSolution temp;
        temp.solution = empBees[i];
        temp.noImproveCount = 0;
        employeedBees.push_back(temp);
    }
    
    std::vector<MMKPSolution> onLookerBees
    (population.begin() + half_size,population.end());
    
    //main loop
    while(!terminationCriterion){
        
        employeedBees = MMKP_ABC::employeedBeePhase(employeedBees);
        onLookerBees = MMKP_ABC::onLookerBeePhase(onLookerBees,employeedBees);
        currentFuncEvals += population.size();
        MMKP_ABC::scoutBeePhase(employeedBees);
        
        //see if we have a new best solution
        for(int i=0;i<employeedBees.size();i++){
            if(this->dataSet.isFeasible(employeedBees[i].solution)){
                if(employeedBees[i].solution.getProfit()
                   > bestSolution.getProfit()){
                    bestSolution = employeedBees[i].solution;
                    convergenceIteration = currentGeneration;
                }
            }
        }
        for(int i=0;i<onLookerBees.size();i++){
            if(this->dataSet.isFeasible(onLookerBees[i])){
                if(onLookerBees[i].getProfit()
                   > bestSolution.getProfit()){
                    bestSolution = onLookerBees[i];
                    this->convergenceIteration = currentGeneration;
                }
            }
        }
        
        std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
        this->convergenceData.push_back(temp);
        
        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        currentGeneration++;
    }
    
    for(int i=0;i<empBees.size();i++){
        empBees[i] = employeedBees[i].solution;
    }

    return bestSolution;
}

std::vector<MMKPSolution> MMKP_ABC::runOneGeneration
(std::vector<MMKPSolution> population){
    
    //split population
    std::size_t const half_size = population.size() / 2;
    std::vector<MMKPSolution> empBees
    (population.begin(),population.begin() + half_size);
    std::vector<MMKPBeeSolution>employeedBees;
    
    for(int i=0;i<empBees.size();i++){
        MMKPBeeSolution temp;
        temp.solution = empBees[i];
        temp.noImproveCount = 0;
        employeedBees.push_back(temp);
    }
    
    std::vector<MMKPSolution> onLookerBees(population.begin() + half_size,population.end());
        
    employeedBees = MMKP_ABC::employeedBeePhase(employeedBees);
    onLookerBees = MMKP_ABC::onLookerBeePhase(onLookerBees,employeedBees);
    MMKP_ABC::scoutBeePhase(employeedBees);
    
    for(int i=0;i<empBees.size();i++){
        empBees[i] = employeedBees[i].solution;
    }
    
    population.clear();
    population.reserve(employeedBees.size() + onLookerBees.size() );
    population.insert( population.end(), empBees.begin(), empBees.end());
    population.insert( population.end(), onLookerBees.begin(), onLookerBees.end());
    
    return population;
}

std::vector<MMKPBeeSolution> MMKP_ABC::employeedBeePhase
(std::vector<MMKPBeeSolution> employeedBees){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    
    for(int i=0;i<employeedBees.size();i++){
        
        MMKPSolution newSolution = employeedBees[i].solution;
        int tempI = rand() % employeedBees.size();
        MMKPSolution compareSols = employeedBees[tempI].solution;
        
        for(int j=0;j<newSolution.size();j++){
            for(int k=0;k<newSolution[j].size();k++){
                float p = dis(gen);
                int o_ij = 0;
                if(p < this->parameters.r){
                    o_ij = 1;
                }
                newSolution[j][k] = newSolution[j][k] ^
                    (o_ij&(newSolution[j][k] | compareSols[j][k]));
            }
        }
        
        MMKP_MetaHeuristic::makeFeasible(newSolution);
        if(newSolution.getProfit() > employeedBees[i].solution.getProfit()){
            employeedBees[i].solution = newSolution;
            employeedBees[i].noImproveCount = 0;
        }else{
            employeedBees[i].noImproveCount++;
        }
        
    }
    return employeedBees;
}

std::vector<MMKPSolution>
MMKP_ABC::onLookerBeePhase(std::vector<MMKPSolution> onLookerBees,
                 std::vector<MMKPBeeSolution> employeedBees){
    
    std::vector<MMKPSolution> empBees;
    float totalObjVal = 0;
    for(int i=0;i<employeedBees.size();i++){
        totalObjVal += employeedBees[i].solution.getProfit();
        empBees.push_back(employeedBees[i].solution);
    }
    MMKP_MetaHeuristic::quickSort(empBees,0,empBees.size()-1);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    
    for(int i=0;i<onLookerBees.size();i++){
        
        MMKPSolution newSolution = onLookerBees[i];
        //find index of solution to compare with, roulette style
        int tempI = 0;
        float accumulated = 0;
        float c = dis(gen);
        for(int j=0;j<empBees.size();j++){
            accumulated += empBees[j].getProfit() / totalObjVal;
            if (accumulated > c){
                tempI = j;
                break;
            }
        }
        MMKPSolution compareSols = empBees[tempI];
        
        for(int j=0;j<newSolution.size();j++){
            for(int k=0;k<newSolution[j].size();k++){
                float p = dis(gen);
                int o_ij = 0;
                if(p < this->parameters.r){
                    o_ij = 1;
                }
                newSolution[j][k] = newSolution[j][k] ^
                (o_ij&(newSolution[j][k] | compareSols[j][k]));
            }
        }
        MMKP_MetaHeuristic::makeFeasible(newSolution);
        if(newSolution.getProfit() > onLookerBees[i].getProfit()){
            onLookerBees[i] = newSolution;
        }
    }
    
    return onLookerBees;
}

void MMKP_ABC::scoutBeePhase(std::vector<MMKPBeeSolution>& employeedBees){
    
    for(int i=0;i<employeedBees.size();i++){
        if(employeedBees[i].noImproveCount > this->parameters.abandonmentCriterion){
            //this is a scout bee
            currentFuncEvals++;
            for(int j=0;j<employeedBees[i].solution.size();j++){
                //reset all to zero
                for(int k=0;k<employeedBees[i].solution[j].size();k++){
                    employeedBees[i].solution[j][k] = 0;
                }
                //mark one in every class..randomly
                int classI = rand() % employeedBees[i].solution.size();
                employeedBees[i].solution[j][classI] = true;
            }
            employeedBees[i].noImproveCount = 0;
            MMKP_MetaHeuristic::makeFeasible(employeedBees[i].solution);
        }
    }
}



