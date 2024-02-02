#include "../include/Action.h"
#include <iostream>

using std::string;
using std::vector;
extern WareHouse* backup;

//======================================CONSTRUCTOR======================================

BaseAction::BaseAction() : errorMsg("Error:"), status(ActionStatus::ERROR){};

//======================================METHODS-BASIC======================================

ActionStatus BaseAction::getStatus() const{return status;}
void BaseAction ::complete(){status = ActionStatus::COMPLETED;}

void BaseAction ::error(string errorMsg){
    status = ActionStatus::ERROR;
    std::cerr << getErrorMsg() + errorMsg << std::endl;
}

string BaseAction ::getErrorMsg() const{return errorMsg;}

//======================================INHERITANCE======================================
//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
void SimulateStep::act(WareHouse &wareHouse){
    for(int i = 0; i < numOfSteps; i++){
        for (Volunteer *vol : wareHouse.getVolunteers()){
            for (Order *order : wareHouse.getPendingOrders()){
                if(vol-> canTakeOrder(*order)){
                    if (order->getStatus() == OrderStatus::PENDING){ 

                        if(typeid(*vol) == typeid(CollectorVolunteer) || typeid(*vol) == typeid(LimitedCollectorVolunteer)){
                            wareHouse.setOrderCollector(order,vol->getId());
                            if(typeid(*vol) == typeid(CollectorVolunteer)){
                                CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(vol);
                                wareHouse.setCollectorOrder(order,collector);
                            }
                            else{
                                LimitedCollectorVolunteer* lCollector = dynamic_cast<LimitedCollectorVolunteer*>(vol);
                                wareHouse.setLimitedCollectorOrder(order,lCollector);
                            }                            
                        }

                    }else if(order->getStatus() == OrderStatus::COLLECTING){
                        if(typeid(*vol) == typeid(DriverVolunteer) || typeid(*vol) == typeid(LimitedDriverVolunteer)){
                            wareHouse.setOrderDriver(order,vol->getId());
                            if(typeid(*vol) == typeid(DriverVolunteer)){
                                DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(vol);
                                wareHouse.setDriverOrder(order,driver);
                            }else{
                                LimitedDriverVolunteer* lDriver = dynamic_cast<LimitedDriverVolunteer*>(vol);
                                wareHouse.setLimitedDriverOrder(order,lDriver);
                            }
                        }
                    }
                }
                 
            }
            vol->step();
            if (!vol->isBusy()){ 
                for (Order *order : wareHouse.getInProcessOrders()){
                    if (order->getId() == vol->getCompletedOrderId()){  
                        if (typeid(*vol) == typeid(LimitedCollectorVolunteer)){ 
                            LimitedCollectorVolunteer *limitedCollector = dynamic_cast<LimitedCollectorVolunteer*>(vol);
                            wareHouse.removeLimitedCollectorVolunteer(limitedCollector);
                            order->switchNeedTransfer(true);       
                        }
                        else if (typeid(*vol) == typeid(CollectorVolunteer)){
                            order->switchNeedTransfer(true);       
                        }
                        else if (typeid(*vol) == typeid(LimitedDriverVolunteer)){
                            wareHouse.setOrderCompleted(order);
                            LimitedDriverVolunteer *limitedDriver = dynamic_cast<LimitedDriverVolunteer*>(vol);
                            wareHouse.removeLimitedDriverVolunteer(limitedDriver);
                        }
                        else{wareHouse.setOrderCompleted(order);}
                    }
                }
            }
        }
        wareHouse.addBackPending();
    }
    complete();
}

//======================================CLONE+BASIC======================================

