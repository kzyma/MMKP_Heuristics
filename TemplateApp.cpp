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

#include "MMKPDataSet.h"
#include "MMKPSolution.h"
#include "MMKP_BBA.h"
#include "MMKPPopulationGenerators.h"

int main(int argc, char* argv[]){
    
    std::string folder = "orlib_data";
    std::string file = "I01";
    int problem = 1;
    std::string mods = "1";
    int popSize = 30;
    int genSize = 40;
    unsigned int seed = 1234;
    
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
        std::cout<<"usage: filename <folder><name><number><mods><popSize><genSize>";
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
    
    /*any init for algorithm goes here */
    BBA_parameters parameters;
    parameters.populationSize = popSize;
    parameters.numberOfGenerations = genSize;
    parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
    parameters.multipleDimFeasibilityMod = mods[2] - '0';
    MMKP_BBA BBA(dataSet,parameters);
    
    /* POPULATION GENERATION */
    PopulationGenerator* populationGenerator[4];
    if(argc != 8){
        populationGenerator[0] = new GenerateRandomizedPopulation();
        populationGenerator[1] = new GenerateRandomizedPopulationNoDups();
        populationGenerator[2] = new GenerateRandomizedPopulationNoDups_Infeasible();
        populationGenerator[3] = new GenerateRandomizedPopulationGreedyV1();
    }else{
        populationGenerator[0] = new GenerateRandomizedPopulation(seed);
        populationGenerator[1] = new GenerateRandomizedPopulationNoDups(seed);
        populationGenerator[2] = new GenerateRandomizedPopulationNoDups_Infeasible(seed);
        populationGenerator[3] = new GenerateRandomizedPopulationGreedyV1(seed);
    }
    
    int populationGenIndx = mods[0] - '0';
    std::vector<MMKPSolution> initPopulation
        = (*(populationGenerator[populationGenIndx]))(dataSet,popSize);
    
    MMKPSolution initialTeacher;
    initialTeacher.setProfit(-1);
    
    for(int i=0;i<initPopulation.size();i++){
        if(dataSet.isFeasible(initPopulation[i])&&(initialTeacher.getProfit() <
                                                initPopulation[i].getProfit())){
            initialTeacher = initPopulation[i];
        }
    }
    
    //free pop-gen array allocated mem
    delete populationGenerator[0];
    delete populationGenerator[1];
    delete populationGenerator[2];
    delete populationGenerator[3];
    
    t1=clock();
    MMKPSolution optimalSolution = BBA(initPopulation);
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





