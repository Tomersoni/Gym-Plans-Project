//
// Created by shir on 25/11/2021.
//

#include "../include/Studio.h"
//
// Created by shir on 23/11/2021.
//


#include <vector>
#include <string>
#include <cstring>
#include "../include/Workout.h"
#include "../include/Studio.h"
#include "../include/Trainer.h"
#include "../include/Action.h"
#include <fstream>
#include <iostream>



Studio::Studio()
{

}

Studio::Studio(const std::string &configFilePath)
{
    std::fstream myfile;
    myfile.open(configFilePath,std::ios::in);
    if(myfile.is_open()){
        std::string line;
        int i=0;
        while(std::getline(myfile,line)){
            if(i==4)
            {
                for(int j=0;j<line.size();j=j+2)
                {
                    int a = (int)(line[j])-48;
                    trainers.push_back(new Trainer(a));
                }
            }

            if(i>=7 & line!="")
            {
                std::string s1="";
                std::string s2="";
                std::string s3="";
                int k=0;
                while(line[k]!=',')
                {
                    s1=s1+line[k];
                    k++;
                }
                k=k+2;

                while(line[k]!=',')
                {
                    s2=s2+line[k];
                    k++;
                }
                k=k+2;

                while(k<line.size())
                {
                    s3 = s3 + line[k];
                    k++;
                }

                WorkoutType type= checkType(s2);
                Workout w(i-7,s1, std::stoi(s3), type);
                workout_options.push_back(w);
            }

            i++;
        }
    }
}


void Studio::copy (bool other_open, std::vector<Trainer*> other_trainers, std::vector<Workout>
other_workout_options, std::vector<BaseAction*> other_actionsLog)
{
    open=other_open;
    for(int i=0; i<other_trainers.size(); i++)
    {
        Trainer* t1 = new Trainer(*other_trainers[i]);
        trainers.push_back(t1);
    }

    for(int i=0;i<other_workout_options.size();i++){
        Workout w(other_workout_options[i].getId(),other_workout_options[i].getName(),
                  other_workout_options[i].getPrice(),other_workout_options[i].getType());
        workout_options.push_back(w);
    }

    for(int i=0; i<other_actionsLog.size(); i++)
    {
        actionsLog.push_back(other_actionsLog[i]->clone());
    }
}

