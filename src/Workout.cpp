//
// Created by shir on 25/11/2021.
//

#include "../include/Workout.h"
#include <string>


Workout::Workout (int w_id, std::string w_name, int w_price, WorkoutType w_type)
        :id(w_id), name(w_name), price(w_price), type(w_type)
{

}

int Workout::getId() const
{
    return id;
}

std::string Workout::getName() const
{
    return name;
}

int Workout::getPrice() const
{
    return price;
}

WorkoutType Workout::getType() const
{
    return type;
}

bool Workout::isEquals(Workout other){
    bool typecheck=false;
    if((type==ANAEROBIC & other.type==ANAEROBIC)|(type==MIXED & other.type==MIXED)|(type==CARDIO & other.type==CARDIO)){
        typecheck= true;
    }
    return (id==other.id)&(price==other.price)&(name.compare(other.name)==0)&(typecheck);
}