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

//private member functions
void MMKP_TLBO::competitiveUpdateSol
(MMKPSolution& sol, MMKPSolution& newSol){
    
    dataSet.updateSolution(sol);
    dataSet.updateSolution(newSol);
    
    bool solFeas = this->dataSet.isFeasible(sol);
    bool newFeas = this->dataSet.isFeasible(newSol);

    //if both feasible, choose best profit
    if(newFeas && solFeas){
        if((newSol).getProfit() > (sol).getProfit()){
            sol = newSol;
        }
        //if one is feasible, that automatically is choosen
    }else if((!solFeas) && newFeas){
        sol = newSol;
        //if neither is feasible, choose greatest profit.
    }else if ((!solFeas && !newFeas)&&(newSol.getProfit() > sol.getProfit())){
        sol = newSol;
    }
}

void MMKP_TLBO::threshholdAnnealingUpdateSol
(MMKPSolution& sol, MMKPSolution& newSol){
    
    dataSet.updateSolution(sol);
    dataSet.updateSolution(newSol);
    
    bool solFeas = this->dataSet.isFeasible(sol);
    bool newFeas = this->dataSet.isFeasible(newSol);
    
    const float T = 0.36;

    float threshold = sol.getProfit() - (sol.getProfit()*(T/currentGeneration));
    
    if(newFeas && solFeas){
        if((newSol).getProfit() > threshold){
            sol = newSol;
        }
        //if one is feasible, that automatically is choosen
    }else if((!solFeas) && newFeas){
        sol = newSol;
    }else if ((!solFeas && !newFeas)&&(newSol.getProfit() > sol.getProfit())){
        sol = newSol;
    }
    
}

//constructors
MMKP_TLBO::MMKP_TLBO(MMKPDataSet dataSet, TLBO_parameters parameters)
:dataSet(dataSet),parameters(parameters){
    this->currentGeneration = 0;
    srand(time(NULL));
}

//overloaded operators
MMKPSolution MMKP_TLBO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_TLBO::run(initialPopulation);
}

void MMKP_TLBO::quickSort(std::vector<MMKPSolution>& input,int p, int r){
    if ( p < r )
    {
        int j = partition(input, p, r);
        quickSort(input, p, j-1);
        quickSort(input, j+1, r);
    }
}

int MMKP_TLBO::partition(std::vector<MMKPSolution>& input,int p, int r){
    //partition will sort greatest->least
    int pivot = input[r].getProfit();
    while ( p < r )
    {
        while ( input[p].getProfit() > pivot )
            p++;
        
        while ( input[r].getProfit() < pivot )
            r--;
        
        if ( input[p].getProfit() == input[r].getProfit() )
            p++;
        
        else if (p < r )
        {
            MMKPSolution tmp = input[p];
            input[p] = input[r];
            input[r] = tmp;
        }
    }
    
    return r;
}

MMKPSolution MMKP_TLBO::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    int currentNOP = 0;
    this->currentGeneration = 0;
    
    quickSort(population,0,(population.size()-1));
    
    //main loop
    while(!terminationCriterion){
        
        if(this->parameters.classroomSize == 0){
            MMKP_TLBO::teachingPhase(population);
        }else{
            int size = this->parameters.classroomSize;
            MMKP_TLBO::teachingPhase_MultiTeacher(population,size);
        }
        MMKP_TLBO::learningPhase(population);
        
        quickSort(population,0,(population.size()-1));
        
        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        this->currentGeneration++;
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
        MMKP_TLBO::makeFeasible(tempSol);
        
        if(parameters.addSimAnnealing == 1){
            MMKP_TLBO::threshholdAnnealingUpdateSol(*currentSol,tempSol);
        }else{
            MMKP_TLBO::competitiveUpdateSol(*currentSol,tempSol);
        }
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
            MMKP_TLBO::makeFeasible(tempSol);
            
            if(parameters.addSimAnnealing == 1){
                MMKP_TLBO::threshholdAnnealingUpdateSol(*currentSol,tempSol);
            }else{
                MMKP_TLBO::competitiveUpdateSol(*currentSol,tempSol);
            }
            
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
        MMKP_TLBO::makeFeasible(tempSol);
        
        if(parameters.addSimAnnealing == 1){
            MMKP_TLBO::threshholdAnnealingUpdateSol(*currentSol,tempSol);
        }else{
            MMKP_TLBO::competitiveUpdateSol(*currentSol,tempSol);
        }
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
            MMKP_TLBO::makeFeasible(tempSol);
            
            //if i==0, the teacher is up for change which means they should
            //ONLY change if other solution profit is higher
            if((parameters.addSimAnnealing == SIMHEAL_THRESHOLD) && (i != 0)){
                MMKP_TLBO::threshholdAnnealingUpdateSol(population[i],tempSol);
            }else{
                MMKP_TLBO::competitiveUpdateSol(population[i],tempSol);
            }
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
        MMKP_TLBO::makeFeasible(tempSol);
        
        //if i==0, the teacher is up for change which means they should
        //ONLY change if other solution profit is higher
        if((parameters.addSimAnnealing == SIMHEAL_THRESHOLD) && (i != 0)){
            MMKP_TLBO::threshholdAnnealingUpdateSol(population[i],tempSol);
        }else{
            MMKP_TLBO::competitiveUpdateSol(population[i],tempSol);
        }
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
        MMKP_TLBO::makeFeasible(tempSol);

        //if i==0, the teacher is up for change which means they should
        //ONLY change if other solution profit is higher
        if((parameters.addSimAnnealing == SIMHEAL_THRESHOLD) && (i != 0)){
            MMKP_TLBO::threshholdAnnealingUpdateSol(*currentSol,tempSol);
        }else{
            MMKP_TLBO::competitiveUpdateSol(*currentSol,tempSol);
        }
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
        MMKP_TLBO::makeFeasible(tempSol);

        //if i==0, the teacher is up for change which means they should
        //ONLY change if other solution profit is higher
        if((parameters.addSimAnnealing == SIMHEAL_THRESHOLD) && (i != 0)){
            MMKP_TLBO::threshholdAnnealingUpdateSol(*currentSol,tempSol);
        }else{
            MMKP_TLBO::competitiveUpdateSol(*currentSol,tempSol);
        }
    }
}

