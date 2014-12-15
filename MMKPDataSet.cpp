/*********************************************************
 *
 * File: MMKPDataSet.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * Implementation of MMKPDataSet
 *
 *********************************************************/

#include "MMKPDataSet.h"

/* ITEM DATA */

/* value semantics */

ItemData::ItemData():profit(-1){}

ItemData::ItemData(float profit, std::vector<float> costs,
std::vector<float> problemResources):profit(profit),costs(costs){

    //analytic data init
    float costsSum = 0;
    float resourceSum = 0;
    
    //get sum of this costs
    for(int i=0;i<costs.size();i++){
        costsSum += costs[i];
    }
    
    //sum of availResources
    for(int i=0;i<problemResources.size();i++){
        resourceSum += problemResources[i];
    }
    
    //set constraintUsePercentSum Er%/n
    this->constraintUsePercentSum = (costsSum/resourceSum);
    
    //set profitConstraintUsePercen v/[Er%/n]
    this->profitConstraintUsePercent = profit/this->constraintUsePercentSum;
    
    std::vector<float> temp(this->costs.size());
    this->constraintUsePercent = temp;
    for(int i=0;i<constraintUsePercent.size();i++){
        constraintUsePercent[i] = (costs[i]/problemResources[i]);
    }
    this->profitConstraintRatio = temp;
    for(int i=0;i<constraintUsePercent.size();i++){
        profitConstraintRatio[i] = profit/constraintUsePercent[i];
    }
}

/* accessors */
float ItemData::getProfit()const{
    return this->profit;
}

std::vector<float> ItemData::getCosts() const{
    return this->costs;
}

float ItemData::getCost(int index) const{
    assert(index<this->costs.size());
    return this->costs.at(index);
}

int ItemData::getNumberOfCosts() const{
    return this->costs.size();
}

float ItemData::getProfitConstraintUsePercent() const{
    return this->profitConstraintUsePercent;
}

float ItemData::getProfitConstraintUsePercent
(std::vector<int> indices) const{
    float sum = 0;
    for(int i=0;i<indices.size();i++){
        sum += profitConstraintRatio.at(indices[i]);
    }
    return sum;
}

float ItemData::getConstraintUsePercent() const{
    return this->constraintUsePercentSum;
}

float ItemData::getConstraintUsePercent(int index){
    return this->constraintUsePercent.at(index);
}

float ItemData::getProfitConstraintUsePercent(int index){
    return this->profitConstraintRatio.at(index);
}

float ItemData::getConstraintUsePercent(std::vector<int> indices){
    float sum = 0;
    for(int i=0;i<indices.size();i++){
        sum += constraintUsePercent.at(indices[i]);
    }
    sum /= indices.size();
    return sum;
}

/* mutators */
void ItemData::setProfit(const float profit){
    this->profit = profit;
}

void ItemData::setCosts(const std::vector<float> costs){
    this->costs = costs;
    this->costs.shrink_to_fit();
}

void ItemData::setAnalytics(const std::vector<float> problemResources){
    //analytic data init
    float costsSum = 0;
    float resourceSum = 0;
    
    //get sum of this costs
    for(int i=0;i<this->costs.size();i++){
        costsSum += costs[i];
    }
    
    //sum of availResources
    for(int i=0;i<problemResources.size();i++){
        resourceSum += problemResources[i];
    }
    
    //set constraintUsePercentSum Er%/n
    this->constraintUsePercentSum = (costsSum/resourceSum);
    
    //set profitConstraintUsePercen v/[Er%/n]
    this->profitConstraintUsePercent = profit/this->constraintUsePercentSum;
    
    std::vector<float> temp(this->costs.size());
    this->constraintUsePercent = temp;
    for(int i=0;i<constraintUsePercent.size();i++){
        constraintUsePercent[i] = (costs[i]/problemResources[i]);
    }
    
}

