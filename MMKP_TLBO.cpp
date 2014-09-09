/*********************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Problem
 *
 *********************************************************/

#include "MMKP_TLBO.h"

/* MMKP_TLBO */

//constructors
MMKP_TLBO::MMKP_TLBO(MMKPDataSet dataSet, TLBO_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){
}

//overloaded operators
MMKPSolution MMKP_TLBO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_TLBO::run(initialPopulation);
}

MMKPSolution MMKP_TLBO::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    
    MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
    
    //main loop
    while(!terminationCriterion){
        
        if(this->parameters.classroomSize == 0){
            MMKP_TLBO::teachingPhase(population);
        }else{
            int size = this->parameters.classroomSize;
            MMKP_TLBO::teachingPhase_MultiTeacher(population,size);
        }
        MMKP_TLBO::learningPhase(population);
        
        MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
        
        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        currentGeneration++;
    }
    //return best, feasible solution
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            return population[i];
        }
    }
    return population[0];
}

void MMKP_TLBO::teachingPhase(std::vector<MMKPSolution>& population){
    int teacherIndex = 0;
    if(parameters.addRandomTeacher){
        teacherIndex = rand() % ((int)(population.size()*.10));
    }
    
    MMKPSolution* teacher = &population[teacherIndex];
    MMKPSolution* mean = &population[(population.size()/2)];
    
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;
        if(currentSol == teacher){continue;}
        
        //step 1: convert bits with -2,-1 to 0, and 1 otherwise
        for(int j=0;j<(*currentSol).size();j++){
            for(int k=0;k<(*currentSol)[j].size();k++){
                int T_f = rand()%2+1;
                int diff = ceil((*currentSol)[j].at(k)+((*teacher)[j].at(k)
                                    - (T_f * (*mean)[j].at(k))));
                if(diff<=0){
                    (tempSol)[j].at(k) = false;
                }else{
                    (tempSol)[j].at(k) = true;
                }
            }
        }
        
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                int classSize){
    
    int start = 0;
    int stop = -1;
    
    if(classSize < this->parameters.populationSize){
        stop = (start+classSize)-1;
    }else{
        stop = this->parameters.populationSize - 1;
    }
    
    while((start < stop) && (stop < this->parameters.populationSize)){
        
        for(int i=start;i<stop;i++){
            
            MMKPSolution* teacher = &population[start];
            MMKPSolution* mean = &population[((start+stop)/2)];
            
            MMKPSolution* currentSol = &population[i];
            MMKPSolution tempSol = *currentSol;
            if(currentSol == teacher){continue;}
            
            //step 1: convert bits with -2,-1 to 0, and 1 otherwise
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int T_f = rand()%2+1;
                    int diff = ceil((*currentSol)[j].at(k)+((*teacher)[j].at(k)
                                                            - (T_f * (*mean)[j].at(k))));
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                }
            }
            dataSet.updateSolution(tempSol);
            MMKP_MetaHeuristic::makeFeasible(tempSol);
            MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
            
        }//end for(int i...
        
        //update start and stop
        start += classSize;
        if(classSize < this->parameters.populationSize){
            stop = (start+classSize)-1;
        }else{
            stop = this->parameters.populationSize - 1;
        }
        
    }//end while(stop....
}