bool MMKP_TLBO::makeFeasible(MMKPSolution& sol){
    return MMKP_TLBO::makeFeasible(sol,
                        this->parameters.multipleChoiceFeasibilityMod,
                        this->parameters.multipleDimFeasibilityMod);
}

bool MMKP_TLBO::makeFeasible(MMKPSolution& sol,int mcFeas,int mdFeas){
    
    bool mcFeasible = false;
    bool mdFeasible = false;
    
    //multiple choice feasibility
    if(mcFeas == CH_FIXEDMAX){
        mcFeasible = MMKP_TLBO::makeMultiChoiceFeasFixedSurrogate(sol);
    }else if(mcFeas == CH_FIXEDMAX_RANDOM){
        mcFeasible = MMKP_TLBO::makeMultiChoiceFeasFixed_Rand_Surrogate(sol);
    }else if(mcFeas == CH_MAXPROFIT){
        mcFeasible = MMKP_TLBO::makeMultiChoiceFeasMaxProfit(sol);
    }else if(mcFeas == CH_MIX){
        int choice = rand() % 2;
        switch(choice){
            case 0:
                mcFeasible = MMKP_TLBO::makeMultiChoiceFeasFixed_Rand_Surrogate(sol);
                break;
            default:
                mcFeasible = MMKP_TLBO::makeMultiChoiceFeasMaxProfit(sol);
                break;
        }
    }else if(mcFeas == CH_NONE){
        //none
    }else{
        //not supported
        throw OpNotSupported(("Multiple Choice feasibility num: "+
                    std::to_string(this->parameters.multipleChoiceFeasibilityMod)
                    +" not supported.\n"));
    }
    
    //multiple dim feasibility
    if(mdFeas == DIM_FIXEDMAX){
        mdFeasible = MMKP_TLBO::makeMultiDimFeasFixedSurrogate(sol);
    }else if(mdFeas == DIM_VARIABLEMAX){
        mdFeasible = MMKP_TLBO::makeMultiDimFeasVariableSurrogate(sol);
    }else if(mdFeas == DIM_VARIABLEMIN){
        mdFeasible = MMKP_TLBO::makeMultiDimFeasVarMinSurrogate(sol);
    }else if(mdFeas == DIM_MAXPROFIT){
        mdFeasible = MMKP_TLBO::makeMultiDimFeasVarMaximizeProfit(sol);
    }else if(mdFeas == DIM_MIX){
        int choice = rand() % 2;
        switch(choice){
            case 0:
                mdFeasible = MMKP_TLBO::makeMultiDimFeasVariableSurrogate(sol);
                break;
            default:
                mdFeasible = MMKP_TLBO::makeMultiDimFeasVarMaximizeProfit(sol);
                break;
        }
    }else if(mdFeas == DIM_NONE){
        //none
    }else{
        //not supported
        throw OpNotSupported(("Multiple Dim feasibility num: "+
                              std::to_string(this->parameters.multipleDimFeasibilityMod)
                              +" not supported.\n"));
    }
    
    if(mcFeasible && mdFeasible){
        return true;
    }else{
        return false;
    }
}

