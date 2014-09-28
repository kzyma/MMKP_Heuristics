/*********************************************************
 *
 * File: MMKP_ACO.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * BBA MMKP Problem
 *
 *********************************************************/

#include "MMKP_ACO.h"

/*ACO_DataSetAdditions*/

ACO_DataSetAdditions::ACO_DataSetAdditions(MMKPDataSet dataSet,float B,float e):
denominators(dataSet.size()),B(B),isUpdated(false),Lstar(0),dataSet(dataSet){
    
    //alloc constainers
    for(std::size_t i=0;i<dataSet.size();i++){
        std::vector<ACO_Additions> temp(dataSet[i].size());
        this->classList.push_back(temp);
    }
}

void ACO_DataSetAdditions::initParameters(){
    
    float INITIAL_PHERAMONE_VAL = 0.50;
    
    //alloc heuristic information and pheramone values
    MMKPSolution LB(dataSet.getSizeOfEachClass(),dataSet.getNumberOfResources());
    for(int i=0;i<LB.size();i++){
        int itemIndex = 0;
        float itemCR = 0;
        for(int k=0;k<dataSet.getNumberOfResources();k++){
            itemCR += dataSet[i][itemIndex].getCost(k) / dataSet.getResource(k);
        }
        for(int j=0;j<LB[i].size();j++){
            int newCR = 0;
            for(int k=0;k<dataSet.getNumberOfResources();k++){
                newCR += dataSet[i][j].getCost(k) / dataSet.getResource(k);
            }
            if(newCR < itemCR){
                itemIndex = j;
                itemCR = newCR;
            }
        }
        LB[i][itemIndex] = true;
    }
    
    //a good LMV can be produced in 10*m iterations
    //get u_0 vector
    std::vector<float> u_0(dataSet.getNumberOfResources(),0);
    for(int i=0;i<u_0.size();i++){
        
        float tempSum1 = 0;
        for(int j=0;j<dataSet.size();j++){
            float tempSum2 = 0;
            for(int k=0;k<dataSet[j].size();k++){
                tempSum2 += dataSet[j][k].getCost(i) / dataSet[j].size();
            }
            tempSum1 = tempSum2 - dataSet.getResource(i);
        }
        u_0[i] += tempSum1 / dataSet.getResource(i);
        
        if(u_0[i] < 0.0){
            u_0[i] = 0.0;
        }
    }
    
    std::vector<float> best_U = u_0;
    MMKPSolution best_LSol = ACO_DataSetAdditions::getLMV(u_0);
    float best_L = best_LSol.getProfit();
    
    std::vector<float> past_u = best_U;
    MMKPSolution past_sol = best_LSol;
    int LSubCounter = 0;
    for(int i=0;i<10*dataSet.getNumberOfResources();i++){
        std::vector<float> curr_u(dataSet.getNumberOfResources(),0);
        float lambda = 1.0;
        
        std::vector<float> SK(dataSet.getNumberOfResources());
        float temp_sum = 0;
        for(int k=0;k<dataSet.getNumberOfResources();k++){
            SK[k] = getS_k(past_sol,k);
            temp_sum += pow(SK[k],2);
        }
        float den = pow(sqrt(temp_sum),2);
        
        for(int k=0;k<dataSet.getNumberOfResources();k++){
            float num =
                (ACO_DataSetAdditions::getLMV(past_u)).getProfit()-LB.getProfit();
            
            curr_u[k] = (past_u[k] + lambda*(num/den)*(SK[k]));
            if(curr_u[k] < 0){
                curr_u[k] = 0;
            }
        }
        MMKPSolution currLSol = ACO_DataSetAdditions::getLMV(curr_u);
        float currLVM = currLSol.getProfit();
        if(currLVM < best_L){
            best_U = curr_u;
            best_L = currLVM;
            LSubCounter = 0;
        }else{
            LSubCounter++;
        }
        if(LSubCounter == 10){
            lambda = lambda/2;
        }
        past_u = curr_u;
        past_sol = currLSol;
    }
    
    
    float lambda = 1.0;
    float U[dataSet.getNumberOfResources()];
    
    for(int i=0;i<this->classList.size();i++){
        for(int j=0;j<this->classList[i].size();j++){
            this->classList[i][j].pheramone = INITIAL_PHERAMONE_VAL;
            float c_ij = 0;
            for(int k=0;k<dataSet[i][j].getNumberOfCosts();k++){
                c_ij += (dataSet[i][j].getCost(k)*best_U[k]);
            }
            float lagrangian_ij = dataSet[i][j].getProfit() - c_ij;
            if(lagrangian_ij < 0){
                lagrangian_ij = 0.0005;  //NEEDS TO CHANGE
            }
            this->classList[i][j].heuristic = lagrangian_ij;
        }
    }
    
    this->Lstar = best_L;
}

