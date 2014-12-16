/*********************************************************
 *
 * File: MMKPPopulationGenerators.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Population Generators Implementation.
 *
 *********************************************************/

#include "MMKPPopulationGenerators.h"

/* POPULATION GENERATOR */
PopulationGenerator::~PopulationGenerator(){
    //do nothing
}

//test if solution is in population. Return false if it is, true otherwise.
bool PopulationGenerator::notIncluded
(MMKPSolution solution,std::vector<MMKPSolution> population){
    for(int i=0;i<population.size();i++){
        bool isSame = true;
        for(int j=0;j<population[i].size();j++){
            for(int k=0;k<population[i][j].size();k++){
                if(solution[j].at(k) != population[i][j].at(k)){
                    isSame = false;
                }
            }
        }
        if(isSame == true)
            return false;
    }
    return true;
}

/* GENERATE RANDOMIZED POPULATION */

GenerateRandomizedPopulation::GenerateRandomizedPopulation():seed(time(NULL)){}

GenerateRandomizedPopulation::GenerateRandomizedPopulation(unsigned int seed)
:seed(seed){}

std::vector<MMKPSolution> GenerateRandomizedPopulation::operator()
(MMKPDataSet dataSet,int populationSize){
    
    int solutionsFound = 0;
    srand(this->seed);
    std::vector<MMKPSolution> population;
    
    while(solutionsFound < populationSize){
        MMKPSolution temp(dataSet.getSizeOfEachClass());
        for(int i=0;i<temp.size();i++){
            int randomIndx = rand() % temp[i].size();
            temp[i].at(randomIndx) = true;
        }
        dataSet.updateSolution(temp);
        
        if(dataSet.isFeasible(temp)){
            population.push_back(temp);
            solutionsFound++;
        }
    }
    
    assert(population.size() == populationSize);
    return population;
}

/* GENERATE RANDOMIZED POPULATION w/o DUPLICATES */

GenerateRandomizedPopulationNoDups::GenerateRandomizedPopulationNoDups()
:seed(time(NULL)){}

GenerateRandomizedPopulationNoDups::GenerateRandomizedPopulationNoDups
(unsigned int seed):seed(seed){}

std::vector<MMKPSolution> GenerateRandomizedPopulationNoDups::operator()
(MMKPDataSet dataSet,int populationSize){
    
    int solutionsFound = 0;
    srand(this->seed);
    std::vector<MMKPSolution> population;
    
    while(solutionsFound < populationSize){
        MMKPSolution temp(dataSet.getSizeOfEachClass());
        for(int i=0;i<temp.size();i++){
            int randomIndx = rand() % temp[i].size();
            temp[i].at(randomIndx) = true;
        }
        dataSet.updateSolution(temp);
        
        if((dataSet.isFeasible(temp))&&(notIncluded(temp,population))){
            population.push_back(temp);
            solutionsFound++;
        }
    }
    
    assert(population.size() == populationSize);
    return population;
}

/* GENERATE RANDOMIZED POPULATION w/o DUPLICATES/INFEASIBLE */

GenerateRandomizedPopulationNoDups_Infeasible::
GenerateRandomizedPopulationNoDups_Infeasible()
:seed(time(NULL)){}

GenerateRandomizedPopulationNoDups_Infeasible::
GenerateRandomizedPopulationNoDups_Infeasible
(unsigned int seed):seed(seed){}

std::vector<MMKPSolution> GenerateRandomizedPopulationNoDups_Infeasible::operator()
(MMKPDataSet dataSet,int populationSize){
    
    int solutionsFound = 0;
    srand(this->seed);
    std::vector<MMKPSolution> population;
    
    while(solutionsFound < populationSize){
        MMKPSolution temp(dataSet.getSizeOfEachClass());
        for(int i=0;i<temp.size();i++){
            int randomIndx = rand() % temp[i].size();
            temp[i].at(randomIndx) = true;
        }
        dataSet.updateSolution(temp);
        if(notIncluded(temp,population)){
            population.push_back(temp);
            solutionsFound++;
        }
    }
    
    assert(population.size() == populationSize);
    return population;
}


/* GENERATE GREEDY POPULATION */

GenerateRandomizedPopulationGreedyV1::
GenerateRandomizedPopulationGreedyV1()
:seed(time(NULL)){}

GenerateRandomizedPopulationGreedyV1::
GenerateRandomizedPopulationGreedyV1
(unsigned int seed):seed(seed){}

std::vector<MMKPSolution> GenerateRandomizedPopulationGreedyV1::operator()
(MMKPDataSet dataSet,int populationSize){
    int solutionsFound = 0;
    srand(this->seed);
    std::vector<MMKPSolution> population;
    
    while(solutionsFound < populationSize){

        MMKPSolution temp(dataSet.getSizeOfEachClass());
        //generate indices for profit/constraint ratio calculation
        int numOfIndices = (rand() % dataSet.getNumberOfResources()) +1;
        std::vector<int> constrIndices;
        
        for(int i=0;i<numOfIndices;){
            int temp = (rand()%dataSet.getNumberOfResources());
            bool isIn = false;
            for(int j=0;j<constrIndices.size();j++){
                if(temp == constrIndices[j]){
                    isIn = true;
                }
            }
            if(!isIn){
                constrIndices.push_back(temp);
                i++;
            }
        }
        //randomly select a  weighting for v/[Er%/n]
        std::vector<std::function<float(float p,float c)> > functors;
        functors.push_back([&](float p,float c){return p/c;});
        functors.push_back([&](float p,float c){return ((p*(rand()%100))/c);});
        functors.push_back([&](float p,float c){return p;});
        functors.push_back([&](float p,float c){return c;});
        functors.push_back([&](float p,float c){return (rand()%100+1);});
        int index = rand()%functors.size();
        std::function<float(float p,float c)> surrFunction = functors[index];
    
        //run greedy
        for(int i=0;i<dataSet.size();i++){
            int largeIndex = 0;
            float largeVal = surrFunction(dataSet[i][0].getProfit(),
                            dataSet[i][0].getConstraintUsePercent(constrIndices));
            for(int j=0;j<dataSet[i].size();j++){
                float tempVal = surrFunction(dataSet[i][j].getProfit(),
                        dataSet[i][j].getConstraintUsePercent(constrIndices));
                if(tempVal > largeVal){
                    largeIndex = j;
                    largeVal = tempVal;
                }
            }
            temp[i].at(largeIndex) = true;
        }
        
        dataSet.updateSolution(temp);
        if(notIncluded(temp,population)){
            population.push_back(temp);
            solutionsFound++;
        }
    }
    assert(population.size() == populationSize);
    return population;
}

               
               
               
               
