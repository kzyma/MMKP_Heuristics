/******************************************************************
 *
 * File: CoaApp.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Application.
 *
 * Usage:
 *  filename <folder><name><number(optional)<mods><popSize><genMax>
 *           <Horizontal Crossover Probability><Vertical Crossover Probability>
 *
 *                      or
 *
 *  filename <folder><name><number(optional)<mods><popSize><genMax>
 *           <Horizontal Crossover Probability><Vertical Crossover Probability><Seed>
 *
 *      folder: The folder containing the problem.
 *      name: filename of the problem
 *      number(optional): some problem sets house multiple problems
 *          in one file, this is a way to specify which to run. If
 *          left out and the file has more than 1, the first will
 *          be run.
 *      mods: modifications to the tlbo algorithm. see below.
 *      popSize: population size of Tlbo
 *      genSize: number of generations to run Tlbo.
 *      classSize: size of a 'classroom'. Introduced by i-tlbo variation, where
 *          a poplulation may be split into many 'classroom's', each with
 *          there own respective teacher. 0 indicates the class size equals
 *          population size.
 *      Seed: random number seed for initial population, so all populations will
 *          start with the same elements (as long as the same pop gen and size are
 *          used).
 *
 *
 *      *mods*
 *      The mods to Tlbo are represented as a numeric string, for
 *      example:
 *                               001
 *      ,where each place represents one mod, and that value dictates
 *      which modification to make. The order is as follow, top->bottom
 *      is left->right.
 *
 *      <PopulationGenMod>
 *      <MultipleChoiceFeasibilityMod>
 *      <MultipleDimFeasilbilityMod>
 *
 *      PopulationGenMod:
 *          0: Generate Randomized Initial Population (duplicate
 *              solutions are allowed)
 *          1: Generate Randomized Initial Population (duplicate
 *              solutions not allowed)
 *
 *          2: Generate Randomized Initial Population (duplcate
 *              solutions not allowed and infeasible solutions
 *              are allowed.
 *
 *      MultipleChoiceFeasibilityMod:
 *          0: If one item is selected in a class, continue. If more than one
 *              item selected, choose the one with highest v/[E_r%/n] value. If
 *              none selected, choose item from class with highest v/[E_r%/n].
 *
 *          1: Same as 0, except if no items are selected, then randomly choose
 *              an item from the class.
 *
 *          2: If one item is selected, continue. If more than one item is
 *              selected, choose the item with the highest profit. If none are
 *              selected choose the item with the highest profit.
 *
 *      MultipleDimFeasilbilityMod:
 *          0: Use n, max diff, fixed size surrogate constraint to obtain
 *              multiple dimension feasibility, where n is the
 *              number of constraints of a problem. Additionally, maximum
 *              difference of surrogate values used to quickly obtain
 *              feasbility.
 *          1: Use variable, max diff, surrogate constraint to obtain
 *              multiple dimension feasbility. Similar to (0), the max
 *              difference of surrogate values are used to quickly
 *              obtain feasilibty. However, this method calculates
 *              it's surrogate constraint values based not on all problem
 *              consraints, but only the ones violated.
 *
 *      	2: Similar to #1, this method uses a variable size surrogate
 *             constraint equal to the number of violated constraints.
 *             However, this method uses the minimum difference to slowly
 *             approach feasibility, in the hopes of getting a better solution.
 *
 *
 *******************************************************************/

#include <iostream>
#include <stdlib.h> //atoi
#include <time.h>
#include <iomanip>

#include "MMKPDataSet.h"
#include "MMKPSolution.h"
#include "MMKP_COA.h"
#include "MMKPPopulationGenerators.h"

int main(int argc, char* argv[]){
    
    std::string folder = "orlib_data";
    std::string file = "I01";
    int problem = 1;
    std::string mods = "1";
    int popSize = 30;
    int genSize = 40;
    float horizCross = 1.0;
    float vertCross = 1.0;
    unsigned int seed = 1234;
    
    if(argc==9){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        mods = argv[4];
        popSize = atoi(argv[5]);
        genSize = atoi(argv[6]);
        horizCross = atof(argv[7]);
        vertCross = atof(argv[8]);
    }else if(argc==10){
        folder = argv[1];
        file = argv[2];
        problem = atoi(argv[3]);
        mods = argv[4];
        popSize = atoi(argv[5]);
        genSize = atoi(argv[6]);
        horizCross = atof(argv[7]);
        vertCross = atof(argv[8]);
        seed = atoi(argv[9]);
    }else{
        std::cout<<"usage: filename <folder><name><number<mods><popSize><genSize>\
        <Horizontal Crossover Probability><Vertical Crossover Probability>";
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
    COA_parameters parameters = DEFAULT_COA_PARAMETERS;
    parameters.populationSize = popSize;
    parameters.numberOfGenerations = genSize;
    parameters.multipleChoiceFeasibilityMod = mods[1] - '0';
    parameters.multipleDimFeasibilityMod = mods[2] - '0';
    parameters.horizCrossProb = horizCross;
    parameters.verticalCrossProb = vertCross;
    MMKP_COA COA(dataSet,parameters);
    
    /* POPULATION GENERATION */
    PopulationGenerator* populationGenerator[4];
    if(argc != 10){
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
    
    COA.quickSort(initPopulation,0,(initPopulation.size()-1));
    MMKPSolution initialTeacher;
    initialTeacher.setProfit(-1);
    for(int i=0;i<initPopulation.size();i++){
        if(dataSet.isFeasible(initPopulation[i])){
            initialTeacher = initPopulation[i];
            break;
        }
    }
    
    //free pop-gen array allocated mem
    delete populationGenerator[0];
    delete populationGenerator[1];
    delete populationGenerator[2];
    delete populationGenerator[3];
    
    t1=clock();
    MMKPSolution optimalSolution = COA(initPopulation);
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