void MMKP_TLBO::teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                             int iteration){
    MMKPSolution* teacher = &population[0];
    MMKPSolution* mean = &population[(population.size()/2)];
    
    float wMax = 1.0;
    float wMin = .5;
    float weight = wMax -
        (((wMax - wMin)/parameters.numberOfGenerations) * iteration);
    
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;
        if(currentSol == teacher){continue;}
        
        //step 1: convert bits with -2,-1 to 0, and 1 otherwise
        for(int j=0;j<(*currentSol).size();j++){
            for(int k=0;k<(*currentSol)[j].size();k++){
                int T_f = rand()%2+1;
                int diff = ceil(weight*((*currentSol)[j].at(k)+((*teacher)[j].at(k)
                                                        - (T_f * (*mean)[j].at(k)))));
                if(diff<=0){
                    (tempSol)[j].at(k) = false;
                }else{
                    (tempSol)[j].at(k) = true;
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::RandomSelectPathRelink
(std::vector<MMKPSolution>& population,float mutateRate){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dit(0, population.size()-1);
    
    for(int i=0;i<population.size();i++){
      
        MMKPSolution tempSol = population[i];
        int r = dit(gen);
        MMKPSolution randomSol = population[r];
        
        if(tempSol.getProfit()<randomSol.getProfit()){
            for(int j=0;j<population[i].size();j++){
                for(int k=0;k<population[i][j].size();k++){
                    float p = dis(gen);
                    if (p<mutateRate){
                        tempSol[j][k] = randomSol[j][k];
                    }
                }
            }
            dataSet.updateSolution(tempSol);
            MMKP_MetaHeuristic::makeFeasible(tempSol);
            MMKP_MetaHeuristic::competitiveUpdateSol(population[i],tempSol);
        }
    }
}

void MMKP_TLBO::RandomizedMutation
(std::vector<MMKPSolution>& population,float mutateRate){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> dit(0, population.size()-1);
    
    for(int i=0;i<population.size();i++){
        
        MMKPSolution tempSol = population[i];
    
        for(int j=0;j<population[i].size();j++){
            for(int k=0;k<population[i][j].size();k++){
                float p = dis(gen);
                if (p<mutateRate){
                    if(tempSol[j][k] == 0){
                        tempSol[j][k] = 1;
                    }else{
                        tempSol[j][k] = 0;
                    }
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(population[i],tempSol);
    }
}

void MMKP_TLBO::learningPhase(std::vector<MMKPSolution>& population){
    
    quickSort(population,0,(population.size()-1));
    
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;
        int randSol = rand() % population.size();
        MMKPSolution* otherSol = &population[randSol];
        
        //if they are the same, skip
        if(currentSol == otherSol){continue;}
        
        //if P_k costs less than P_i
        if((*otherSol).getProfit() > (*currentSol).getProfit()){
            //then P_new = P_i+r(P_k-P_i)
                for(int j=0;j<(*currentSol).size();j++){
                    for(int k=0;k<(*currentSol)[j].size();k++){
                        int r = rand()%2;
                        int diff = ceil((*currentSol)[j].at(k)+
                                    (r*((*otherSol)[j].at(k)-(*currentSol)[j].at(k))));
                        if(diff<=0){
                            (tempSol)[j].at(k) = false;
                        }else{
                            (tempSol)[j].at(k) = true;
                        }
                    }
                }
        //else
        }else{
            //then P_new = P_i+r(P_i-P_k)
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int r = rand()%2;
                    int diff = ceil((*currentSol)[j].at(k)+
                                (r*((*currentSol)[j].at(k)-(*otherSol)[j].at(k))));
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::learningPhase_Orthognal(std::vector<MMKPSolution>& population,
                             int iteration){
    
    quickSort(population,0,(population.size()-1));
    
    float wMax = 1.0;
    float wMin = .5;
    float weight = wMax -
    (((wMax - wMin)/parameters.numberOfGenerations) * iteration);
    
    
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;
        int randSol = rand() % population.size();
        MMKPSolution* otherSol = &population[randSol];
        
        //if they are the same, skip
        if(currentSol == otherSol){continue;}
        
        //if P_k costs less than P_i
        if((*otherSol).getProfit() > (*currentSol).getProfit()){
            //then P_new = P_i+r(P_k-P_i)
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int r = rand()%2;
                    int diff = ceil(weight*((*currentSol)[j].at(k)+
                                    (r*((*otherSol)[j].at(k)-(*currentSol)[j].at(k)))));
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                }
            }
            //else
        }else{
            //then P_new = P_i+r(P_i-P_k)
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int r = rand()%2;
                    int diff = ceil(weight*((*currentSol)[j].at(k)+
                                    (r*((*currentSol)[j].at(k)-(*otherSol)[j].at(k)))));
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}