bool MMKP_TLBO::makeMultiChoiceFeasFixedSurrogate(MMKPSolution& sol){
    
    if(dataSet.isMultiChoiceFeasible(sol)){return true;}
    
    //step 2 from "draft of logic to convert TLBO Sol...
    for(int i=0;i<sol.size();i++){
        
        int sum = 0;
        int h1 = 0;   //highest index of all v/[Er%/n] in a sol row
        int h2 = -1;   //highest index of selected v/[Er%/n] in a sol row
        
        for(int j=0;j<sol[i].size();j++){
            if(sol[i][j]){h2 = j;break;}
        }
        
        for(int j=0;j<sol[i].size();j++){
            sum += sol[i][j];
            
            if(dataSet[i][j].getProfitConstraintUsePercent() >
               dataSet[i][h1].getProfitConstraintUsePercent()){
                h1 = j;
            }
            if((sol[i][j]) && (dataSet[i][j].getProfitConstraintUsePercent() >
                               dataSet[i][h2].getProfitConstraintUsePercent())){
                assert(h2 != -1);   //if nothing is selected and here, fail.
                h2 = j;
            }
        }
        
        if(sum == 1){
            continue;
        //choose highest v/[Er%/n]
        }else if(sum == 0){
            sol[i][h1] = true;
        //choose highest v/[Er%/n] of selected items,
        //all others should be unselected
        }else{
            assert(h2 != -1);   //if nothing is selected and here, fail.
            for(int j=0;j<sol[i].size();j++){
                sol[i][j] = false;
            }
            sol[i][h2] = true;
        }
    }
    this->dataSet.updateSolution(sol);
    return true;    //will always be made feasible multi-choice feasible.
}

bool MMKP_TLBO::makeMultiChoiceFeasFixed_Rand_Surrogate(MMKPSolution& sol){
    if(dataSet.isMultiChoiceFeasible(sol)){return true;}
    
    //step 2 from "draft of logic to convert TLBO Sol...
    for(int i=0;i<sol.size();i++){
        
        int sum = 0;
        int h1 = 0;   //highest index of all v/[Er%/n] in a sol row
        int h2 = -1;   //highest index of selected v/[Er%/n] in a sol row
        
        for(int j=0;j<sol[i].size();j++){
            if(sol[i][j]){h2 = j;break;}
        }
        
        for(int j=0;j<sol[i].size();j++){
            sum += sol[i][j];
            
            if(dataSet[i][j].getProfitConstraintUsePercent() >
               dataSet[i][h1].getProfitConstraintUsePercent()){
                h1 = j;
            }
            if((sol[i][j]) && (dataSet[i][j].getProfitConstraintUsePercent() >
                               dataSet[i][h2].getProfitConstraintUsePercent())){
                assert(h2 != -1);   //if nothing is selected and here, fail.
                h2 = j;
            }
        }
        
        if(sum == 1){
            continue;
            //choose highest v/[Er%/n]
        }else if(sum == 0){
            int r = rand() % sol[i].size();
            sol[i][r] = true;
            //choose highest v/[Er%/n] of selected items,
            //all others should be unselected
        }else{
            assert(h2 != -1);   //if nothing is selected and here, fail.
            for(int j=0;j<sol[i].size();j++){
                sol[i][j] = false;
            }
            sol[i][h2] = true;
        }
    }
    this->dataSet.updateSolution(sol);
    return true;    //will always be made feasible multi-choice feasible.
}

bool MMKP_TLBO::makeMultiChoiceFeasMaxProfit(MMKPSolution& sol){
    if(dataSet.isMultiChoiceFeasible(sol)){return true;}
    
    //step 2 from "draft of logic to convert TLBO Sol...
    for(int i=0;i<sol.size();i++){
        
        int sum = 0;
        int h1 = 0;   //highest index of all profit
        int h2 = -1;   //highest index of selected profit in row
        
        for(int j=0;j<sol[i].size();j++){
            if(sol[i][j]){h2 = j;break;}
        }
        
        for(int j=0;j<sol[i].size();j++){
            sum += sol[i][j];
            
            if(dataSet[i][j].getProfit() >
               dataSet[i][h1].getProfit()){
                h1 = j;
            }
            if((sol[i][j]) && (dataSet[i][j].getProfit() >
                               dataSet[i][h2].getProfit())){
                assert(h2 != -1);   //if nothing is selected and here, fail.
                h2 = j;
            }
        }
        
        if(sum == 1){
            continue;
            //choose highest v/[Er%/n]
        }else if(sum == 0){
            int r = rand() % sol[i].size();
            sol[i][r] = true;
            //choose highest profit of selected items,
            //all others should be unselected
        }else{
            assert(h2 != -1);   //if nothing is selected and here, fail.
            for(int j=0;j<sol[i].size();j++){
                sol[i][j] = false;
            }
            sol[i][h2] = true;
        }
    }
    this->dataSet.updateSolution(sol);
    return true;    //will always be made feasible multi-choice feasible.
}