void Studio::start()
{
    open=true;
    std::cout<<"Studio is now open!"<<std::endl;
    std::string s1;
    getline(std::cin, s1);

    while(s1!="closeall")
    {
        std::string firstWord = s1.substr(0,s1.find(" "));
        if(firstWord=="open")
        {
            s1=s1.substr(5,s1.size());
            int i=s1.find(" ");
            std::string sid=s1.substr(0,s1.find(" "));
            int id=std::stoi(sid);
            s1=s1.substr(i+1,s1.size());

            std::vector<Customer*> customersList;
            while(s1.size()>3)
            {
                if(s1.find(" ")!=-1){
                    int i=s1.find(" ");
                    std::string customer = s1.substr(0, i);
                    s1=s1.substr(i+1,s1.size());
                    std::string name = customer.substr(0,customer.find(","));
                    customer= customer.substr(customer.find(",")+1, customer.size());
                    std::string type = customer;

                    if(type=="swt")
                    {

                        customersList.push_back(new SweatyCustomer (name,customerId));
                    }

                    else if(type=="fbd")
                    {

                        customersList.push_back(new FullBodyCustomer(name,customerId));
                    }
                    else if(type=="chp")
                    {
                        customersList.push_back(new CheapCustomer(name,customerId));
                    }
                    else if(type=="mcl")
                    {
                        customersList.push_back(new HeavyMuscleCustomer (name,customerId));
                    }
                }
                else{
                    std::string name = s1.substr(0,s1.find(","));
                    s1= s1.substr(s1.find(",")+1, s1.size());
                    std::string type = s1;

                    if(type=="swt")
                    {

                        customersList.push_back(new SweatyCustomer (name,customerId));
                    }

                    else if(type=="fbd")
                    {

                        customersList.push_back(new FullBodyCustomer(name,customerId));
                    }
                    else if(type=="chp")
                    {
                        customersList.push_back(new CheapCustomer(name,customerId));
                    }
                    else if(type=="mcl")
                    {
                        customersList.push_back(new HeavyMuscleCustomer (name,customerId));
                    }
                }


                customerId++;

            }
            OpenTrainer *x=new OpenTrainer(id,customersList);
            x->act(*this);
            actionsLog.push_back(x);
            //delete x;
        }

        else if(firstWord=="order")
        {
            s1=s1.substr(s1.find(" ")+1, s1.size());
            int id= std::stoi(s1);
            Order* order= new Order(id);
            order->act(*this);
            actionsLog.push_back(order);
        }

        else if(firstWord=="move")
        {
            s1 = s1.substr(s1.find(" ")+1, s1.size());
            int tid = std::stoi(s1.substr(0,s1.find(" ")));
            s1 = s1.substr(s1.find(" ")+1, s1.size());
            int dst = std::stoi(s1.substr(0,s1.find(" ")));
            s1 = s1.substr(s1.find(" ")+1, s1.size());
            int cid = std::stoi(s1.substr(0,s1.find(" ")));
            MoveCustomer* move= new MoveCustomer(tid,dst,cid);
            move->act(*this);
            actionsLog.push_back(move);
        }
        else if(firstWord=="close")
        {
            s1=s1.substr(s1.find(" ")+1, s1.size());
            int id= std::stoi(s1);
            Close *close= new Close(id);
            close->act(*this);
            actionsLog.push_back(close);
        }

        else if(firstWord=="workout_options")
        {
            PrintWorkoutOptions* p1= new PrintWorkoutOptions;
            p1->act(*this);
            actionsLog.push_back(p1);
        }

        else if(firstWord=="status")
        {
            s1=s1.substr(s1.find(" ")+1, s1.size());
            int id= std::stoi(s1);
            PrintTrainerStatus* status= new PrintTrainerStatus(id);
            status->act(*this);
            actionsLog.push_back(status);
        }

        else if(firstWord=="log")
        {
            PrintActionsLog* p1= new PrintActionsLog;
            p1->act(*this);
            actionsLog.push_back(p1);

        }

        else if(firstWord=="backup")
        {
            BackupStudio* p1= new BackupStudio;
            actionsLog.push_back(p1);
            p1->act(*this);
        }

        else if(firstWord=="restore")
        {
            RestoreStudio* p1= new RestoreStudio;
            p1->act(*this);
            actionsLog.push_back(p1);
        }


        getline(std::cin, s1);
    }

    if(s1=="closeall")
    {
        CloseAll* close= new CloseAll;
        close->act(*this);
        delete close;
    }

}

int Studio::getNumOfTrainers() const
{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid) {
    if(tid<trainers.size())
        return trainers[tid];
    else
        return nullptr;
}

const std::vector<BaseAction*>& Studio::getActionsLog() const {
    return actionsLog;
}

WorkoutType Studio::checkType(std::string s) {
    if(s=="Anaerobic")
        return ANAEROBIC;
    else if(s=="Cardio")
        return CARDIO;
    else
        return MIXED;

}

//Rule Of Five:
Studio::Studio(const Studio &other) {
    copy(other.open,other.trainers,
         other.workout_options,other.actionsLog);

}

Studio& Studio::operator=(const Studio &other){
    if(this!=&other){
        clear();
        copy(other.open,other.trainers,
             other.workout_options,other.actionsLog);

    }
    return *this;
}

Studio::Studio(Studio &&other): open(other.open){
    trainers=std::move(other.trainers);
    workout_options=std::move(other.workout_options);
    actionsLog=std::move(other.actionsLog);
    other.open=false;

}
Studio& Studio::operator=(Studio &&other){
    if(this!=&other){
        clear();
        open=other.open;
        trainers=std::move(other.trainers);
        workout_options=std::move(other.workout_options);
        actionsLog=std::move(other.actionsLog);
        other.open=false;
    }
    return *this;
}
Studio::~Studio(){

    clear();
}

void Studio::clear(){
    for(int i=0; i<trainers.size(); i++)
    {
        if(trainers[i]!= nullptr) {
            delete trainers[i];
            trainers[i]=nullptr;
        }
    }

    for(int i=0; i<actionsLog.size(); i++)
    {
        if(actionsLog[i]!= nullptr) {
            delete actionsLog[i];
            actionsLog[i]=nullptr;
        }
    }
    open=false;
    trainers.clear();
    workout_options.clear();
    actionsLog.clear();
}

std::vector<Workout>& Studio::getWorkoutOptions()
{
    return workout_options;
}

std::vector<Trainer*> Studio::getTrainers() {
    return trainers;
}