MMKPSolution ACO_DataSetAdditions::getLMV(std::vector<float> U){
    
    assert(dataSet.getNumberOfResources() == U.size());
    
    MMKPSolution sol(dataSet.getSizeOfEachClass(),dataSet.getNumberOfResources());
    
    float LU = 0;
    for(int i=0;i<dataSet.size();i++){
        float bestLagrangian = 0;
        int bestLIndex = 0;
        for(int j=0;j<dataSet[i].size();j++){
            float tempC = 0;
            for(int k=0;k<dataSet.getNumberOfResources();k++){
                tempC += U[k]* dataSet[i][j].getCost(k);
            }
            float lagrangian = dataSet[i][j].getProfit() - tempC;
            if(bestLagrangian < lagrangian){
                bestLagrangian = lagrangian;
                bestLIndex = j;
            }
        }
        LU += bestLagrangian;
        sol[i][bestLIndex] = 1;
    }
    for(int k=0;k<dataSet.getNumberOfResources();k++){
        LU += U[k]*dataSet.getResource(k);
    }
    sol.setProfit(LU); //hack to use the profit to carry our L(U) value
    return sol;
}

float ACO_DataSetAdditions::getS_k(MMKPSolution sol,int r){
    float temp = 0;
    for(int i=0;i<sol.size();i++){
        for(int j=0;j<sol[i].size();j++){
            if(sol[i][j] == 1){
                temp += dataSet[i][j].getCost(r);
            }
        }
    }
    temp -= dataSet.getResource(r);
    return temp;
    
}

std::vector<ACO_Additions>& ACO_DataSetAdditions::operator[](int index){
    assert(index<this->classList.size());
    this->isUpdated = false;
    return this->classList.at(index);
}

void ACO_DataSetAdditions::update(){
    
    for(int i=0;i<this->classList.size();i++){
        float denomonator = 0.0;
        for(int j=0;j<this->classList[i].size();j++){
            denomonator += this->classList[i][j].pheramone
            *pow(this->classList[i][j].heuristic,this->B);
        }
        this->denominators[i] = denomonator;
    }
    
    this->isUpdated = true;
}

float ACO_DataSetAdditions::getProbability(int classNum, int itemNum){
    assert(classNum<this->classList.size());
    assert(itemNum<this->classList[classNum].size());
    
    if(!this->isUpdated){
        ACO_DataSetAdditions::update();
    }
    
    /*
     * Equation (3): phara_ij*heur_ij^B / summation(phara_ij * heur_ij^B)
     */
    float denomonator = this->denominators[classNum];
    float numerator = this->classList[classNum][itemNum].pheramone
        *pow(this->classList[classNum][itemNum].heuristic,this->B);
    
    return numerator / denomonator;
}

int ACO_DataSetAdditions::returnItemIndex(int classNum){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dif(0, 1);
    
    float r = dif(gen);
    for(int j=0;j<dataSet[classNum].size();j++){
        float temp = ACO_DataSetAdditions::getProbability(classNum,j);
        if(r < temp){
            return j;
        }
        r -= temp;
    }
    return -1;
}

int ACO_DataSetAdditions::size(){
    return this->classList.size();
}

float ACO_DataSetAdditions::getLStar(){
    return this->Lstar;
}

/* MMKP_ACO */

//constructors
MMKP_ACO::MMKP_ACO(MMKPDataSet dataSet, ACO_parameters parameters)
:MMKP_MetaHeuristic(dataSet,parameters),parameters(parameters),
solDesirability(dataSet,parameters.B,parameters.e){}

//overloaded operators
MMKPSolution MMKP_ACO::operator()(std::vector<MMKPSolution> initialPopulation){
    return MMKP_ACO::run(initialPopulation);
}

