/*********************************************************
 *
 * File: MMKP_TLBO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Teaching-Learning Based Optimization implementation.
 *
 *********************************************************/

#include "MMKP_TLBO.h"

/* MMKP_TLBO */

//constructors
MMKP_TLBO::MMKP_TLBO(MMKPDataSet dataSet, TLBO_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters){
}

MMKP_TLBO::MMKP_TLBO(MMKPDataSet dataSet)
:MMKP_MetaHeuristic(dataSet),parameters(){}

//overloaded operators
MMKPSolution MMKP_TLBO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_TLBO::run(initialPopulation);
}

MMKPSolution MMKP_TLBO::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    currentFuncEvals = 0;
    
    MMKPSolution bestSolution;
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            if(population[i].getProfit() > bestSolution.getProfit()){
                bestSolution = population[i];
                this->convergenceIteration = currentGeneration;
            }
        }
    }
    
    std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
    this->convergenceData.push_back(temp);
    
    MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
    
    //main loop
    while(!terminationCriterion){
        if(this->parameters.alg_Type == 0){
            MMKP_TLBO::teachingPhase(population);
            MMKP_TLBO::learningPhase(population);
        }else if(this->parameters.alg_Type == 1){
            MMKP_TLBO::teachingPhase_MultiTeacher(population,4);
            MMKP_TLBO::improvedLearningPhase(population);
        }else if(this->parameters.alg_Type == 2){
            MMKP_TLBO::teachingPhase_MultiTeacherEvenDist(population,4);
            MMKP_TLBO::improvedLearningPhase(population);
        }else if(this->parameters.alg_Type == 3){
            MMKP_TLBO::teachingPhase_Modified(population,4);
            MMKP_TLBO::modifiedLearningPhase(population);
        }else{
            throw OpNotSupported(("Teaching Phase ID not recognized.\n"));
        }
        
        MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
        
        for(int i=0;i<population.size();i++){
            if(this->dataSet.isFeasible(population[i])){
                if(population[i].getProfit() > bestSolution.getProfit()){
                    bestSolution = population[i];
                    this->convergenceIteration = currentGeneration;
                    break;
                }
            }
        }
        std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
        this->convergenceData.push_back(temp);
        
        if(currentGeneration >= (this->parameters.numberOfGenerations)){
            terminationCriterion = true;
        }
        currentGeneration++;
    }

    return bestSolution;
}

std::vector<MMKPSolution> MMKP_TLBO::runOneGeneration
(std::vector<MMKPSolution> population){
    
    MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
    
    if(this->parameters.alg_Type == 0){
        MMKP_TLBO::teachingPhase(population);
        MMKP_TLBO::learningPhase(population);
    }else if(this->parameters.alg_Type == 1){
        MMKP_TLBO::teachingPhase_MultiTeacher(population,4);
        MMKP_TLBO::improvedLearningPhase(population);
    }else if(this->parameters.alg_Type == 2){
        MMKP_TLBO::teachingPhase_MultiTeacherEvenDist(population,4);
        MMKP_TLBO::improvedLearningPhase(population);
    }else if(this->parameters.alg_Type == 3){
        MMKP_TLBO::teachingPhase_Modified(population,4);
        MMKP_TLBO::modifiedLearningPhase(population);
    }else{
        throw OpNotSupported(("Teaching Phase ID not recognized.\n"));
    }
    
    return population;
}

