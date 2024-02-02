#pragma once
#ifndef WAREHOUSE_H
#define WAREHOUSE_H
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"



class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

class WareHouse
{

public:
    WareHouse(const string &configFilePath);
    void start();
    void addOrder(Order *order);
    void addAction(BaseAction *action);
    Customer &getCustomer(int customerId) const;
    Volunteer &getVolunteer(int volunteerId) const;
    Order &getOrder(int orderId) const;
    const vector<Order *> &getPendingOrders() const;   
    const vector<Volunteer *> &getVolunteers() const;  
    const vector<Order *> &getInProcessOrders() const; 
    const vector<Order *> &getCompletedOrders() const;
    const vector<BaseAction *> &getActions() const;
    void addOrderToInProcessOrders(Order *order);
    void close();
    void open();
    void removeOrderFromPending(int orderId);
    void removeOrderFromProcessing(int orderId);
    void addOrderToCompletedOrders(Order *order);   
    void setOrderCollector(Order *order,int collectorId);
    void setOrderDriver(Order *order,int driverId);
    void setCollectorOrder(Order *order,CollectorVolunteer* vol);
    void setLimitedCollectorOrder(Order *order, LimitedCollectorVolunteer* vol);
    void setDriverOrder(Order *order,DriverVolunteer* vol);
    void setLimitedDriverOrder(Order *order, LimitedDriverVolunteer* vol);
    void removeLimitedCollectorVolunteer(LimitedCollectorVolunteer* vol);
    void removeLimitedDriverVolunteer(LimitedDriverVolunteer* vol);
    void addBackPending();
    void setOrderCompleted(Order *order);
    void addCutomer(Customer *customer);
    void RemoveVolunteerById(int volunteer);
    void deleteBackup(WareHouse*& backup);
    int getCustomerCounter();
    int getVolunteerCounter();
    ~WareHouse();//Destructor
    WareHouse(const WareHouse &other);//Copy constructor
    WareHouse& operator=(const WareHouse &other);//copy assignment operator
    WareHouse(WareHouse &&other) noexcept;//Move Constructor
    WareHouse& operator=(WareHouse &&other) noexcept;// Move Assignment
    


private:
    bool isOpen;
    vector<BaseAction *> actionsLog;
    vector<Volunteer *> volunteers;
    vector<Order *> pendingOrders;
    vector<Order *> inProcessOrders;
    vector<Order *> completedOrders;
    vector<Customer *> customers;
    int customerCounter;  // For assigning unique customer IDs
    int volunteerCounter; // For assigning unique volunteer IDs
};


#endif