MMKPSolution MMKP_ACO::run(std::vector<MMKPSolution> initialPopulation){
    
    solDesirability.initParameters();
    
    std::vector<MMKPSolution> population = initialPopulation;
    
    MMKPSolution bestSolution;
    std::vector<MMKPSolution> bestSolutionOfGeneration;
    this->convergenceData.empty();
    this->convergenceIteration = 0;
    this->currentFuncEvals = 0;
    
    MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
    for(int i=0;i<population.size();i++){
        if(dataSet.isFeasible(population[i])){
            bestSolution = population[i];
            break;
        }
    }
    
    bool terminationCriterion = false;
    int currentGeneration = 0;
    
    //main loop
    while(!terminationCriterion){
        
        //reset population
        for(int i=0;i<population.size();i++){
            for(int j=0;j<population[i].size();j++){
                for(int k=0;k<population[i][j].size();k++){
                    population[i][j][k] = 0;
                }
            }
        }
        
        MMKP_ACO::constructSolutions(population);
        
        //make every solution feasible
        for(int i=0;i<population.size();i++){
            MMKP_MetaHeuristic::makeFeasible(population[i]);
            dataSet.updateSolution(population[i]);
        }
        
        //record best solution from population
        MMKP_MetaHeuristic::quickSort(population,0,(population.size()-1));
        for(int i=0;i<population.size();i++){
            if(dataSet.isFeasible(population[i])){
                bestSolutionOfGeneration.push_back(population[i]);
                break;
            }
        }
        if(bestSolution.getProfit()
           < bestSolutionOfGeneration[bestSolutionOfGeneration.size()-1].getProfit()){
            bestSolution = bestSolutionOfGeneration[bestSolutionOfGeneration.size()-1];
            this->convergenceIteration = currentGeneration;
        }
        
        updatePheramone(bestSolutionOfGeneration[(bestSolutionOfGeneration.size()-1)]);
        
        this->currentFuncEvals += population.size();
        std::tuple<int,float> temp(currentFuncEvals,bestSolution.getProfit());
        this->convergenceData.push_back(temp);
        
        if(currentGeneration >= this->parameters.numberOfGenerations){
            terminationCriterion = true;
        }
        currentGeneration++;
    }
    

    return bestSolution;
}

std::vector<MMKPSolution> MMKP_ACO::runOneGeneration
(std::vector<MMKPSolution> population){
    
    
    MMKP_ACO::constructSolutions(population);
    
    //make every solution feasible
    for(int i=0;i<population.size();i++){
        MMKP_MetaHeuristic::makeFeasible(population[i]);
        dataSet.updateSolution(population[i]);
    }
    
    return population;
}

void MMKP_ACO::constructSolutions(std::vector<MMKPSolution>& population){
    
    for(int i=0;i<population.size();i++){
        //get random permutation of class index's
        int currentClassI[population[i].size()];
        for(int j=0;j<population[i].size();j++){
            currentClassI[j] = j;
        }
        std::random_shuffle(currentClassI,currentClassI+population[i].size());
        
        for(int j=0;j<population[i].size();j++){
            int classChoosen = currentClassI[j];
            int itemChoosen = solDesirability.returnItemIndex(classChoosen);
            population[i][classChoosen][itemChoosen];
        }
    }
}

void MMKP_ACO::updatePheramone(MMKPSolution bestSolution){
    
    //find asymtotic t_max
    float tMax = 1/((1-this->parameters.p)
                    *(solDesirability.getLStar()-bestSolution.getProfit()));
    //set t_min as a function of t_max and rate coeffienct
    float tMin = this->parameters.e * tMax;
    
    for(int i=0;i<this->solDesirability.size();i++){
        for(int j=0;j<this->solDesirability[i].size();j++){
            
            float rememberedPharamone =
                this->parameters.p * this->solDesirability[i][j].pheramone;
            float diffP = 0;
            if(bestSolution[i][j] == 1){
                diffP = 1/(solDesirability.getLStar() - bestSolution.getProfit());
            }else{
                diffP = 0;
            }

            float updatePheramoneVal = rememberedPharamone+diffP;
            
            //check min&max requirements for updatedPheramoneValue
            if(updatePheramoneVal > tMax){
                updatePheramoneVal = tMax;
            }
            if(updatePheramoneVal < tMin){
                updatePheramoneVal = tMin;
            }
            
            this->solDesirability[i][j].pheramone = updatePheramoneVal;
        }
    }
    
}
