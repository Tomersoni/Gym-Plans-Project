//
// Created by shir on 25/11/2021.
//

#include "../include/Trainer.h"
//
// Created by shir on 23/11/2021.
//


#include <vector>
#include "../include/Customer.h"
#include "../include/Workout.h"
#include "../include/Trainer.h"

typedef std::pair<int, Workout> OrderPair;


Trainer::Trainer(int t_capacity):capacity(t_capacity)
{
    open=true;
    salary=0;
    currentCapacity=capacity;
}

void Trainer::copy(int &other_capacity,int &other_currentCapacity, int &other_salary, bool &other_open,
                   std::vector<Customer*> &other_customersList,std::vector<OrderPair> &other_orderList)
{
    capacity=other_capacity;
    currentCapacity=other_currentCapacity;
    salary=other_salary;
    open=other_open;
    for(int i=0; i<other_customersList.size(); i++)
    {
        std::string type= other_customersList[i]->toString();
        if(type=="swt")
        {
            SweatyCustomer* c1= new SweatyCustomer(other_customersList[i]->getName(),other_customersList[i]->getId());
            customersList.push_back(c1);
        }
        if(type=="chp")
        {
            CheapCustomer* c1 = new CheapCustomer(other_customersList[i]->getName(),other_customersList[i]->getId());
            customersList.push_back(c1);
        }
        if(type=="mcl")
        {
            HeavyMuscleCustomer* c1= new HeavyMuscleCustomer(other_customersList[i]->getName(),other_customersList[i]->getId());
            customersList.push_back(c1);
        }
        if(type=="fbd")
        {
            FullBodyCustomer* c1= new FullBodyCustomer(other_customersList[i]->getName(),other_customersList[i]->getId());
            customersList.push_back(c1);
        }

    }

    for(int i=0; i<other_orderList.size();i++)
    {
        Workout w1(other_orderList[i].second.getId(),other_orderList[i].second.getName(),
                   other_orderList[i].second.getPrice(),other_orderList[i].second.getType());
        OrderPair p1(other_orderList[i].first,w1);
        orderList.push_back(p1);
    }

}

Trainer::Trainer (Trainer &other_Trainer)
{
    copy(other_Trainer.capacity, other_Trainer.currentCapacity,other_Trainer.salary,
         other_Trainer.open,other_Trainer.customersList,other_Trainer.orderList);
}

Trainer& Trainer::operator=(Trainer &other_Trainer)
{
    if(this!=&other_Trainer)
    {
        clear();
        copy(other_Trainer.capacity, other_Trainer.currentCapacity,other_Trainer.salary,
             other_Trainer.open,other_Trainer.customersList,other_Trainer.orderList);
    }

    return *this;
}

void Trainer:: clear()
{
    capacity=0;
    open= false;
    salary=0;
    for(int i=0; i<customersList.size();i++)
    {
        delete customersList[i];
        customersList[i]=nullptr;
    }
    customersList.clear();
    orderList.clear();
}

Trainer:: ~Trainer()
{
    clear();
}

Trainer::Trainer (Trainer &&other_Trainer) :
        capacity(other_Trainer.capacity),open(other_Trainer.open),salary(other_Trainer.salary)//we hope shir knows
{
    other_Trainer.capacity=0;
    other_Trainer.open= false;
    other_Trainer.salary=0;
    customersList= std::move(other_Trainer.customersList);
    orderList=std::move(other_Trainer.orderList);
}

Trainer& Trainer::operator=(Trainer &&other_Trainer)
{
    if(this!=&other_Trainer)
    {
        clear();
        capacity=other_Trainer.capacity;
        open=other_Trainer.open;
        salary=other_Trainer.salary;
        other_Trainer.capacity=0;
        other_Trainer.open= false;
        other_Trainer.salary=0;
        customersList= std::move(other_Trainer.customersList);
        orderList=std::move(other_Trainer.orderList);
    }

    return *this;
}
int Trainer::getcurrCapacity() const
{
    return currentCapacity;
}
int Trainer::getCapacity() const
{
    return capacity;
}

void Trainer::addCustomer(Customer* customer)
{
    customersList.push_back(customer);
    currentCapacity--;
}

void Trainer::removeCustomer(int id)
{
    std::vector<Customer*> newlist;
    for(int i =0; i<customersList.size(); i++)
    {
        if(customersList[i]->getId()!=id)
            newlist.push_back(customersList[i]);

    }
    customersList=std::move(newlist);

    std::vector<OrderPair> new2;
    for(int i=0; i<orderList.size(); i++)
    {
        if(orderList[i].first!=id)
        {
            Workout w1(orderList[i].second.getId(),orderList[i].second.getName(),
                       orderList[i].second.getPrice(),orderList[i].second.getType());
            OrderPair p1(orderList[i].first,w1);
            new2.push_back(p1);
        }
        if(orderList[i].first==id){
            salary=salary-orderList[i].second.getPrice();
        }


    }
    orderList=std::move(new2);

    currentCapacity++;
}

Customer* Trainer::getCustomer(int id)
{
    for(int i =0; i<customersList.size(); i++)
    {
        if(customersList[i]->getId()==id)
            return customersList[i];
    }

    return nullptr;
}

std::vector<Customer*>& Trainer::getCustomers()
{
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders()
{
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
    for(int i =0; i<workout_ids.size(); i++)
    {
        Workout w1=searchWorkout(workout_ids[i],workout_options);
        OrderPair toInsert = std::make_pair(customer_id,w1);

        bool contain=false;
        for(int i=0;i<orderList.size();i++){
            if((toInsert.first==orderList[i].first )& toInsert.second.isEquals(orderList[i].second)  ){
                contain=true;
            }
        }
        if(!contain){
            orderList.push_back(toInsert);
            salary=salary+w1.getPrice();
        }


    }
}

Workout Trainer::searchWorkout (int id, const std::vector<Workout>& workout_options)
{
    bool found=false;
    int i=0;
    for(; i<workout_options.size() && !found; i++)
    {
        if(id==workout_options[i].getId())
            found=true;
    }
    return workout_options[i-1];
}

void Trainer::openTrainer()
{
    open=false;
}
void Trainer::closeTrainer()
{
    for(int i=0;i<customersList.size();i++) {
        if (customersList[i] != nullptr) {
            delete customersList[i];
            customersList[i]=nullptr;
        }
    }
    customersList.clear();
    orderList.clear();
    open=true;
    currentCapacity=capacity;
}

int Trainer::getSalary() const
{
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