//ret true if solution is feasible, false otherwise.
bool MMKP_TLBO::makeMultiDimFeasFixedSurrogate(MMKPSolution& sol){
    
    int iterationCounter = 0;

    while(!(this->dataSet.isMultiDimFeasible(sol))){
        std::vector<int> solMaxIndex;
        std::vector<int> minIndex;
        
        //find resource using the most above min for all classes
        for(int i=0;i<sol.size();i++){
            //find max of all selected in a given class
            int localMax = -1;
            float localMaxVal = 0;
            int localMin = 0;   //default to first item in class
            for(int j=0;j<sol[i].size();j++){
                //just check for some off-chance there may
                //be a negative here at some point.
                //in which case this will never be a feasible
                //pick.
                assert(dataSet[i].at(j).getConstraintUsePercent() >= 0);
                if((sol[i].at(j)) &&
                   (dataSet[i].at(j).getConstraintUsePercent() >= localMaxVal)){
                    localMax = j;
                    localMaxVal = dataSet[i].at(j).getConstraintUsePercent();
                }
                if(dataSet[i].at(j).getConstraintUsePercent() <
                   dataSet[i].at(localMin).getConstraintUsePercent()){
                    localMin = j;
                }
            }
            assert(localMax != -1);
            solMaxIndex.push_back(localMax);
            minIndex.push_back(localMin);
        }
        
        //replace with min resource item for that class
        bool noDiff = true;
        int maxDiffIndex = 0;
        std::vector<float> diff(solMaxIndex.size(),0);
        for(int i=0;i<diff.size();i++){
            diff.at(i) = dataSet[i].at(solMaxIndex.at(i)).getConstraintUsePercent()
            - dataSet[i].at(minIndex.at(i)).getConstraintUsePercent();
            if(diff.at(i) > diff.at(maxDiffIndex)){maxDiffIndex = i;}
            if((diff.at(i)) > 0){noDiff = false;}
        }
        
        //there is no diff, solution is all lowest Er%/n
        if(noDiff){
            return false;
        }
        
        sol[maxDiffIndex][solMaxIndex.at(maxDiffIndex)] = false;
        sol[maxDiffIndex][minIndex.at(maxDiffIndex)] = true;
        this->dataSet.updateSolution(sol);
        
        iterationCounter++;
    }
    
    return true;
}

bool MMKP_TLBO::makeMultiDimFeasVariableSurrogate(MMKPSolution& sol){
    
    int currentIterations = 0;
    const int maxIterations = (dataSet.getNumberOfResources() *
                               dataSet.size());
    
    while(!(this->dataSet.isMultiDimFeasible(sol))){
        std::vector<int> solMaxIndex;
        std::vector<int> minIndex;
        std::vector<int> offendingI = this->dataSet.getOffendingIndices(sol);
        
        //find resource using the most above min for all classes
        for(int i=0;i<sol.size();i++){
            //find max of all selected in a given class
            int localMax = -1;
            float localMaxVal = 0;
            int localMin = 0;   //default to first item in class
            for(int j=0;j<sol[i].size();j++){
                //just check for some off-chance there may
                //be a negative here at some point.
                //in which case this will never be a feasible
                //pick.
                assert(dataSet[i].at(j).getConstraintUsePercent(offendingI) >= 0);
                if((sol[i].at(j)) &&
                   (dataSet[i].at(j).getConstraintUsePercent(offendingI) >=
                        localMaxVal)){
                    localMax = j;
                    localMaxVal = dataSet[i].at(j)
                       .getConstraintUsePercent(offendingI);
                }
                if(dataSet[i].at(j).getConstraintUsePercent(offendingI) <
                   dataSet[i].at(localMin).getConstraintUsePercent(offendingI)){
                    localMin = j;
                }
            }
            assert(localMax != -1);
            solMaxIndex.push_back(localMax);
            minIndex.push_back(localMin);
        }
        
        //replace with min resource item for that class
        bool noDiff = true;
        int maxDiffIndex = 0;
        std::vector<float> diff(solMaxIndex.size(),0);
        for(int i=0;i<diff.size();i++){
            diff.at(i) = dataSet[i].at(solMaxIndex.at(i))
                .getConstraintUsePercent(offendingI)
            - dataSet[i].at(minIndex.at(i)).getConstraintUsePercent(offendingI);
            if(diff.at(i) > diff.at(maxDiffIndex)){maxDiffIndex = i;}
            if((diff.at(i)) > 0){noDiff = false;}
        }
        
        //there is no diff, solution is all lowest Er%/n
        if(noDiff || (currentIterations == maxIterations)){
            return false;
        }
        sol[maxDiffIndex][solMaxIndex.at(maxDiffIndex)] = false;
        sol[maxDiffIndex][minIndex.at(maxDiffIndex)] = true;
        this->dataSet.updateSolution(sol);
        currentIterations++;
    }
    
    return true;
}

