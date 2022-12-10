//
// Created by shir on 25/11/2021.
//

#include "../include/Customer.h"
//
// Created by shir on 23/11/2021.
//s


#include "../include/Customer.h"
#include "../include/Workout.h"
#include <vector>
#include <string>
#include <algorithm>



Customer::Customer(std::string c_name, int c_id):
        name(c_name),id(c_id)
{

}

std::string Customer::getName() const
{
    return name;
}

int Customer::getId() const
{
    return id;
}


SweatyCustomer::SweatyCustomer(std::string name, int id):
        Customer(name,id)
{

}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> workouts;
    for(int i=0; i<workout_options.size(); i++)
    {
        if(workout_options[i].getType()==CARDIO)
            workouts.push_back(workout_options[i].getId());
    }
    return workouts;
}


std::string SweatyCustomer::toString() const
{
    std::string s1 = "swt";
    return s1;
}

SweatyCustomer* SweatyCustomer::clone(){
    SweatyCustomer* c=new SweatyCustomer(getName(),getId());
    return c;
}


CheapCustomer::CheapCustomer(std::string name, int id):
        Customer(name,id)
{

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> workouts;
    int min=workout_options[0].getPrice();
    int minLocation=0;
    for(int i=1; i<workout_options.size(); i++)
    {
        if(workout_options[i].getPrice()<min
           ||
           (workout_options[i].getPrice()==min && workout_options[i].getId()<workout_options[minLocation].getId()))
        {
            min=workout_options[i].getPrice();
            minLocation=i;
        }

    }
    workouts.push_back(workout_options[minLocation].getId());
    return workouts;
}
std::string CheapCustomer::toString() const
{
    std::string s1 = "chp";
    return s1;
}

CheapCustomer* CheapCustomer::clone(){
    CheapCustomer* c=new CheapCustomer(getName(),getId());
    return c;
}


HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):
        Customer(name,id)
{

}

typedef std::pair<int, int> Pair;

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<Pair> workouts;

    for(int i =0; i<workout_options.size(); i++)
    {
        if(workout_options[i].getType()==ANAEROBIC){
            Pair p=std::make_pair(workout_options[i].getId(),workout_options[i].getPrice());
            workouts.push_back(p);
        }
    }
    std::vector<int> output;
    std::sort(workouts.begin(),workouts.end(),[](std::pair<int,int> p1,std::pair<int,int> p2){
        return p1.second>p2.second;
    });
    for(int i=0;i<workouts.size();i++){
        output.push_back(workouts[i].first);
    }

    return output;

}

std::string HeavyMuscleCustomer::toString() const
{
    std::string s1 = "mcl";
    return s1;
}

HeavyMuscleCustomer* HeavyMuscleCustomer::clone(){
    HeavyMuscleCustomer* c=new HeavyMuscleCustomer(getName(),getId());
    return c;
}


FullBodyCustomer::FullBodyCustomer(std::string name, int id):
        Customer(name,id)
{

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options)
{
    std::vector<int> output;
    std::vector<Workout> cardios;
    std::vector<Workout> mixed;
    std::vector<Workout> anerobics;
    for(int i=0;i<workout_options.size();i++){
        if(workout_options[i].getType()==CARDIO){
            cardios.push_back(workout_options[i]);
        }
        if(workout_options[i].getType()==MIXED){
            mixed.push_back(workout_options[i]);
        }
        if(workout_options[i].getType()==ANAEROBIC){
            anerobics.push_back(workout_options[i]);
        }
    }
    if(cardios.size()!=0) {
        int minCardioPrice = cardios[0].getPrice();
        int minCardioId = 0;
        for (int i = 1; i < cardios.size(); i++) {
            if (minCardioPrice > cardios[i].getPrice()) {
                minCardioPrice = cardios[i].getPrice();
                minCardioId = i;
            }
        }

        output.push_back(cardios[minCardioId].getId());
    }
    if(mixed.size()!=0) {
        int maxMixedPrice = mixed[0].getPrice();
        int maxMixedId = 0;
        for (int i = 1; i < mixed.size(); i++) {
            if (maxMixedPrice < mixed[i].getPrice()) {
                maxMixedPrice = mixed[i].getPrice();
                maxMixedId = i;
            }
        }
        output.push_back(mixed[maxMixedId].getId());
    }
    if(anerobics.size()!=0) {
        int minAnerobicPrice = anerobics[0].getPrice();
        int minAnerobicId = 0;
        for (int i = 1; i < anerobics.size(); i++) {
            if (minAnerobicPrice > anerobics[i].getPrice()) {
                minAnerobicPrice = anerobics[i].getPrice();
                minAnerobicId = i;
            }
        }

        output.push_back(anerobics[minAnerobicId].getId());
    }
    return output;
}

std::string FullBodyCustomer::toString() const
{
    std::string s1 ="fbd";
    return s1;
}

FullBodyCustomer* FullBodyCustomer::clone(){
    FullBodyCustomer* c=new FullBodyCustomer(getName(),getId());
    return c;
}

Customer:: ~Customer() {

};