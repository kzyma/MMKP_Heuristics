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
    
    int eliteSolutionSize = 5;
    std::vector<MMKPSolution> eliteSolutions;
    
    int count = 0;
    for(int i=0;i<population.size();i++){
        dataSet.updateSolution(population[i]);
        if(dataSet.isFeasible(population[i])){
            eliteSolutions.push_back(population[i]);
            count++;
        }
        if(count > eliteSolutionSize){
            break;
        }
    }
    
    int convergenceGen = 0;
    std::vector<std::tuple<int, float> > convData;
    int functionEvalCounter = 0;

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
    
        //replace worse 3 solutions of each population with
        //elite solutions
        tlbo.quickSort(pop1,0,pop1.size()-1);
        tlbo.quickSort(pop2,0,pop2.size()-1);
        tlbo.quickSort(pop3,0,pop3.size()-1);
        
        for(int j=0;j<eliteSolutions.size();j++){
            pop1[((pop1.size()-1)-j)] = eliteSolutions[j];
            pop2[((pop2.size()-1)-j)] = eliteSolutions[j];
            pop3[((pop3.size()-1)-j)] = eliteSolutions[j];
        }
        
        pop1 = tlbo.runOneGeneration(pop1);
        functionEvalCounter += pop1.size()*2;
        pop2 = coa.runOneGeneration(pop2);
        functionEvalCounter += pop2.size()*2;
        pop3 = ga.runOneGeneration(pop3);
        functionEvalCounter += pop3.size();

        population.clear();
        population.reserve(pop1.size() + pop2.size() + pop3.size());
        population.insert(population.end(), pop1.begin(), pop1.end());
        population.insert(population.end(), pop2.begin(), pop2.end());
        population.insert(population.end(), pop3.begin(), pop3.end());
        
        //get and save best solution
        //and update our elite list
        count = 0;
        tlbo.quickSort(population,0,population.size()-1);
        for(int j=0;j<population.size();j++){
            dataSet.updateSolution(population[j]);
            if(dataSet.isFeasible(population[j])){
                if(population[j].getProfit()
                   > optimalSolution.getProfit()){
                    optimalSolution = population[j];
                    convergenceGen = i+1;
                }
                if(!(doesContain(eliteSolutions,population[j]))){
                    eliteSolutions[count] = population[j];
                    count++;
                }
                if(count > eliteSolutionSize){
                    break;
                }
            }
        }
        ReactiveLocalSearch RLS(dataSet);
        //run local search on elite solutions
        eliteSolutions = RLS(eliteSolutions);
        
        //update conv. vector
        std::tuple<int, float> temp(functionEvalCounter,optimalSolution.getProfit());
        convData.push_back(temp);
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
    std::cout<<"Sol Found in _ Iterations"<<std::endl;
    std::cout<<convergenceGen<<std::endl;
    std::cout<<"Convergence Count:"<<std::endl;
    std::cout<<convData.size()<<std::endl;
    for(int i=0;i<convData.size();i++){
        std::cout<<std::get<0>(convData[i])<<std::endl;
        std::cout<<std::get<1>(convData[i])<<std::endl;
    }
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




