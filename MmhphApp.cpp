/******************************************************************
 *
 * File: BbaApp.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * ACO MMKP Application.
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
#include "MMKPPopulationGenerators.h"

int main(int argc, char* argv[]){
    
    std::string folder = "orlib_data";
    std::string file = "I01";
    int problem = 1;
    int popSize = 30;
    int genSize = 40;
    unsigned int seed = 1234;
    
    if(argc==6){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        popSize = atoi(argv[4]);
        genSize = atoi(argv[5]);
    }else if(argc==7){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        popSize = atoi(argv[4]);
        genSize = atoi(argv[5]);
        seed = atoi(argv[6]);
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
    
    //algs used
    MMKP_TLBO tlbo(dataSet);
    MMKP_COA coa(dataSet);
    MMKP_GA ga(dataSet);

    tlbo.quickSort(population,0,population.size()-1);
    MMKPSolution optimalSolution;
    for(int i=0;i<population.size();i++){
        if(dataSet.isFeasible(population[i])){
            optimalSolution = population[i];
            break;
        }
    }
    MMKPSolution initialTeacher = optimalSolution;
    
    t1=clock();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    
    for(int i=0;i<genSize;i++){
        
        //divide population by distribution
        std::vector<MMKPSolution> pop1;
        std::vector<MMKPSolution> pop2;
        std::vector<MMKPSolution> pop3;
        
        for(int j=0;j<population.size();j++){
            int p = dis(gen);
            switch(p){
                case 0:
                    pop1.push_back(population[j]);
                    break;
                case 1:
                    pop2.push_back(population[j]);
                    break;
                case 2:
                    pop3.push_back(population[j]);
                    break;
            }
        }
        
        pop1 = tlbo.runOneGeneration(pop1);
        pop2 = coa.runOneGeneration(pop2);
        pop3 = ga.runOneGeneration(pop3);
        
        population.clear();
        population.reserve(pop1.size() + pop2.size() + pop3.size());
        population.insert(population.end(), pop1.begin(), pop1.end());
        population.insert(population.end(), pop2.begin(), pop2.end());
        population.insert(population.end(), pop3.begin(), pop3.end());
        
        //get and save best solution
        tlbo.quickSort(population,0,population.size()-1);
        for(int j=0;j<population.size();j++){
            if(dataSet.isFeasible(population[j])){
                if(population[j].getProfit()
                   > optimalSolution.getProfit()){
                    optimalSolution = population[j];
                    break;
                }
            }
        }
    }
    
    t2 = clock();
    
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





