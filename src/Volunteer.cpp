#include "../include/Volunteer.h"
#include <iostream>

using namespace std;

//======================================CONSTRUCTOR======================================

Volunteer::Volunteer(int id, const string &name):completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {};

//======================================METHODS-BASIC======================================

int Volunteer::getId() const {return id;}
const string& Volunteer::getName() const {return name;}
int Volunteer::getActiveOrderId() const {return activeOrderId;}
int Volunteer::getCompletedOrderId() const {return completedOrderId;}
bool Volunteer::isBusy() const {return activeOrderId != NO_ORDER;}

//======================================INHERITANCE======================================
//======================================CONSTRUCTOR+CLONE================================

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id,name),coolDown(coolDown),timeLeft(0){}
CollectorVolunteer *CollectorVolunteer::clone() const {return new CollectorVolunteer(*this);}

//======================================METHODS-BASIC======================================

bool CollectorVolunteer::hasOrdersLeft() const{return true;}//no limited orders number
bool CollectorVolunteer::canTakeOrder(const Order &order) const{return !isBusy();}
int CollectorVolunteer::getCoolDown() const{return coolDown;}
int CollectorVolunteer::getTimeLeft() const {return timeLeft;}

//======================================METHODS======================================


void CollectorVolunteer::step(){
    if(isBusy() && decreaseCoolDown()){
        completedOrderId = activeOrderId; //finished the order.
        activeOrderId = NO_ORDER;
    }
}

bool CollectorVolunteer::decreaseCoolDown() {
    if(timeLeft > 0)
        timeLeft--;
    if(timeLeft == 0)
        return true;
        
    return false;
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){
        activeOrderId = order.getId();
        timeLeft = coolDown;
    }
}


string CollectorVolunteer::toString() const{return "CollectorVolunteer #" + to_string(getId()) + " (" + getName() +")";}
string CollectorVolunteer::getType() const{return "CollectorVolunteer";}

//======================================CONSTRUCTOR+CLONE================================

LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders):CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {return new LimitedCollectorVolunteer(*this);}

//======================================METHODS-BASIC======================================

bool LimitedCollectorVolunteer::hasOrdersLeft() const {return ordersLeft > 0;}
bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const{return (hasOrdersLeft() && !isBusy());}
int LimitedCollectorVolunteer:: getMaxOrders() const{return maxOrders;}
int LimitedCollectorVolunteer:: getNumOrdersLeft() const{return ordersLeft;}

//======================================METHODS======================================

void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
    if(canTakeOrder(order)){
        CollectorVolunteer::acceptOrder(order); 
        ordersLeft--;
    }
}

string LimitedCollectorVolunteer::toString() const{return "Limited_CollectorVolunteer #" + 
to_string(getId()) + " (" + getName() +")" + " Orders left until retirement:" + to_string(getNumOrdersLeft());}

string LimitedCollectorVolunteer::getType() const{return "LimitedCollectorVolunteer";}

//======================================CONSTRUCTOR+CLONE================================

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
: Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}
DriverVolunteer *DriverVolunteer::clone() const {return new DriverVolunteer(*this);}

//======================================METHODS-BASIC======================================

int DriverVolunteer::getDistanceLeft() const {return distanceLeft;}
int DriverVolunteer::getMaxDistance() const {return maxDistance;}
int DriverVolunteer::getDistancePerStep() const {return distancePerStep;}
bool DriverVolunteer::hasOrdersLeft() const {return true; }
bool DriverVolunteer::canTakeOrder(const Order &order) const {return (!isBusy() && order.getDistance()<=maxDistance);}

//======================================METHODS======================================

bool DriverVolunteer::decreaseDistanceLeft() {
    if(distanceLeft > 0){
        distanceLeft -= distancePerStep;
        if(distanceLeft < 0)
            distanceLeft = 0;
        return distanceLeft==0;
    }
    return false;
}

void DriverVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)){
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    }
}
void DriverVolunteer::step() {
   //If I'm not busy - no order to progress with.
        if(isBusy() && decreaseDistanceLeft()){ //Finished the delivery if true. anyway decreasing the distance.
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }


string DriverVolunteer::toString() const {
    return "DriverVolunteer #" + to_string(getId()) +
     " (" + getName() +")  || Distance Left:" + to_string(getDistanceLeft()) + "|| MaxDistance: " + to_string(getMaxDistance()) 
     +"|| Distance per step:"+ to_string(getDistancePerStep());
}

string DriverVolunteer::getType() const{return "DriverVolunteer";}

//======================================CONSTRUCTOR+CLONE================================

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
: DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {return new LimitedDriverVolunteer(*this);}

//======================================METHODS-BASIC======================================

int LimitedDriverVolunteer::getMaxOrders() const {return maxOrders;}
int LimitedDriverVolunteer::getNumOrdersLeft() const {return ordersLeft;}
bool LimitedDriverVolunteer::hasOrdersLeft() const {return ordersLeft > 0;}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();}

//======================================METHODS======================================

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
    return "Limited"+DriverVolunteer::toString()+ 
    "|| max Orders: " +  to_string(maxOrders)+ "|| Orders Left: "+ to_string(ordersLeft) + "\n";
}

string LimitedDriverVolunteer::getType() const{return "LimitedDriverVolunteer";}