/******************************************************************
 *
 * File: HeuristicApp.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Main driver for running/testing MMKP problem instances. Takes command
 * line arguments folder, file, problem number, algorithm (ex. tlbo), 
 * algorithm mods (ex. 0010), population size, and number of generations.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) [2015] [Kutztown University]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************/

#include <iostream>
#include <stdlib.h> //atoi
#include <time.h>
#include <string>

#include "MMKPDataSet.h"
#include "MMKPSolution.h"
#include "MMKPPopulationGenerators.h"
#include "MMKP_ABC.h"
#include "MMKP_ACO.h"
#include "MMKP_BBA.h"
#include "MMKP_COA.h"
#include "MMKP_GA.h"
#include "MMKP_TLBO.h"
#include "MMKP_LocalSearch.h"
#include "MMKP_PSO.h"

int main(int argc, char* argv[]){
    
    unsigned int seed = 1234;
    
    std::string folder = argv[1];
    std::string file = argv[2];
    int problem = atoi(argv[3]);
    std::string alg = argv[4];
    std::string mods = argv[5];
    int popSize = atoi(argv[6]);
    int genSize = atoi(argv[7]);
    
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
    
    MMKP_MetaHeuristic* algorithm;
    
    if(alg.compare("aco")==0){
            if(argc != 11){std::cout<<"Invalid args for aco.\n";return 0;}
            ACO_parameters parameters;
            parameters.B = atof(argv[8]);
            parameters.p = atof(argv[9]);
            parameters.e = atof(argv[10]);
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            algorithm = new MMKP_ACO(dataSet,parameters);
    }else if(alg.compare("coa")==0){
            if(argc != 10){std::cout<<"Invalid args for coa.\n";return 0;}
            COA_parameters parameters;
            parameters.horizCrossProb = atof(argv[8]);
            parameters.verticalCrossProb = atof(argv[9]);
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            algorithm = new MMKP_COA(dataSet,parameters);
    }else if(alg.compare("bba")==0){
            if(argc != 8){std::cout<<"Invalid args for coa.\n";return 0;}
            BBA_parameters parameters;
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            algorithm = new MMKP_BBA(dataSet,parameters);
    }else if(alg.compare("ga")==0){
            if(argc != 10){std::cout<<"Invalid args for ga.\n";return 0;}
            GA_parameters parameters;
            parameters.parentPoolSizeT = atoi(argv[8]);
            parameters.mutateProb = atof(argv[9]);
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            algorithm = new MMKP_GA(dataSet,parameters);
    }else if(alg.compare("abc")==0){
            if(argc != 8){std::cout<<"Invalid args for abc.\n";return 0;}
            ABC_parameters parameters;
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            algorithm = new MMKP_ABC(dataSet,parameters);
    }else if(alg.compare("pso")==0){
            if(argc != 9){std::cout<<"Invalid args for pso.\n";return 0;}
            PSO_parameters parameters;
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            parameters.learningFactor = atof(argv[8]);
            algorithm = new MMKP_PSO(dataSet,parameters);
    }else if(alg.compare("tlbo")==0){
            if(argc != 10){std::cout<<"Invalid args for tlbo.\n";return 0;}
            TLBO_parameters parameters;
            parameters.numberOfGenerations = genSize;
            parameters.populationSize = popSize;
            parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
            parameters.multipleDimFeasibilityMod = mods[2] - '0';
            parameters.alg_Type = atof(argv[8]);
            parameters.rls_on = atof(argv[9]);
            algorithm = new MMKP_TLBO(dataSet,parameters);
    }else{
            std::cout<<"Cannot recognize algorithm, try again."<<std::endl;
            return 0;
            
    }
    
    /* POPULATION GENERATION */
    PopulationGenerator* populationGenerator[4];
    populationGenerator[0] = new GenerateRandomizedPopulation(seed);
    populationGenerator[1] = new GenerateRandomizedPopulationNoDups(seed);
    populationGenerator[2] = new GenerateRandomizedPopulationNoDups_Infeasible(seed);
    populationGenerator[3] = new GenerateRandomizedPopulationGreedyV1(seed);
    
    int populationGenIndx = mods[0] - '0';
    std::vector<MMKPSolution> initPopulation
        = (*(populationGenerator[populationGenIndx]))(dataSet,popSize);
    
    algorithm->quickSort(initPopulation,0,(initPopulation.size()-1));
    MMKPSolution initialBest;
    initialBest.setProfit(-1);
    for(int i=0;i<initPopulation.size();i++){
        if(dataSet.isFeasible(initPopulation[i])){
            initialBest = initPopulation[i];
            break;
        }
    }
    
    //free pop-gen array allocated mem
    delete populationGenerator[0];
    delete populationGenerator[1];
    delete populationGenerator[2];
    delete populationGenerator[3];
    
    t1=clock();
    MMKPSolution optimalSolution = (*algorithm)(initPopulation);
    
    //local search heuristics
    if((mods[3] - '0') == 1){
        CompLocalSearch CLS(dataSet);
        optimalSolution = CLS(optimalSolution);
    }
    if((mods[3] - '0') == 2){
        ReactiveLocalSearch RLS(dataSet);
        optimalSolution = RLS(optimalSolution);
    }
    
    t2 = clock();
    runtime = ((float)t2-(float)t1)/(double) CLOCKS_PER_SEC;
    std::vector<std::tuple<int,float> > convData = algorithm->getConvergenceData();
    
    std::cout<<"Problem: "<<std::endl;
    std::cout<<folder<<std::string("/")<<file<<std::endl;
    std::cout<<"Problem Number:"<<std::endl;
    std::cout<<problem<<std::endl;
    std::cout<<"Starting Best Sol:"<<std::endl;
    std::cout<<initialBest.getProfit()<<std::endl;
    std::cout<<"Profit:"<<std::endl;
    if(dataSet.isFeasible(optimalSolution)){
        std::cout<<optimalSolution.getProfit()<<std::endl;
    }else{
        std::cout<<0<<std::endl;
    }
    std::cout<<"Runtime:"<<std::endl;
    std::cout<<runtime<<std::endl;
    std::cout<<"Sol Found in _ Iterations"<<std::endl;
    std::cout<<algorithm->getConvergenceGeneration()<<std::endl;
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
    
    delete algorithm;
    
    return 0;
}





