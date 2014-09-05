/*********************************************************
 *
 * File: MMKPSolution.cpp
 * Author: Ken Zyma
 *
 * @All rights reserved
 * Kutztown University, PA, U.S.A
 *
 * TLBO MMKP Problem
 *
 *********************************************************/

#include "MMKPSolution.h"

/* MMKPSolution */

MMKPSolution::MMKPSolution():profitSummation(0){}

MMKPSolution::MMKPSolution(std::vector<int> numberOfItemsPerClass){
    for(std::size_t i=0;i<numberOfItemsPerClass.size();i++){
        std::vector<bool> temp(numberOfItemsPerClass[i],false);
        this->solution.push_back(temp);
    }
    this->profitSummation = 0;
}

MMKPSolution::MMKPSolution(std::vector<int> numberOfItemsPerClass,
             int numberOfConstraints){
    for(std::size_t i=0;i<numberOfItemsPerClass.size();i++){
        std::vector<bool> temp(numberOfItemsPerClass[i],false);
        this->solution.push_back(temp);
    }
    this->profitSummation = 0;
    this->costsSummation.resize(numberOfConstraints,0);
}

//operator overloading
std::vector<bool>& MMKPSolution::operator[](int index){
    assert(index<this->solution.size());
    return this->solution.at(index);
}

bool MMKPSolution::operator==(MMKPSolution &other) const{
    for(int i=0;i<this->solution.size();i++){
        for(int j=0;j<this->solution[i].size();j++){
            if(other[i][j] != this->solution[i][j]){
                return false;
            }
        }
    }
    return true;
}

//accessors
int MMKPSolution::size(){
    return this->solution.size();
}

float MMKPSolution::getProfit(){
    return this->profitSummation;
}

std::vector<float> MMKPSolution::getCostsSummation(){
    return this->costsSummation;
}

float MMKPSolution::getCost(int index){
    return this->costsSummation.at(index);
}

//mutators
void MMKPSolution::setCosts(const std::vector<float> costsSummation){
    this->costsSummation = costsSummation;
}

void MMKPSolution::setCost(const std::size_t index, float costSummation){
    assert(index<this->costsSummation.size());
    this->costsSummation.at(index) = costSummation;
}

void MMKPSolution::setProfit(const float profitSummation){
    this->profitSummation = profitSummation;
}

//non-member functions
std::ostream& operator<<(std::ostream& os,MMKPSolution& obj){
    
    os<<"Profit: "<<std::fixed<<std::setprecision(2)<<obj.getProfit()<<std::endl;
    std::vector<float> temp(obj.getCostsSummation());
    os<<"Costs: ";
    for(std::vector<float>::iterator it=temp.begin();it<temp.end();it++){
        os<<std::fixed<<std::setprecision(2)<<*it<<" ";
    }
    os<<std::endl;
    
    //walk through and print each item in each class.
    for(std::vector<std::vector<bool> >::iterator it=obj.solution.begin();
        it<obj.solution.end();it++){
        for(std::vector<bool>::iterator jt=(*it).begin();jt<(*it).end();jt++){
            os<<*jt<<" ";
        }
        os<<std::endl;
    }
    return os;
}




