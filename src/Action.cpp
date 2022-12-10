
//
// Created by shir on 23/11/2021.
//
using namespace std;

#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Studio.h"
#include "../include/Trainer.h"



BaseAction::BaseAction()
{

}

ActionStatus BaseAction::getStatus() const
{
    return status;
}


void BaseAction::complete()
{
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg)
{
    status=ERROR;
    this->errorMsg=errorMsg;
    std::cout << "Error: " ;
    std::cout << errorMsg <<endl;

}
std::string BaseAction::getErrorMsg() const
{
    return errorMsg;
}



BaseAction::~BaseAction() {

}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) :
        trainerId(id) , customers(customersList)
{


}

void OpenTrainer::act(Studio &studio)
{
    Trainer* t= studio.getTrainer(trainerId);
    if(t==nullptr || !t->isOpen())
        error("Workout session does not exist or is already open");

    if(t->getCapacity()<customers.size())
        error("Number of customers exceeds trainer's capacity");

    else{
        for(int i=0; i<t->getCapacity() & i<customers.size(); i++)
        {
            t->addCustomer(customers[i]->clone());
        }

        t->openTrainer();
        complete();

    }

}

std::string OpenTrainer::toString() const
{
    std::string s1="open "+ std::to_string(trainerId) + " ";
    for(int i=0;i<customers.size();i++){
        s1=s1+customers[i]->getName()+","+customers[i]->toString()+ " ";
    }
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

OpenTrainer* OpenTrainer::clone()
{
    std::vector<Customer*> c;
    for(int i=0;i<customers.size();i++){
        c.push_back(customers[i]->clone());
    }
    OpenTrainer* o=new OpenTrainer(trainerId,c);
    return o;
}

OpenTrainer::~OpenTrainer(){
    for(int i=0;i<customers.size();i++){
        if (customers[i] != nullptr) {
            delete customers[i];
            customers[i]=nullptr;
        }
    }
}



Order::Order(int id) : trainerId(id)
{

}

void Order::act(Studio &studio)
{
    Trainer* t= studio.getTrainer(trainerId);
    if(t==nullptr || t->isOpen())
        error("Workout session does not exist or is'nt open");
    else{
        std::vector<Customer*> customers= t->getCustomers();

        for(int i=0; i<customers.size(); i++)
        {
            std::vector<int> workoutIds = customers[i]->order(studio.getWorkoutOptions());
            t->order(customers[i]->getId(),workoutIds,studio.getWorkoutOptions());
        }

        std::vector<OrderPair> orders= t->getOrders();
        for(int i =0; i<orders.size(); i++)
        {
            Customer* c = t->getCustomer(orders[i].first);
            std::cout<<c->getName()+" Is Doing "+ orders[i].second.getName()<<std::endl;
        }
        complete();
    }
}

std::string Order::toString() const
{

    std::string s1="order " + std::to_string(trainerId)+" ";
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

Order* Order::clone(){
    Order* o=new Order(trainerId);
    return o;
}


MoveCustomer::MoveCustomer(int src, int dst, int customerId) :
        srcTrainer(src), dstTrainer(dst), id(customerId)
{
}

void MoveCustomer::act(Studio &studio)
{
    Trainer* t1= studio.getTrainer(srcTrainer);
    Trainer* t2= studio.getTrainer(dstTrainer);
    if(t1==nullptr || t2==nullptr || t1->isOpen() || t2->isOpen() ||
       t1->getCustomer(id)==nullptr || t2->getcurrCapacity()==0)
    {
        error("Cannot move customer");
    }
    else{
        Customer* c1 = t1->getCustomer(id);

        t1->removeCustomer(id);

        if(t1->getCustomers().size()==0)
        {
            Close close = Close(srcTrainer);
            close.act(studio);
        }

        t2->addCustomer(c1);

        std::vector<int> workoutIds = c1->order(studio.getWorkoutOptions());
        t2->order(c1->getId(),workoutIds,studio.getWorkoutOptions());
        complete();
    }
}

std::string MoveCustomer::toString() const
{
    std::string s1="move "+std::to_string(srcTrainer)+" "+std::to_string(dstTrainer)+" "+std::to_string(id);
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

MoveCustomer* MoveCustomer::clone(){
    MoveCustomer* m=new MoveCustomer(srcTrainer,dstTrainer,id);
    return m;
}


Close::Close(int id) : trainerId(id)
{

}


void Close::act(Studio &studio)
{
    Trainer* t1 = studio.getTrainer(trainerId);
    if(t1==nullptr || t1->isOpen())
        error("Trainer does not exist or is not open");
    else{
        std::cout<<"Trainer ";
        std::cout<< trainerId;
        std::cout<<" Closed. Salary ";
        std::cout<< t1->getSalary();
        std::cout<< "NIS" <<endl;
        t1->closeTrainer();
        complete();
    }
}

std::string Close::toString() const
{
    std::string s1="close "+std::to_string(trainerId);
    if(getStatus()==COMPLETED)
        s1=s1+" Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}


Close* Close::clone(){
    Close* c=new Close(trainerId);
    return c;
}


CloseAll::CloseAll(){};
void CloseAll::act(Studio &studio)
{
    std::vector<Trainer*> trainers = studio.getTrainers();
    for(int i=0; i<trainers.size(); i++)
    {
        if(!trainers[i]->isOpen())
        {
            Close close(i);
            close.act(studio);
        }

    }

    std::cout<<"Studio is now close!";
}

std::string CloseAll::toString() const
{
    return "closeall";
}

CloseAll* CloseAll::clone(){
    CloseAll* c=new CloseAll;
    return c;
}


PrintWorkoutOptions::PrintWorkoutOptions()
{

}

void PrintWorkoutOptions::act(Studio &studio)
{
    std::vector<Workout> options= studio.getWorkoutOptions();
    for(int i=0; i<options.size(); i++)
    {
        std::cout<<options[i].getName();
        std::cout << ", ";
        WorkoutType t = options[i].getType();
        if(t==MIXED){
            std::cout<<"Mixed";
        }
        else if(t==ANAEROBIC){
            std::cout<<"Anaerobic";
        }
        else{
            std::cout<<"Cardio";
        }
        std::cout << ", ";
        std::cout <<options[i].getPrice()<<endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const
{
    std::string s1= "workout_options " ;
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

PrintWorkoutOptions* PrintWorkoutOptions::clone(){
    PrintWorkoutOptions* p=new PrintWorkoutOptions;
    return p;
}



PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id)
{

}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *t1 = studio.getTrainer(trainerId);
    bool open = !t1->isOpen();
    std::string isOpen = "closed";
    if (open)
    {
        std::cout << "Trainer " << trainerId << " Status: " << "open" << endl;
        std::cout << "Customers:" << endl;
        std::vector<Customer *> customers = t1->getCustomers();
        for (int i = 0; i < customers.size(); i++) {
            std::cout << customers[i]->getId() ;
            std::cout << " ";
            std::cout << customers[i]->getName() << endl;
        }
        std::cout << "Orders:" << endl;
        std::vector<OrderPair> orders = t1->getOrders();

        for (int i = 0; i < orders.size(); i++)
        {
            std::cout << orders[i].second.getName() ;
            std::cout << " ";
            std::cout << orders[i].second.getPrice() ;
            std::cout << "NIS";
            std::cout << " ";
            std::cout <<orders[i].first<< endl;
        }
        std::cout<<"Current Trainer's Salary: "<< t1->getSalary();
        std::cout << "NIS"<<endl;
    }
    else
        std::cout << "Trainer " << trainerId << " Status: " << "closed" << endl;

    complete();
}

std::string PrintTrainerStatus::toString() const
{
    std::string s1="status "+std::to_string(trainerId);
    if(getStatus()==COMPLETED)
        s1=s1+" Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

PrintTrainerStatus* PrintTrainerStatus::clone(){
    PrintTrainerStatus* p=new PrintTrainerStatus(trainerId);
    return p;
}





PrintActionsLog::PrintActionsLog()
{

}

void PrintActionsLog::act(Studio &studio)
{
    std::vector<BaseAction*> actions= studio.getActionsLog();
    for(int i=0; i<actions.size(); i++)
    {
        std::cout<< actions[i]->toString()<<endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const
{
    std::string s1="log ";
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}
PrintActionsLog* PrintActionsLog::clone(){
    PrintActionsLog* p=new PrintActionsLog;
    return p;
}

BackupStudio::BackupStudio()
{

}

void BackupStudio::act(Studio &studio)
{
    if(backup!= nullptr){
        delete backup;
    }

    backup=new Studio(studio);
    complete();
}
std::string BackupStudio::toString() const
{
    std::string s1= "backup ";
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}
BackupStudio* BackupStudio::clone(){
    BackupStudio* p=new BackupStudio;
    return p;
}





RestoreStudio::RestoreStudio()
{

}

void RestoreStudio::act(Studio &studio)
{
    if(backup==nullptr){
        error("No backup available");

    }
    else {
        studio = *backup;
        complete();
    }
}

std::string RestoreStudio::toString() const
{
    std::string s1= "restore ";
    if(getStatus()==COMPLETED)
        s1=s1+"Completed";
    else
        s1=s1+" Error: "+getErrorMsg();
    return s1;
}

RestoreStudio* RestoreStudio::clone(){
    RestoreStudio* p=new RestoreStudio;
    return p;
}

