//
// Created by shir on 25/11/2021.
//



#ifndef FINALWORK1_TRAINER_H
#define FINALWORK1_TRAINER_H
#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary() const;
    bool isOpen();
    void copy(int &other_capacity,int &other_currentCapacity, int &other_salary, bool &other_open,
              std::vector<Customer*> &other_customersList,std::vector<OrderPair> &other_orderList);
    Trainer (Trainer &other_Trainer); //Copy Constructor
    Trainer& operator=(Trainer &other_Trainer);
    void clear();
    ~Trainer();
    Trainer (Trainer &&other_Trainer);
    Trainer& operator=(Trainer &&other_Trainer);
    int getcurrCapacity() const;
private:
    int capacity;
    int currentCapacity;
    int salary;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    Workout searchWorkout (int id, const std::vector<Workout>& workout_options);
};

#endif //FINALWORK1_TRAINER_H