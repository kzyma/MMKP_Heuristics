/*********************************************************
 *
 * File: MMKP_BBA.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Binary Bat implementation.
 *
 *********************************************************/

#include "MMKP_BBA.h"

/* MMKP_BBA */

//constructors
MMKP_BBA::MMKP_BBA(MMKPDataSet dataSet, BBA_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){}

//overloaded operators
MMKPSolution MMKP_BBA::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_BBA::run(initialPopulation);
}

void MMKP_BBA::quickSort(std::vector<MMKPBatSolution>& input,int p, int r){
    if ( p < r )
    {
        int j = partition(input, p, r);
        MMKP_BBA::quickSort(input, p, j-1);
        MMKP_BBA::quickSort(input, j+1, r);
    }
}

int MMKP_BBA::partition(std::vector<MMKPBatSolution>& input,int p, int r){
    //partition will sort greatest->least
    int pivot = input[r].solution.getProfit();
    while ( p < r )
    {
        while ( input[p].solution.getProfit() > pivot )
            p++;
        
        while ( input[r].solution.getProfit() < pivot )
            r--;
        
        if ( input[p].solution.getProfit() == input[r].solution.getProfit() )
            p++;
        
        else if (p < r )
        {
            MMKPSolution tmp = input[p].solution;
            input[p].solution = input[r].solution;
            input[r].solution = tmp;
        }
    }
    
    return r;
}

MMKPSolution MMKP_BBA::run(std::vector<MMKPSolution> initialPopulation){
    
    //init initial parameters
    //init MMKPBatSolution vector
    std::vector<MMKPBatSolution> population(initialPopulation.size());
    for(int i=0;i<initialPopulation.size();i++){
        population[i].solution = initialPopulation[i];
    }
    MMKP_BBA::initBatParemeters(population);
    
    this->currentGeneration = 0;
    bool terminationCriterion = false;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    currentFuncEvals = 0;
    
    MMKP_BBA::quickSort(population,0,(population.size()-1));
    MMKPSolution bestSolution;
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i].solution)){
            bestSolution =  population[i].solution;
            break;
        }
    }
    
    std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
    this->convergenceData.push_back(temp);
    
    //main loop
    while(!terminationCriterion){
        
        MMKP_BBA::globalSearch(population);
        MMKP_BBA::quickSort(population,0,(population.size()-1));
        
        for(int i=0;i<population.size();i++){
            if(this->dataSet.isFeasible(population[i].solution)){
                if(population[i].solution.getProfit() > bestSolution.getProfit()){
                    bestSolution =  population[i].solution;
                    this->convergenceIteration = currentGeneration;
                    break;
                }
            }
        }

        std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
        this->convergenceData.push_back(temp);
        
        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        this->currentGeneration++;
    }
    
    return bestSolution;
}

std::vector<MMKPSolution> MMKP_BBA::runOneGeneration
(std::vector<MMKPSolution> population){
    
    //init initial parameters
    //init MMKPBatSolution vector
    std::vector<MMKPBatSolution> tempPopulation(population.size());
    for(int i=0;i<population.size();i++){
        tempPopulation[i].solution = population[i];
    }
    MMKP_BBA::quickSort(tempPopulation,0,(population.size()-1));
    MMKP_BBA::initBatParemeters(tempPopulation);
    MMKP_BBA::globalSearch(tempPopulation);
    
    std::vector<MMKPSolution> newPopulation(population.size());
    for(int i=0;i<population.size();i++){
        newPopulation[i] = tempPopulation[i].solution;
    }
    
    return newPopulation;
}

void MMKP_BBA::initBatParemeters(std::vector<MMKPBatSolution>& population){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for(int i=0;i<population.size();i++){
        std::uniform_real_distribution<> dif(0, 1);
        std::uniform_real_distribution<> div(-1, 1);
        std::uniform_real_distribution<> dil(1, 2);
        
        //init frequency
        float f1;
        float f2;
        do{
            f1 = dif(gen);
            f2 = dif(gen);
        }while(f1 == f2);
        
        if(f1 > f2){
            population[i].fmax = f1;
            population[i].fmin = f2;
        }else{
            population[i].fmax = f2;
            population[i].fmin = f1;
        }
        
        //init velocity
        population[i].v = div(gen);
        
        //init rate of pulse
        population[i].r = dif(gen);
        population[i].r_0 =dif(gen);
        
        //init loudness
        population[i].a = dif(gen);
    }
}

void MMKP_BBA::globalSearch(std::vector<MMKPBatSolution>& population){
    //already in sorted order in this implementation.
    //MMKP_BBA::quickSort(population,0,(population.size()-1));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dib(0, 1);
    
    MMKPBatSolution bestBat = population[0];
    
    ReactiveLocalSearch RLS(dataSet);
    bestBat.solution = RLS(bestBat.solution);
    this->currentFuncEvals += RLS.getFuncEvals();
    
    for(int i=0;i<population.size();i++){
        
        MMKPBatSolution currentBat = population[i];
        
        //equation 1
        float B = dib(gen);
        float fTemp = currentBat.fmax + ((currentBat.fmax
                                          -currentBat.fmin)*B);
        
        //equation 2
        float averageV = 0.0;
        int averageVSize = 0;
        for(int j=0;j<currentBat.solution.size();j++){
            for(int k=0;k<currentBat.solution[j].size();k++){
                this->currentFuncEvals++;
                float vTemp = currentBat.v + ((currentBat.solution[j][k]
                                               -bestBat.solution[j][k])*fTemp);
                averageV += vTemp;
                averageVSize++;
                
                //equation "3"--binary uses 7 and 8
                float normalizedV = 1/(1+(exp(-(vTemp))));
                float r = dib(gen);
                
                if(r <= normalizedV){
                    currentBat.solution[j][k] = 1;
                }else{
                    currentBat.solution[j][k] = 0;
                }
            }
        }
        currentBat.v = (averageV/averageVSize);
        MMKP_MetaHeuristic::makeFeasible(currentBat.solution);
        
        if(dib(gen) > currentBat.r){
            int localSolutionIndex = rand() % 10;
            MMKPBatSolution bestBat = population[localSolutionIndex];
            MMKP_BBA::localSearch(bestBat,currentBat);
        }else{
            MMKP_BBA::Mutate(currentBat);
        }
        //MMKP_BBA::randomSearch(currentBat);
        MMKP_BBA::competitiveUpdateSol(population[i],currentBat);
    }
}