void MMKP_TLBO::teachingPhase(std::vector<MMKPSolution>& population){
    int teacherIndex = 0;
    
    MMKPSolution* teacher = &population[teacherIndex];
    MMKPSolution* mean = &population[(population.size()/2)];
    
    if(this->parameters.rls_on == 1){
        ReactiveLocalSearch RLS(dataSet);
        *teacher = RLS(*teacher);
        this->currentFuncEvals += RLS.getFuncEvals();
    }
    
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;
        if(currentSol == teacher){continue;}
        
        //step 1: convert bits with -2,-1 to 0, and 1 otherwise
        for(int j=0;j<(*currentSol).size();j++){
            for(int k=0;k<(*currentSol)[j].size();k++){
                int T_f = rand()%2+1;
                int r = rand()%2;
                int diff = ceil((*currentSol)[j].at(k)+(r*((*teacher)[j].at(k)
                                    - (T_f * (*mean)[j].at(k)))));
                if(diff<=0){
                    (tempSol)[j].at(k) = false;
                }else{
                    (tempSol)[j].at(k) = true;
                }
                this->currentFuncEvals++;
            }
        }
        
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::teachingPhase_MultiTeacherEvenDist(std::vector<MMKPSolution>& population,
                                int numberOfTeachers){
    
    int classSize = population.size() / numberOfTeachers;
    
    int start = 0;
    int stop = -1;
    
    if(classSize < this->parameters.populationSize){
        stop = (start+classSize)-1;
    }else{
        stop = this->parameters.populationSize - 1;
    }
    
    while((start < stop) && (stop < this->parameters.populationSize)){
        
        MMKPSolution* teacher = &population[start];
        if(this->parameters.rls_on == 1){
            ReactiveLocalSearch RLS(dataSet);
            *teacher = RLS(*teacher);
            this->currentFuncEvals += RLS.getFuncEvals();
        }
        
        MMKPSolution* mean = &population[((start+stop)/2)];
        
        for(int i=start;i<stop;i++){
            
            MMKPSolution* currentSol = &population[i];
            MMKPSolution tempSol = *currentSol;
            if(currentSol == teacher){continue;}
            //r_i is the random learnin to effect i-tlbo
            int r_i = rand() % (stop-start)+start;
            
            //step 1: convert bits with -2,-1 to 0, and 1 otherwise
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int T_f = rand()%2+1;
                    int r = rand()%2;   //r, not used in Vasko et al.
                    int diff = ((*currentSol)[j].at(k)+(r*((*teacher)[j].at(k)
                                                - (T_f * (*mean)[j].at(k)))));
                    //i-tlbo addition to difference mean
                    if((*currentSol).getProfit() > population[r_i].getProfit()){
                        diff += (*currentSol)[j].at(k) - population[r_i][j].at(k);
                    }else{
                        diff += population[r_i][j].at(k) - (*currentSol)[j].at(k);
                    }
                    
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                    this->currentFuncEvals++;
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

void MMKP_TLBO::teachingPhase_Modified(std::vector<MMKPSolution>& population,
                            int numberOfTeachers){
    
    std::priority_queue<int,std::vector<int>,std::greater<int> > pq;
    pq.push(0);
    for(int i=0;i<numberOfTeachers-1;i++){
        pq.push((rand() % population.size()));
    }
    
    while(!pq.empty()){
        
        int start = pq.top();
        pq.pop();
        int stop;
        if(!pq.empty()){
            stop = pq.top();
        }else{
            stop = population.size();
        }
        
        MMKPSolution* teacher = &population[start];
        if(this->parameters.rls_on == 1){
            ReactiveLocalSearch RLS(dataSet);
            *teacher = RLS(*teacher);
            this->currentFuncEvals += RLS.getFuncEvals();
        }
        
        MMKPSolution* mean = &population[((start+stop)/2)];
        
        for(int i=start;i<stop;i++){
            
            MMKPSolution* currentSol = &population[i];
            MMKPSolution tempSol = *currentSol;
            if(currentSol == teacher){continue;}
            
            //step 1: convert bits with -2,-1 to 0, and 1 otherwise
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int T_f = rand()%2+1;
                    int r = rand()%2;   //r, not used in Vasko et al.
                    int diff = ((*currentSol)[j].at(k)+(r*((*teacher)[j].at(k)
                                                        - (T_f * (*mean)[j].at(k)))));
                    
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                    this->currentFuncEvals++;
                }
            }
            dataSet.updateSolution(tempSol);
            MMKP_MetaHeuristic::makeFeasible(tempSol);
            MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
            
        }
        
    }//end while(stop....
}

void MMKP_TLBO::teachingPhase_MultiTeacher(std::vector<MMKPSolution>& population,
                                int numberOfTeachers){
    
    std::priority_queue<int,std::vector<int>,std::greater<int> > pq;
    pq.push(0);
    for(int i=0;i<numberOfTeachers-1;i++){
        pq.push((rand() % population.size()));
    }
    
    while(!pq.empty()){
        
        int start = pq.top();
        pq.pop();
        int stop;
        if(!pq.empty()){
            stop = pq.top();
        }else{
            stop = population.size();
        }
        
        MMKPSolution* teacher = &population[start];
        if(this->parameters.rls_on == 1){
            ReactiveLocalSearch RLS(dataSet);
            *teacher = RLS(*teacher);
            this->currentFuncEvals += RLS.getFuncEvals();
        }
        
        MMKPSolution* mean = &population[((start+stop)/2)];
        
        for(int i=start;i<stop;i++){
            
            MMKPSolution* currentSol = &population[i];
            MMKPSolution tempSol = *currentSol;
            if(currentSol == teacher){continue;}
            //r_i is the random learnin to effect i-tlbo
            int r_i = rand() % (stop-start)+start;
            
            //step 1: convert bits with -2,-1 to 0, and 1 otherwise
            for(int j=0;j<(*currentSol).size();j++){
                for(int k=0;k<(*currentSol)[j].size();k++){
                    int T_f = rand()%2+1;
                    int r = rand()%2;   //r, not used in Vasko et al.
                    int diff = ((*currentSol)[j].at(k)+(r*((*teacher)[j].at(k)
                                                        - (T_f * (*mean)[j].at(k)))));
                    //i-tlbo addition to difference mean
                    if((*currentSol).getProfit() > population[r_i].getProfit()){
                        diff += (*currentSol)[j].at(k) - population[r_i][j].at(k);
                    }else{
                        diff += population[r_i][j].at(k) - (*currentSol)[j].at(k);
                    }
                    
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                    this->currentFuncEvals++;
                }
            }
            dataSet.updateSolution(tempSol);
            MMKP_MetaHeuristic::makeFeasible(tempSol);
            MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
            
        }
        
    }//end while(stop....
}

void MMKP_TLBO::teachingPhase_Orthognal(std::vector<MMKPSolution>& population,
                             int iteration){
    MMKPSolution* teacher = &population[0];
    if(this->parameters.rls_on == 1){
        ReactiveLocalSearch RLS(dataSet);
        *teacher = RLS(*teacher);
        this->currentFuncEvals += RLS.getFuncEvals();
    }
    
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
                        this->currentFuncEvals++;
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
                    this->currentFuncEvals++;
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::improvedLearningPhase(std::vector<MMKPSolution>& population){
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
                    //i-tlbo addition
                    int selfLearning = population[0][j].at(k)
                        -(1+(rand()%2)*(*currentSol)[j].at(k));
                    
                    if (selfLearning < 0){
                        diff -= (rand() % abs(selfLearning));
                    }else if( selfLearning > 0){
                        diff += (rand() % abs(selfLearning));
                    }
                    
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                    this->currentFuncEvals++;
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
                    //i-tlbo addition
                    int selfLearning = population[0][j].at(k)
                    -(1+(rand()%2)*(*currentSol)[j].at(k));
                    
                    if (selfLearning < 0){
                        diff -= (rand() % abs(selfLearning));
                    }else if( selfLearning > 0){
                        diff += (rand() % abs(selfLearning));
                    }
                    
                    if(diff<=0){
                        (tempSol)[j].at(k) = false;
                    }else{
                        (tempSol)[j].at(k) = true;
                    }
                    this->currentFuncEvals++;
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
}

void MMKP_TLBO::modifiedLearningPhase(std::vector<MMKPSolution>& population){
    
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
                    this->currentFuncEvals++;
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
                    this->currentFuncEvals++;
                }
            }
        }
        dataSet.updateSolution(tempSol);
        MMKP_MetaHeuristic::makeFeasible(tempSol);
        MMKP_MetaHeuristic::competitiveUpdateSol(*currentSol,tempSol);
    }
    
    //self motivated learning
    for(int i=0;i<population.size();i++){
        MMKPSolution* currentSol = &population[i];
        MMKPSolution tempSol = *currentSol;

        for(int j=0;j<(*currentSol).size();j++){
            for(int k=0;k<(*currentSol)[j].size();k++){
                int r = rand()%2;
                int diff = ((*currentSol)[j].at(k)+r);
                if(diff<=0){
                    (tempSol)[j].at(k) = false;
                }else{
                    (tempSol)[j].at(k) = true;
                }
                this->currentFuncEvals++;
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