bool MMKP_TLBO::makeMultiDimFeasVarMinSurrogate(MMKPSolution& sol){
    
    int lastClass = -1;
    int lastItem = -1;
    int iterations = 0;
    
    while(!(this->dataSet.isMultiDimFeasible(sol))){
        std::vector<int> selectedIndex(sol.size());
        //find difference for selected values of each class
        for(int i=0;i<sol.size();i++){
            for(int j=0;j<sol[i].size();j++){
                if(sol[i][j]){
                    selectedIndex[i] = j;
                    break;
                }
            }
        }
        
        int choosenClass = 0;
        int choosenItem = selectedIndex[0];
        int choosenDiff = 100;
        
        //find minimum difference
        for(int i=0;i<sol.size();i++){
            std::vector<int> indices = this->dataSet.getOffendingIndices(sol);
            int selectedPercent
            = (this->dataSet[i][selectedIndex[i]]
               .getConstraintUsePercent(indices))*100;
            for(int j=0;j<sol[i].size();j++){
                int newPercent
                    = ((float)this->dataSet[i][j]
                       .getConstraintUsePercent(indices))*100;
                int diff = selectedPercent - newPercent;
                if((diff > 0)&&(choosenDiff > diff)&&
                   (lastClass != i)&&(lastItem != j)){
                    choosenClass = i;
                    choosenItem = j;
                    choosenDiff = diff;
                }
            }
        }

        sol[choosenClass][selectedIndex[choosenClass]] = false;
        sol[choosenClass][choosenItem] = true;
        lastClass = choosenClass;
        lastItem = selectedIndex[choosenClass];
        this->dataSet.updateSolution(sol);
        
        if(iterations == 20){
            return false;
        }
        iterations++;

    }
    
    return true;
    
}

bool MMKP_TLBO::makeMultiDimFeasVarMaximizeProfit(MMKPSolution& sol){
    
    int lastClass = -1;
    int lastItem = -1;
    int iterations = 0;
    
    while(!(this->dataSet.isMultiDimFeasible(sol))){
        std::vector<int> indices = this->dataSet
            .getOffendingIndices(sol);
        std::vector<int> selectedIndex(sol.size());
        //find difference for selected values of each class
        for(int i=0;i<sol.size();i++){
            for(int j=0;j<sol[i].size();j++){
                if(sol[i][j]){
                    selectedIndex[i] = j;
                    break;
                }
            }
        }
        
        int choosenClass = 0;
        int choosenItem = selectedIndex[0];
        float choosenDiff = FLT_MAX;
        
        //find minimum difference
        for(int i=0;i<sol.size();i++){
            float selectedProfit = (this->dataSet[i][selectedIndex[i]]
                                  .getProfit());
            float selectedConstrUse = (this->dataSet[i][selectedIndex[i]]
                                      .getConstraintUsePercent(indices));
            for(int j=0;j<sol[i].size();j++){
                float newProfit = this->dataSet[i][j]
                                  .getProfit();
                float diff = selectedProfit - newProfit;
                float newConstrUse = this->dataSet[i][j]
                                      .getConstraintUsePercent(indices);
                if((diff > 0)&&(choosenDiff > diff)&&
                   (lastClass != i)&&(lastItem != j)&&
                   (newConstrUse < selectedConstrUse)){
                    choosenClass = i;
                    choosenItem = j;
                    choosenDiff = diff;
                }
            }
        }
        
        sol[choosenClass][selectedIndex[choosenClass]] = false;
        sol[choosenClass][choosenItem] = true;
        lastClass = choosenClass;
        lastItem = selectedIndex[choosenClass];
        this->dataSet.updateSolution(sol);
        
        //temporary workaround to handle non-typical data
        if(iterations == 100){
            return false;
        }
    
        iterations++;
    }
    return true;
}



