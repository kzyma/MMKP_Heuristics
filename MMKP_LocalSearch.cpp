/*********************************************************
 *
 * File: MMKP_LocalSearch.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Implementation of MMKP_LocalSearch
 *
 *********************************************************/

#include "MMKP_LocalSearch.h"

LocalSearch::LocalSearch(MMKPDataSet dataSet):dataSet(dataSet),funcEvals(0){}

int LocalSearch::getFuncEvals(){
    return this->funcEvals;
}

int LocalSearch::localSwapProcedure(MMKPSolution solution,int classI){
    
    dataSet.updateSolution(solution);
    assert(dataSet.isFeasible(solution));
    
    int selIndex = 0;
    float selProfit = 0;
    
    //get initial index
    for(int i=0;i<solution[classI].size();i++){
        if(solution[classI][i]){
            selIndex = i;
            selProfit = dataSet[classI][i].getProfit();
            break;
        }
    }
    
    int maxIndex = selIndex;
    
    //get resource use gap
    std::vector<float> constraintDiff(dataSet.getNumberOfResources());
    for(int i=0;i<dataSet.getNumberOfResources();i++){
        float diff = dataSet.getResource(i) - solution.getCost(i);
        constraintDiff.at(i) = diff;
    }
    
    for(int i=0;i<solution[classI].size();i++){
        bool isViolation = false;
        if(dataSet[classI][i].getProfit() <= dataSet[classI][maxIndex].getProfit()){
            continue;
        }
        //make sure does not violate any resource contraints
        for(int j=0;j<dataSet.getNumberOfResources();j++){
            float newDiff = dataSet[classI][i].getCost(j) -
                dataSet[classI][selIndex].getCost(j);
            funcEvals++;
            if((constraintDiff[j]) < newDiff){
                isViolation = true;
            }
        }
        if(!isViolation){
            maxIndex = i;
        }
    }
    return maxIndex;
}

CompLocalSearch::CompLocalSearch(MMKPDataSet dataSet):LocalSearch(dataSet){}

MMKPSolution CompLocalSearch::run(MMKPSolution solution){
    
    MMKPSolution newSol = solution;
    
    bool stoppingCriterion = false;
    
    while(!stoppingCriterion){
        int bestIndex = 0;
        int bestClassIndex = 0;
        float bestProfit = -1;
        int selIndex = 0;
        
        for(int i=0;i<newSol.size();i++){
            
            //get selected index
            for(int j=0;j<solution[i].size();j++){
                if(newSol[i][j]){
                    selIndex = j;
                }
            }
            
            //get new swap index
            int indx = LocalSearch::localSwapProcedure(newSol,i);
            
            //find 'swap' which will optimize our objective function
            if((dataSet[i][indx].getProfit() - dataSet[i][selIndex].getProfit())
               > bestProfit){
                bestIndex = indx;
                bestProfit = dataSet[i][indx].getProfit()
                    - dataSet[i][selIndex].getProfit();
                bestClassIndex = i;
            }
            //not a function evaluation, but the meat of this example is here
            funcEvals++;
        }
        
        //swap for largest profit increase
        int swapIndex = 0;
        for(int i=0;i<newSol[bestClassIndex].size();i++){
            if(newSol[bestClassIndex][i]){
                swapIndex = i;
            }
        }
        funcEvals++;
        newSol[bestClassIndex][swapIndex] = 0;
        newSol[bestClassIndex][bestIndex] = 1;
        
        dataSet.updateSolution(newSol);
        
        //stopping criterion if no progress is made.
        if(newSol.getProfit() <= solution.getProfit()){
            stoppingCriterion = true;
        }else{
            solution = newSol;
        }
    }
    
    assert(dataSet.isFeasible(newSol) == true);
    
    return newSol;
}

MMKPSolution CompLocalSearch::operator()(MMKPSolution solution){
    return run(solution);
}

std::vector<MMKPSolution> CompLocalSearch::run(std::vector<MMKPSolution> solutions){
    for(int i=0;i<solutions.size();i++){
        solutions[i] = CompLocalSearch::run(solutions[i]);
    }
    return solutions;
}

std::vector<MMKPSolution> CompLocalSearch::operator()
(std::vector<MMKPSolution> solutions){
    return run(solutions);
}

MMKPDataSet ReactiveLocalSearch::penalize
(MMKPSolution solution,int delta, float pie){
    
    MMKPDataSet newDataSet = this->dataSet;
    for(int i=0;i<delta;i++){
        int classI = rand() % solution.size();
        int itemI = 0;
        for(int j=0;j<solution[classI].size();j++){
            if(solution[i][j]){
                itemI = j;
                break;
            }
        }
        float p = newDataSet[classI][itemI].getProfit();
        newDataSet[classI][itemI].setProfit(p*pie);
    }
    return newDataSet;
}

ReactiveLocalSearch::ReactiveLocalSearch(MMKPDataSet dataSet)
:LocalSearch(dataSet){}

MMKPSolution ReactiveLocalSearch::run(MMKPSolution solution){
    
    MMKPSolution newSol = solution;
    MMKPDataSet newDataSet = dataSet;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> ud(0, 1);
    
    for(int i=0;i<10;i++){
        
        float pie = ud(gen);
        int delta = rand() % solution.size();
        
        CompLocalSearch CLS(newDataSet);
        newSol = CLS(newSol);
        this->funcEvals += CLS.getFuncEvals();
        if(solution.getProfit()<newSol.getProfit()){
            solution = newSol;
            dataSet.updateSolution(solution);
            newDataSet = this->dataSet;
        }else{
            newDataSet = ReactiveLocalSearch::penalize(newSol,delta,pie);
        }
        
    }
    return solution;
}

MMKPSolution ReactiveLocalSearch::operator()
(MMKPSolution solution){
    return run(solution);
}

std::vector<MMKPSolution> ReactiveLocalSearch::run
(std::vector<MMKPSolution> solutions){
    for(int i=0;i<solutions.size();i++){
        solutions[i] = ReactiveLocalSearch::run(solutions[i]);
    }
    return solutions;
}

std::vector<MMKPSolution> ReactiveLocalSearch::operator()
(std::vector<MMKPSolution> solutions){
    return run(solutions);
}