string SimulateStep::toString() const{return "simulateStep " + std::to_string(numOfSteps);}
SimulateStep *SimulateStep::clone() const{return new SimulateStep(*this);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

AddOrder::AddOrder(int id) : customerId(id) {}
void AddOrder::act(WareHouse &wareHouse){
    try
    {
        Customer& customer = wareHouse.getCustomer(customerId);
        int orderId = wareHouse.getInProcessOrders().size() + wareHouse.getPendingOrders().size() + wareHouse.getCompletedOrders().size();
        if(customer.addOrder(orderId) != -1){
            Order* order = new Order(orderId,customerId,customer.getCustomerDistance());// Status already pending in constructor.
            wareHouse.addOrder(order);
            complete();
        }
        else
            error("Cannot place this order");
    }
    catch (const std::out_of_range& ex){
        error("Customer Id doesn't exist");
    }
}

//======================================CLONE+BASIC======================================

string AddOrder::toString() const{return "order " + std::to_string(customerId);}
AddOrder *AddOrder::clone() const{return new AddOrder(*this);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

CustomerType AddCustomer::stringToCustomerType(string customerType){return (customerType == "soldier")
 ? CustomerType::Soldier : CustomerType::Civilian;}

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
:customerName(customerName), customerType(stringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders){};

void AddCustomer::act(WareHouse &wareHouse)
{
    if(customerType==CustomerType::Soldier){
        SoldierCustomer* customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCutomer(customer);
    }else{
        CivilianCustomer* customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCutomer(customer);
    }
    complete();
}

//======================================CLONE+BASIC======================================

string AddCustomer::toString() const{  
    if(customerType==CustomerType::Soldier) 
        return "customer "+ customerName +" Soldier "+std::to_string(distance) +  " " +std::to_string(maxOrders);
    else
        return "customer "+customerName+" Civilian "+std::to_string(distance) + " " +std::to_string(maxOrders);
}

AddCustomer *AddCustomer::clone() const{return new AddCustomer(*this);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) {}
void PrintOrderStatus::act(WareHouse &wareHouse) {
    try {
        Order &order = wareHouse.getOrder(orderId);
        std::cout << "OrderID: " << order.getId() << std::endl;
        std::cout << "OrderStatus: " + order.status_helper(order.getStatus()) << std::endl;
        std::cout << "CustomerID: " << order.getCustomerId() << std::endl;
        if(order.getCollectorId() == NO_VOLUNTEER)
            std::cout << "Collector:None "<< std::endl;
        else
            std::cout << "Collector: " << order.getCollectorId() << std::endl;

        if(order.getDriverId() == NO_VOLUNTEER)
            std::cout << "Driver:None "<< std::endl;
        else
            std::cout << "Driver: " << order.getDriverId() << std::endl;
        complete();
    } catch (const std::out_of_range &) {
        error( "Order doesn’t exist");
    }
}

//======================================CLONE+BASIC======================================

PrintOrderStatus *PrintOrderStatus::clone() const {return new PrintOrderStatus(*this);}
string PrintOrderStatus::toString() const {return "orderStatus " + std::to_string(orderId);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId) {}
void PrintCustomerStatus::act(WareHouse &wareHouse) {
    try {
        Customer &customer = wareHouse.getCustomer(customerId);
        std::cout << "CustomerID: " << customer.getId() << std::endl;
        for (int orderId : customer.getOrdersIds()) {
            Order &order = wareHouse.getOrder(orderId);
            std::cout << "OrderID: " << order.getId() << std::endl;
            std::cout << "OrderStatus: " + order.status_helper(order.getStatus()) << std::endl;
        }
        std::cout << "numOrdersLeft: " << customer.getMaxOrders() - customer.getNumOrders() << std::endl;
        complete();
    } catch (const std::out_of_range &) {
        error("Customer doesn’t exist");
    }
}

//======================================CLONE+BASIC======================================

PrintCustomerStatus *PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}
string PrintCustomerStatus::toString() const {return "costumerStatus " + std::to_string(customerId);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id) {}
void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    try {
        Volunteer &volunteer = wareHouse.getVolunteer(VolunteerId);
        std::cout << "VolunteerID: " << volunteer.getId() << std::endl;
        std::cout << "isBusy: " << (volunteer.isBusy() ? "True" : "False") << std::endl;
        if (volunteer.isBusy()) {
            std::cout << "OrderID: " << volunteer.getActiveOrderId()<< std::endl;
            if(typeid(volunteer) == typeid(CollectorVolunteer)){
                CollectorVolunteer& vol = dynamic_cast<CollectorVolunteer&>(volunteer);
                std::cout << "TimeLeft: " << vol.getTimeLeft() << std::endl;
                std::cout << "OrdersLeft:No Limit " << std::endl;
            }
            else if(typeid(volunteer) == typeid(LimitedCollectorVolunteer)){
                LimitedCollectorVolunteer& vol = dynamic_cast<LimitedCollectorVolunteer&>(volunteer);
                std::cout << "TimeLeft: " << vol.getTimeLeft() << std::endl;
                std::cout << "OrderLeft: " << vol.getNumOrdersLeft() << std::endl;
            }
            else if(typeid(volunteer)==typeid(DriverVolunteer)){
                DriverVolunteer& vol =dynamic_cast<DriverVolunteer&>(volunteer);
                std::cout << "DistanceLeft: " << vol.getDistanceLeft() << std::endl;
                std::cout << "OrdersLeft:No Limit " << std::endl; ;
            }
            else if(typeid(volunteer)==typeid(LimitedDriverVolunteer)){
                LimitedDriverVolunteer& vol = dynamic_cast<LimitedDriverVolunteer&>(volunteer);
                std::cout << "DistanceLeft: " << vol.getDistanceLeft() << std::endl;
                std::cout << "OrdersLeft:" << vol.getNumOrdersLeft() << std::endl;
            }
         }else {
            std::cout << "OrderID: None" << std::endl;
            std::cout << "TimeLeft: None" << std::endl;
            std::cout << "OrdersLeft:No Limit " << std::endl;
        }
        complete();
    } catch (const std::out_of_range &) {
        error("Volunteer doesn’t exist");
    }
}