void MMKP_BBA::competitiveUpdateSol
(MMKPBatSolution& sol, MMKPBatSolution& newSol){
    
    dataSet.updateSolution(sol.solution);
    dataSet.updateSolution(newSol.solution);
    
    bool solFeas = this->dataSet.isFeasible(sol.solution);
    bool newFeas = this->dataSet.isFeasible(newSol.solution);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dib(0, 1);
    
    if(dib(gen) < sol.a){
        //if both feasible, choose best profit
        if(newFeas && solFeas){
            if((newSol.solution).getProfit() > (sol.solution).getProfit()){
                sol = newSol;
                sol.a = sol.a - (sol.a * .05);
                sol.r = sol.r + (sol.r * .05);
            }
            //if one is feasible, that automatically is choosen
        }else if((!solFeas) && newFeas){
            sol.a = sol.a - (sol.a * .05);
            sol.r = sol.r + (sol.r * .05);
            sol = newSol;
            //if neither is feasible, choose greatest profit.
        }else if ((!solFeas && !newFeas)&&(newSol.solution.getProfit()
                                           > sol.solution.getProfit())){
            sol = newSol;
        }
    }
}

void MMKP_BBA::localSearch(MMKPBatSolution& bestBat,MMKPBatSolution& solution){
        
    //already in sorted order in this implementation.
    //MMKP_BBA::quickSort(population,0,(population.size()-1));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dib(0, 1);
    float r_1 = dib(gen);
    
    //find P1 and P2
    int P1 = rand() % solution.solution.size();
    int P2 = (rand() % (solution.solution.size()-P1)) + (P1);

    //equation 1
    float B = dib(gen);
    float fTemp = solution.fmax + ((solution.fmax
                                      -solution.fmin)*B);
    
    //equation 2
    float averageV = 0.0;
    int averageVSize = 0;
    for(int j=P1;j<P2;j++){
        for(int k=0;k<solution.solution[j].size();k++){
            this->currentFuncEvals++;
            float vTemp = solution.v + ((solution.solution[j][k]
                                           -bestBat.solution[j][k])*fTemp);
            averageV += vTemp;
            averageVSize++;
            
            //equation "3"--binary uses 7 and 8
            float normalizedV = 1/(1+(exp(-(vTemp))));
            float r = dib(gen);
            
            if(r <= normalizedV){
                solution.solution[j][k] = 1;
            }else{
                solution.solution[j][k] = 0;
            }
            
        }
        solution.v = (averageV/averageVSize);
        MMKP_MetaHeuristic::makeFeasible(solution.solution);
    }
}

void MMKP_BBA::randomSearch(MMKPBatSolution& sol){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dib(0, 1);
    
    //generate new random solution
    MMKPSolution newSol = sol.solution;
    for(int i=0;i<newSol.size();i++){
        int randomIndx = rand() % newSol[i].size();
        newSol[i].at(randomIndx) = true;
    }
    MMKP_MetaHeuristic::makeFeasible(newSol);
    
    //update sol
    float r_1 = dib(gen);
    if(r_1 < sol.a){
        bool updateAandR = false;
        float rateOfChange = .85;
        
        dataSet.updateSolution(sol.solution);
        dataSet.updateSolution(newSol);
        
        bool solFeas = this->dataSet.isFeasible(sol.solution);
        bool newFeas = this->dataSet.isFeasible(newSol);
        
        //if both feasible, choose best profit
        if(newFeas && solFeas){
            if((newSol).getProfit() > (sol.solution).getProfit()){
                sol.solution = newSol;
                updateAandR = true;
            }
            //if one is feasible, that automatically is choosen
        }else if((!solFeas) && newFeas){
            sol.solution = newSol;
            updateAandR = true;
            //if neither is feasible, choose greatest profit.
        }else if ((!solFeas && !newFeas)&&(newSol.getProfit()
                                           > sol.solution.getProfit())){
            sol.solution = newSol;
            updateAandR = true;
        }
        if(updateAandR){
            sol.r = sol.r_0 * (1.0-(exp(-(this->currentGeneration*rateOfChange))));
            sol.a = sol.a * rateOfChange;
        }
    }
}

void MMKP_BBA::Mutate(MMKPBatSolution& sol){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dif(0, 1);
    
    for(int i=0;i<sol.solution.size();i++){
        for(int j=0;j<sol.solution[i].size();j++){
            this->currentFuncEvals++;
            float r = dif(gen);
            if(r < .05){
                if(sol.solution[i][j] == 0){
                    sol.solution[i][j] = 1;
                }else{
                    sol.solution[i][j] = 0;
                }
            }
        }
    }
}

