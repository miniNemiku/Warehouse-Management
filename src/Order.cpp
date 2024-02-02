#include "../include/Order.h"
#include <iostream>

using namespace std;

//======================================CONSTRUCTOR======================================
//changed
Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

//======================================METHODS-BASIC======================================

int Order::getId() const{return id;}
int Order::getCustomerId() const{return customerId;}
int Order::getCollectorId() const{return collectorId;}
int Order::getDriverId() const{return driverId;}
OrderStatus Order::getStatus() const{return status;}
int Order::getDistance() const {return distance;}
void Order::setStatus(OrderStatus status){this -> status = status;}
void Order::setCollectorId(int collectorId){this -> collectorId = collectorId;}
void Order::setDriverId(int driverId){this -> driverId = driverId;}
bool Order::getNeedTransfer(){return needTransfer;}
void Order::switchNeedTransfer(bool transferStatus){needTransfer = transferStatus;}

//======================================METHODS======================================

const string Order::volun_helper(int volun_status) const{
    if(volun_status == -1)
        return "No Volunteer";
    return to_string(volun_status);
}

const string Order::status_helper(OrderStatus status) const{
    if(status == OrderStatus::PENDING)
        return "PENDING";
    if(status == OrderStatus::COLLECTING)
        return "COLLECTING";
    if(status == OrderStatus::DELIVERING)
        return "DELIVERING";
    return "COMPLETED";
}

const string Order::toString() const{
    string output = "Order #" + to_string(id)  +"|| Customer #" + to_string(customerId) +
    "|| Distance:" + to_string(distance) + "|| CollectorId #" + volun_helper(collectorId) +
    "|| DriverId #" + volun_helper(driverId) + "|| STATUS: " + status_helper(status);

    return output;
}


