//
// Created by shir on 25/11/2021.
//


#ifndef FINALWORK1_STUDIO_H
#define FINALWORK1_STUDIO_H



#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

extern Studio* backup;

class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainers ();
    void copy (bool other_open, std::vector<Trainer*> other_trainers, std::vector<Workout>
    other_workout_options, std::vector<BaseAction*> other_actionsLog); //Copy resources
    Studio(const Studio &other);
    Studio& operator=(const Studio &other);
    Studio(Studio &&other);
    Studio& operator=(Studio &&other);
    virtual ~Studio();
    void clear();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    WorkoutType checkType(std::string s);
    int customerId=0;




};



#endif //FINALWORK1_STUDIO_H