//======================================CLONE+BASIC======================================

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {return new PrintVolunteerStatus(*this);}
string PrintVolunteerStatus::toString() const {return "volunteerStatus " + std::to_string(VolunteerId);}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

PrintActionsLog::PrintActionsLog(): BaseAction(){}
void PrintActionsLog::act(WareHouse &wareHouse) {
    for (BaseAction *action : wareHouse.getActions()) 
        if(action!=this)
            std::cout << action->toString() << (action->getStatus() == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR") << std::endl;
    complete();
}

//======================================CLONE+BASIC======================================

PrintActionsLog *PrintActionsLog::clone() const {return new PrintActionsLog(*this);}
string PrintActionsLog::toString() const {return "log";}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

Close::Close() {}
void Close::act(WareHouse &wareHouse) {
    for (Order *order : wareHouse.getPendingOrders()) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()<< " , OrderStatus: " << order->status_helper(order->getStatus()) << std::endl;
    }
    for (Order *order : wareHouse.getInProcessOrders()) {
         std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()<< " , OrderStatus: " << order->status_helper(order->getStatus()) << std::endl;
    }
    for (Order *order : wareHouse.getCompletedOrders()) {
         std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()<< " , OrderStatus: " << order->status_helper(order->getStatus()) << std::endl;
    }
    complete();
    wareHouse.close();
}

//======================================CLONE+BASIC======================================

Close *Close::clone() const {return new Close(*this);}
string Close::toString() const {return "Close";}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(WareHouse &wareHouse) {
    if(backup) // Delete old backup
        delete backup;
    backup = new WareHouse(wareHouse); // Using copy constructor
    complete();
}

//======================================CLONE+BASIC======================================

BackupWareHouse *BackupWareHouse::clone() const {return new BackupWareHouse(*this);}
string BackupWareHouse::toString() const {return "backup";}

//======================================CONSTRUCTOR======================================
//===========================================ACT=========================================

RestoreWareHouse::RestoreWareHouse() {}
void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        wareHouse.deleteBackup(backup);
        complete();
    } else 
        error("No backup available");
}

//======================================CLONE+BASIC======================================
RestoreWareHouse *RestoreWareHouse::clone() const {return new RestoreWareHouse(*this);}
string RestoreWareHouse::toString() const {return "restore";}