//non-member functions
std::ostream& operator<<(std::ostream& os, const ItemData& obj){
    os<<"Profit: "<<obj.getProfit()<<std::endl;
    os<<"Costs: ";
    for(std::size_t i = 0; i<obj.getNumberOfCosts();i++){
        os<<obj.getCost(i)<<" ";
    }
    os<<std::endl;
    os<<"Er%/n: "<<obj.getConstraintUsePercent()<<std::endl;
    os<<"v/[Er%/n]: "<<obj.getProfitConstraintUsePercent();
    return os;
}

/* MMKPDataSet */

//value semantics
MMKPDataSet::MMKPDataSet(){}

MMKPDataSet::MMKPDataSet(std::vector<int> numberOfItemsPerClass,
            std::vector<float> constraints){
    //alloc constainers
    for(std::size_t i=0;i<numberOfItemsPerClass.size();i++){
        std::vector<ItemData> temp(numberOfItemsPerClass[i]
                                      ,ItemData());
        this->classList.push_back(temp);
    }
    this->resources = constraints;
    this->numberOfItemsInClasses = numberOfItemsPerClass;
}

//overloaded operators
std::vector<ItemData>& MMKPDataSet::operator[](int index){
    assert(index<this->classList.size());
    return this->classList.at(index);
}

//non-member functions
std::ostream& operator<<(std::ostream& os, MMKPDataSet& obj){
    
    //print resource's
    std::cout<<"Resources: ";
    for(std::vector<float>::iterator it=obj.resources.begin();
        it<obj.resources.end();it++){
        std::cout<<*it<<" ";
    }
    std::cout<<std::endl<<std::endl;
    
    //walk through and print each item in each class.
    for(std::vector<std::vector<ItemData> >::iterator it=obj.classList.begin();
        it<obj.classList.end();it++){
        std::cout<<"Class "<<
            distance(obj.classList.begin(),it)<<" data:"<<std::endl;
        for(std::vector<ItemData>::iterator jt=(*it).begin();jt<(*it).end();jt++){
            std::cout<<*jt<<std::endl<<std::endl;
        }
    }
    return os;
}

//accessors
std::size_t MMKPDataSet::size(){
    return this->classList.size();
}

std::vector<int> MMKPDataSet::getSizeOfEachClass(){
    return this->numberOfItemsInClasses;
}

int MMKPDataSet::getNumberOfResources(){
    return this->resources.size();
}

std::vector<float> MMKPDataSet::getResources(){
    return this->resources;
}

float MMKPDataSet::getResource(int index){
    assert(index<this->resources.size());
    return this->resources.at(index);
}

//mutators
void MMKPDataSet::setResources(const std::vector<float> resources){
    this->resources = resources;
}

//solution helpers
bool MMKPDataSet::isFeasible(MMKPSolution solution)const{
    if((MMKPDataSet::isMultiChoiceFeasible(solution)) &&
        (MMKPDataSet::isMultiDimFeasible(solution))){
        return true;
    }else{
        return false;
    }
}

std::vector<int> MMKPDataSet::getOffendingIndices(MMKPSolution solution) const{
    std::vector<int> temp;
    
    std::vector<float> cSum = solution.getCostsSummation();
    
    for(int i=0;i<cSum.size();i++){
        if(this->resources.at(i) < cSum.at(i)){
            temp.push_back(i);
        }
    }
    return temp;
}

bool MMKPDataSet::isMultiChoiceFeasible(MMKPSolution solution)const{
    bool isFeasible = true;
    for(std::size_t i=0;i<this->classList.size();i++){
        bool isSelected = false;
        for(std::size_t j=0;j<this->classList[i].size();j++){
            if((solution[i].at(j)) && (!isSelected)){
                isSelected = true;
            }else if((solution[i].at(j)) && (isSelected)){
                return false;
            }else{}
        }
        //check if none from class is selected
        if(!isSelected){
            return false;
        }
    }
    return true;
}

bool MMKPDataSet::isMultiDimFeasible(MMKPSolution solution)const{
    std::vector<float> constraints(solution.getCostsSummation());
    for(std::size_t i=0;i<this->resources.size();i++){
        if(constraints.at(i) > this->resources.at(i)){
            return false;
        }
    }
    return true;
}

