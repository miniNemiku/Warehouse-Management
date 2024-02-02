#include "../include/Customer.h"
#include <iostream>

//======================================CONSTRUCTOR======================================

Customer::Customer (int id, const string& name, int locationDistance, int maxOrders) 
: id(id), name(name),locationDistance(locationDistance), maxOrders(maxOrders),ordersId(){} 

//======================================METHODS-BASIC======================================

const string& Customer :: getName() const{return name;}
int Customer ::getId() const{return id;}
int Customer:: getCustomerDistance() const{return locationDistance;}
int Customer:: getMaxOrders() const{return maxOrders;}
int Customer:: getNumOrders() const{return ordersId.size();} 
bool Customer:: canMakeOrder() const{return getNumOrders() < getMaxOrders();}
const vector<int>& Customer:: getOrdersIds() const{return ordersId;}

//======================================METHODS======================================

int Customer::addOrder(int orderId){
    if(canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

//======================================INHERITANCE======================================
//======================================CONSTRUCTOR+CLONE================================

SoldierCustomer::SoldierCustomer(int id, const string& name, int locationDistance,int maxOrders) :
Customer(id,name,locationDistance,maxOrders){}

SoldierCustomer *SoldierCustomer::clone() const {return new SoldierCustomer(*this);}

//======================================CONSTRUCTOR+CLONE======================================

CivilianCustomer::CivilianCustomer(int id, const string& name, int locationDistance,int maxOrders) :
Customer(id,name,locationDistance,maxOrders){}

CivilianCustomer *CivilianCustomer::clone() const {return new CivilianCustomer(*this);}

