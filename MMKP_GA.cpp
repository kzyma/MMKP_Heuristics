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

//private member functions
void MMKP_GA::competitiveUpdateSol
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

//constructors
MMKP_GA::MMKP_GA(MMKPDataSet dataSet, GA_parameters parameters)
:dataSet(dataSet),parameters(parameters){
    this->currentGeneration = 0;
    srand(time(NULL));
}

//overloaded operators
MMKPSolution MMKP_GA::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_GA::run(initialPopulation);
}

void MMKP_GA::quickSort(std::vector<MMKPSolution>& input,int p, int r){
    if ( p < r )
    {
        int j = partition(input, p, r);
        quickSort(input, p, j-1);
        quickSort(input, j+1, r);
    }
}

int MMKP_GA::partition(std::vector<MMKPSolution>& input,int p, int r){
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

MMKPSolution MMKP_GA::run(std::vector<MMKPSolution> initialPopulation){
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    bool terminationCriterion = false;
    this->currentGeneration = 0;
    
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
        
        quickSort(population,0,(population.size()-1));

        if(currentGeneration >=
           (this->parameters.numberOfGenerations*population.size())){
            terminationCriterion = true;
        }
        this->currentGeneration++;
    }
    
    for(int i=0;i<population.size();i++){
        if(this->dataSet.isFeasible(population[i])){
            if(population[i].getProfit() > this->bestSolution.getProfit()){
                this->bestSolution = population[i];
            }
        }
    }
    return this->bestSolution;
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


bool MMKP_GA::MMKP_GA::makeFeasible(MMKPSolution& sol){
    return MMKP_GA::makeFeasible(sol,
                                 this->parameters.multipleChoiceFeasibilityMod,
                                 this->parameters.multipleDimFeasibilityMod);
}

bool MMKP_GA::makeFeasible(MMKPSolution& sol,int mcFeas,int mdFeas){
    
    bool mcFeasible = false;
    bool mdFeasible = false;
    
    //multiple choice feasibility
    if(mcFeas == CH_FIXEDMAX){
        mcFeasible = MMKP_GA::makeMultiChoiceFeasFixedSurrogate(sol);
    }else if(mcFeas == CH_FIXEDMAX_RANDOM){
        mcFeasible = MMKP_GA::makeMultiChoiceFeasFixed_Rand_Surrogate(sol);
    }else if(mcFeas == CH_MAXPROFIT){
        mcFeasible = MMKP_GA::makeMultiChoiceFeasMaxProfit(sol);
    }else if(mcFeas == CH_MIX){
        int choice = rand() % 2;
        switch(choice){
            case 0:
                mcFeasible = MMKP_GA::makeMultiChoiceFeasFixed_Rand_Surrogate(sol);
                break;
            default:
                mcFeasible = MMKP_GA::makeMultiChoiceFeasMaxProfit(sol);
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
        mdFeasible = MMKP_GA::makeMultiDimFeasFixedSurrogate(sol);
    }else if(mdFeas == DIM_VARIABLEMAX){
        mdFeasible = MMKP_GA::makeMultiDimFeasVariableSurrogate(sol);
    }else if(mdFeas == DIM_VARIABLEMIN){
        mdFeasible = MMKP_GA::makeMultiDimFeasVarMinSurrogate(sol);
    }else if(mdFeas == DIM_MAXPROFIT){
        mdFeasible = MMKP_GA::makeMultiDimFeasVarMaximizeProfit(sol);
    }else if(mdFeas == DIM_MIX){
        int choice = rand() % 2;
        switch(choice){
            case 0:
                mdFeasible = MMKP_GA::makeMultiDimFeasVariableSurrogate(sol);
                break;
            default:
                mdFeasible = MMKP_GA::makeMultiDimFeasVarMaximizeProfit(sol);
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

bool MMKP_GA::makeMultiChoiceFeasFixedSurrogate(MMKPSolution& sol){
    
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

bool MMKP_GA::makeMultiChoiceFeasFixed_Rand_Surrogate(MMKPSolution& sol){
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

bool MMKP_GA::makeMultiChoiceFeasMaxProfit(MMKPSolution& sol){
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
bool MMKP_GA::makeMultiDimFeasFixedSurrogate(MMKPSolution& sol){
    
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

bool MMKP_GA::makeMultiDimFeasVariableSurrogate(MMKPSolution& sol){
    
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

bool MMKP_GA::makeMultiDimFeasVarMinSurrogate(MMKPSolution& sol){
    
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

bool MMKP_GA::makeMultiDimFeasVarMaximizeProfit(MMKPSolution& sol){
    
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