void MMKPDataSet::updateSolution(MMKPSolution& solution){
    std::vector<float> cSum(resources.size(),0);
    float profit = 0;
    for(std::size_t i=0;i<solution.size();i++){
        for(std::size_t j=0;j<solution[i].size();j++){
            if(solution[i].at(j)){
                profit += (this->classList.at(i)).at(j).getProfit();
                //get sum of contraints && profit
                std::vector<float> temp((this->classList.at(i).at(j)).getCosts());
                for(std::size_t k=0;k<temp.size();k++){
                    cSum[k] += temp[k];
                }
            }
        }
    }
    solution.setProfit(profit);
    solution.setCosts(cSum);
}

float MMKPDataSet::getVariableConstraintUseDiff(MMKPSolution& solution){
    std::vector<int> offI = MMKPDataSet::getOffendingIndices(solution);
    float tempCost = 0;
    float tempResource = 0;
    
    for(int i=0;i<offI.size();i++){
        tempCost += solution.getCost(offI.at(i));
        tempResource += MMKPDataSet::getResource(offI.at(i));
    }
    
    assert(tempCost > tempResource);    //should be since all resources
                                        //are violated.
    return (1-(tempResource/tempCost));
}

/* ORLIB_READ */
MMKPDataSet OrLib_Read::operator()(std::ifstream& file){
    //read general problem data
    int classSize;
    int itemsPerClassSize;
    int resourceSize;
    std::vector<float> resources;
    int buffer;
    
    file>>classSize;
    file>>itemsPerClassSize;
    //these problems are all equal number of items in each class,
    //so fill vector with same data to pass into MMKPDataSet
    std::vector<int> itemsPerClass(classSize,itemsPerClassSize);
    file>>resourceSize;
    
    for(std::size_t i=0;i<resourceSize;i++){
        file>>buffer;
        resources.push_back(buffer);
    }
    
    //add to dataset
    MMKPDataSet dataSet(itemsPerClass,resources);

    //read each class information
    //for each class
    for(std::size_t i=0;i<classSize;i++){
        file>>buffer;   //eat each class number
        //for each item in each class
        for(std::size_t j=0;j<itemsPerClass[i];j++){
            //for each constraint
            float profit;
            std::vector<float> constraints;
            file>>profit;
            for(std::size_t k=0;k<resourceSize;k++){
                file>>buffer;
                constraints.push_back(buffer);
            }
            //add to dataSet
            ItemData temp(profit,constraints,resources);
            dataSet[i][j] = temp;
        }
    }
    
    return dataSet;
}


MMKPDataSet HiremathHill_Read::operator()(std::ifstream& file,int problemToRun){
    
    //read general problem data
    int classSize;
    int itemsPerClassSize;
    int resourceSize;
    std::vector<float> resources;
    int buffer;
    int problemNumber;
    MMKPDataSet dataSet;
    
    do{
        resources.clear();
        file>>problemNumber;
        file>>classSize;
        file>>itemsPerClassSize;
        //these problems are all equal number of items in each class,
        //so fill vector with same data to pass into MMKPDataSet
        std::vector<int> itemsPerClass(classSize,itemsPerClassSize);
        file>>resourceSize;
        
        for(std::size_t i=0;i<resourceSize;i++){
            file>>buffer;
            resources.push_back(buffer);
        }
        //add to dataset
        MMKPDataSet temp(itemsPerClass,resources);
        dataSet = temp;
        
        //read each class information
        //for each class
        for(std::size_t i=0;i<classSize;i++){
            file>>buffer;   //eat each class number
            //for each item in each class
            for(std::size_t j=0;j<itemsPerClass[i];j++){
                //for each constraint
                float profit;
                std::vector<float> constraints;
                file>>profit;
                for(std::size_t k=0;k<resourceSize;k++){
                    file>>buffer;
                    constraints.push_back(buffer);
                }
                //add to dataSet
                ItemData temp(profit,constraints,resources);
                dataSet[i][j] = temp;
            }
        }
    }while(problemNumber != problemToRun);
    
    return dataSet;
}






