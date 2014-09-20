/******************************************************************
 *
 * File: DoApp.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Darwin's Observation MMKP Application.
 *
 *
 *******************************************************************/

#include <iostream>
#include <stdlib.h> //atoi
#include <time.h>
#include <iomanip>
#include <random>

#include "MMKPDataSet.h"
#include "MMKPSolution.h"
#include "MMKP_TLBO.h"
#include "MMKP_COA.h"
#include "MMKP_GA.h"
#include "MMKP_MetaHeuristic.h"
#include "MMKPPopulationGenerators.h"

bool doesContain(std::vector<MMKPSolution> population,MMKPSolution sol);

int main(int argc, char* argv[]){
    
    std::string folder = "orlib_data";
    std::string file = "I01";
    int problem = 1;
    int popSize = 30;
    int genSize = 40;
    unsigned int seed = 1234;
    std::string mods = "";
    
    if(argc==7){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        mods = argv[4];
        popSize = atoi(argv[5]);
        genSize = atoi(argv[6]);
    }else if(argc==8){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        mods = argv[4];
        popSize = atoi(argv[5]);
        genSize = atoi(argv[6]);
        seed = atoi(argv[7]);
    }else{
        std::cout<<"usage: filename <folder><name><number><popSize><genSize>\n";
        return 0;
    }
    
    std::ifstream fileStream;
    MMKPDataSet dataSet;
    clock_t t1,t2;
    float runtime;
    fileStream.open(folder+std::string("/")+file);
    
    /* READ INPUT */
    if(fileStream.is_open()){
        //build MMKPDataSet object
        if(folder=="orlib_data"){
            OrLib_Read readInput;
            dataSet = readInput(fileStream);
        }else if(folder=="HiremathHill_data"){
            HiremathHill_Read readInput;
            dataSet = readInput(fileStream,problem);
        }else{
            std::cout<<"Error, unrecognized folder name."<<std::endl;
        }
    }else{
        std::cerr<<std::string("File ")+file+std::string(" failed to open.")
        <<std::endl;
    }
    fileStream.close();
    
    //generate initial population
    GenerateRandomizedPopulationNoDups generatePopulation;
    std::vector<MMKPSolution> population
    = generatePopulation(dataSet,popSize);
    
    MMKP_MetaHeuristic** algorithms = new MMKP_MetaHeuristic*[3];
    algorithms[0] = new MMKP_TLBO(dataSet);
    algorithms[1] = new MMKP_COA(dataSet);
    algorithms[2] = new MMKP_GA(dataSet);

    MMKPSolution optimalSolution;
    //find first feasible solution to make initial optimal solution
    for(int i=0;i<population.size();i++){
        if(dataSet.isFeasible(population[i])){
            optimalSolution = population[i];
            break;
        }
    }
    //find best solution
    for(int i=0;i<population.size();i++){
        if(dataSet.isFeasible(population[i])){
            if(optimalSolution.getProfit() < population[i].getProfit()){
                optimalSolution = population[i];
            }
        }
    }
    
    MMKPSolution initialTeacher = optimalSolution;
    
    t1=clock();
    int countNOP = 0;
    int maxNOP = 5;
    int currentAlgorithm = 0;
    int numberOfAlgorithms = 3;
    bool solIncrease = false;
    
    for(int i=0;i<genSize;i++){
        
        if(countNOP > maxNOP){
            //next algorithm is active
            currentAlgorithm++;
            countNOP = 0;
            if(currentAlgorithm >= numberOfAlgorithms){
                currentAlgorithm = 0;
            }
        }

        population
            = algorithms[currentAlgorithm]->runOneGeneration(population);
        
        //find new best solution
        bool solIncrease = false;
        for(int i=0;i<population.size();i++){
            if(dataSet.isFeasible(population[i])){
                if(optimalSolution.getProfit() < population[i].getProfit()){
                    optimalSolution = population[i];
                    solIncrease = true;
                }
            }
        }
        if(solIncrease){
            countNOP = 0;
        }else{
            countNOP ++;
        }
    }
    
    t2 = clock();
    
    delete algorithms[0];
    delete algorithms[1];
    delete algorithms[2];
    delete[] algorithms;
    
    runtime = ((float)t2-(float)t1)/(double) CLOCKS_PER_SEC;
    
    std::cout<<"Problem: "<<std::endl;
    std::cout<<folder<<std::string("/")<<file<<std::endl;
    std::cout<<"Problem Number:"<<std::endl;
    std::cout<<problem<<std::endl;
    std::cout<<"Initial Profit:"<<std::endl;
    std::cout<<initialTeacher.getProfit()<<std::endl;
    std::cout<<"Profit:"<<std::endl;
    if(dataSet.isFeasible(optimalSolution)){
        std::cout<<optimalSolution.getProfit()<<std::endl;
    }else{
        std::cout<<0<<std::endl;
    }
    std::cout<<"Runtime:"<<std::endl;
    std::cout<<runtime<<std::endl;
    std::cout<<"Num of Classes:"<<std::endl;
    std::cout<<optimalSolution.size()<<std::endl;   //num of classes
    for(int i=0;i<optimalSolution.size();i++){
        for(int j=0;j<optimalSolution[i].size();j++){
            std::cout<<optimalSolution[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    return 0;
}

bool doesContain(std::vector<MMKPSolution> population,MMKPSolution sol){
    for(int i=0;i<population.size();i++){
        if(population[i].getProfit() == sol.getProfit()){
            return true;
        }
    }
    return false;
}




