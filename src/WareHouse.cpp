#include "../include/WareHouse.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

//======================================CONSTRUCTOR======================================

WareHouse::WareHouse(const string &configFilePath) : isOpen(true),actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),customerCounter(0), volunteerCounter(0){
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()){throw std::runtime_error("Unable to open configuration file");}

    string line;
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line); // Parsing input from string
        string type;
        iss >> type;

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        if (type == "customer")
        {
            // Process customer information
            string customerName, customerType;
            int customerDistance, maxOrders;
            iss >> customerName >> customerType >> customerDistance >> maxOrders;
            if (customerType == "soldier"){
                customers.push_back(new SoldierCustomer(customerCounter, customerName, customerDistance, maxOrders));
                customerCounter = customerCounter + 1;
            }

            else if (customerType == "civilian"){
                customers.push_back(new CivilianCustomer(customerCounter, customerName, customerDistance, maxOrders));
                customerCounter = customerCounter + 1;
            }
        }
        else if(type == "volunteer")
        {
            // Process volunteer information
            string volunteerName, volunteerRole;
            int coolDown, maxDistance, distancePerStep, maxOrders=-1;
            iss >> volunteerName >> volunteerRole;
            if (volunteerRole == "collector" || volunteerRole == "limited_collector")
                iss >> coolDown;
            if (volunteerRole == "driver" || volunteerRole == "limited_driver")
                iss >> maxDistance >> distancePerStep;
            if (volunteerRole == "limited_driver" || volunteerRole == "limited_collector")
                iss >> maxOrders;

            if (volunteerRole == "driver"){
                volunteers.push_back(new DriverVolunteer(volunteerCounter, volunteerName, maxDistance, distancePerStep));
                volunteerCounter = volunteerCounter + 1;
            }
            else if (volunteerRole == "limited_driver" && maxOrders != 0){
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, volunteerName, maxDistance, distancePerStep, maxOrders));
                volunteerCounter = volunteerCounter + 1;
            }
            else if (volunteerRole == "limited_collector" && maxOrders != 0){
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, volunteerName, coolDown, maxOrders));
                volunteerCounter = volunteerCounter + 1;
            }
            else{
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, volunteerName, coolDown));
                volunteerCounter = volunteerCounter + 1;
            }

        }
    }

    configFile.close();
}


//======================================START======================================

void WareHouse::start()
{
    string input;
    cout << "WareHouse is open" << endl;
    while (isOpen)
    {
        cout << "Choose a method" << endl;
        cout << "step || order || customer || orderStatus || customerStatus || " << endl;
        cout << "volunteerStatus || log || close || backup || restore" << endl;
        getline (cin,input);
        if (input.substr(0, 4) == "step" && input.size() >= 5 && input.at(4) == ' ')
        {
            // Getting the digits from the input
            std::stringstream ss;
            for (char c : input) 
                if (std::isdigit(c)) 
                    ss << c;

            // Assuming step number is >0 (according to version 1.3)
            int stepNumber;
            ss >> stepNumber;
            SimulateStep *step = new SimulateStep(stepNumber);
            step->act(*this);
            addAction(step);
        }
        else if (input.substr(0, 11) == "orderStatus" && input.size() >= 12 && input.at(11) == ' ')
        {
            std::stringstream ss;
            for (char c : input) 
                if (std::isdigit(c)) 
                    ss << c;   

            int id;
            ss >> id;
            PrintOrderStatus *orderStatus= new PrintOrderStatus(id);
            orderStatus->act(*this);
            addAction(orderStatus);
        }
        else if (input.substr(0, 14) == "customerStatus" && input.size() >= 15 && input.at(14) == ' ')
        {
            std::stringstream ss;
            for (char c : input) 
                if (std::isdigit(c)) 
                    ss << c;

            int customerId;
            ss >> customerId;
            PrintCustomerStatus *customerStatus = new PrintCustomerStatus(customerId);
            customerStatus->act(*this);
            addAction(customerStatus);
        }
        else if (input.substr(0, 5) == "order" && input.size() >= 6 && input.at(5) == ' ')
        {
            std::stringstream ss;
            for (char c : input) 
                if (std::isdigit(c)) 
                    ss << c;

            int id;
            ss >> id;
            AddOrder *addOrder = new AddOrder(id);
            addOrder->act(*this);
            addAction(addOrder);
        }
        else if (input.substr(0, 8) == "customer" && input.size() >= 9 && input.at(8) == ' ')
        {
            istringstream iss(input); //parsing input to string/int
            string keyword, customerName, customerType;
            int distance, maxOrders;
            iss >> keyword >> customerName >> customerType >> distance >> maxOrders;
            AddCustomer *customer = new AddCustomer(customerName, customerType, distance, maxOrders);
            customer->act(*this);
            addAction(customer);
            
        }
        
        else if (input.substr(0, 15) == "volunteerStatus" && input.size() >= 16 && input.at(15) == ' ')
        {
            std::stringstream ss;
            for (char c : input) 
                if (std::isdigit(c)) 
                    ss << c;
                
            int volunId;
            ss >> volunId;
            PrintVolunteerStatus *volunStatus = new PrintVolunteerStatus(volunId);
            volunStatus->act(*this);
            addAction(volunStatus);
        }
        else if (input.substr(0, 3) == "log" && input.size() == 3)
        {
            PrintActionsLog *actionLog = new PrintActionsLog();
            actionLog->act(*this);
            addAction(actionLog);
        }
        else if (input.substr(0, 5) == "close" && input.size() == 5)
        {
            Close *close = new Close();
            close->act(*this);
            addAction(close);
        }
        else if (input.substr(0, 6) == "backup" && input.size() == 6)
        {
            BackupWareHouse *backup = new BackupWareHouse();
            backup->act(*this);
            addAction(backup);
        }
        else if (input.substr(0, 7) == "restore" && input.size() == 7)
        {
            RestoreWareHouse *restore = new RestoreWareHouse();
            restore->act(*this);
            addAction(restore);
        }
        else
            cout << "Invalid method" << endl;
    }
}

//======================================METHODS-BASIC======================================

void WareHouse::addOrder(Order *order){pendingOrders.push_back(order);}
void WareHouse::addAction(BaseAction *action){actionsLog.push_back(action);}

Customer &WareHouse::getCustomer(int customerId) const{
    for (Customer *customer : customers)
        if (customer->getId() == customerId)
            return *customer;

    throw std::out_of_range("Customer not found");
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    for (Volunteer *volunteer : volunteers)
        if (volunteer->getId() == volunteerId)
            return *volunteer;

    throw std::out_of_range("Volunteer not found");
}

// Searching in each Vector of orders for the orderId.
Order &WareHouse::getOrder(int orderId) const{
    for (Order *order : pendingOrders)
        if (order->getId() == orderId)
            return *order;

    for (Order *order : inProcessOrders)
        if (order->getId() == orderId)
            return *order;

    for (Order *order : completedOrders)
        if (order->getId() == orderId)
            return *order;

    throw std::out_of_range("Order not found");
}

const vector<Order *> &WareHouse::getPendingOrders() const{return pendingOrders;}
const vector<Volunteer *> &WareHouse::getVolunteers() const{return volunteers;}
const vector<Order *> &WareHouse::getInProcessOrders() const{return inProcessOrders;}
const vector<Order *> &WareHouse::getCompletedOrders() const{return completedOrders;}
const vector<BaseAction *> &WareHouse::getActions() const{return actionsLog;}

void WareHouse::removeOrderFromPending(int orderId) {
    for (auto it = pendingOrders.begin(); it != pendingOrders.end(); ++it) {
        if ((*it)->getId() == orderId) {
            pendingOrders.erase(it);
            break; // Exit the loop after erasing the element
        }
    }
}

void WareHouse::removeOrderFromProcessing(int orderId) {
    for (auto it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it) {
        if ((*it)->getId() == orderId) {
            inProcessOrders.erase(it);
            break; // Exit the loop after erasing the element
        }
    }
}

void WareHouse::addOrderToInProcessOrders(Order *order){inProcessOrders.push_back(order);}
void WareHouse::addOrderToCompletedOrders(Order *order){completedOrders.push_back(order);}
int WareHouse::getCustomerCounter(){return customerCounter;}
int WareHouse::getVolunteerCounter(){return volunteerCounter;}
void WareHouse::close(){isOpen = false;}
void WareHouse::open(){isOpen = true;}


void WareHouse:: addCutomer(Customer *customer){
    customers.push_back(customer);
    customerCounter++;
}
//======================================SETTERS+DELETERS========================================

void WareHouse::setOrderCollector(Order *order,int collectorId){
    order->setCollectorId(collectorId);
    order->setStatus(OrderStatus::COLLECTING);
    addOrderToInProcessOrders(order);
    removeOrderFromPending(order->getId());
}

void WareHouse::setOrderDriver(Order *order,int driverId){
    order->setDriverId(driverId);
    order->setStatus(OrderStatus::DELIVERING);
    addOrderToInProcessOrders(order);
    removeOrderFromPending(order->getId());
}

void WareHouse::setCollectorOrder(Order *order,CollectorVolunteer* vol){vol->acceptOrder(*order);}
void WareHouse::setLimitedCollectorOrder(Order *order, LimitedCollectorVolunteer* vol){vol->acceptOrder(*order);}
void WareHouse::setDriverOrder(Order *order,DriverVolunteer* vol){vol->acceptOrder(*order);}
void WareHouse::setLimitedDriverOrder(Order *order, LimitedDriverVolunteer* vol){vol->acceptOrder(*order);}

 void WareHouse::RemoveVolunteerById(int volunteerId) {
    auto it = volunteers.begin();
    while (it != volunteers.end()) {
        if ((*it)->getId() == volunteerId) {
            it = volunteers.erase(it);
        } else {
            ++it;
        }
    }
}

void WareHouse::removeLimitedCollectorVolunteer(LimitedCollectorVolunteer* vol){
    if (!vol->hasOrdersLeft()){
        int id = vol ->getId();
        RemoveVolunteerById(id);
        delete vol;
    }
}

void WareHouse::setOrderCompleted(Order *order){
    addOrderToCompletedOrders(order);
    removeOrderFromProcessing(order->getId());
    order ->setStatus(OrderStatus::COMPLETED);
}
void WareHouse::removeLimitedDriverVolunteer(LimitedDriverVolunteer* vol){
     if (!vol -> hasOrdersLeft()){
        int id = vol->getId();
        RemoveVolunteerById(id);
        delete vol;
    }   

}

void WareHouse:: addBackPending(){
    for(Order *order : getInProcessOrders()){
        if(order->getNeedTransfer()){
            order->switchNeedTransfer(false);
            addOrder(order);
            removeOrderFromProcessing(order->getId());
        }
    }
}

void WareHouse:: deleteBackup(WareHouse*& backup){
       *this = *backup; // Using assignment operator.
        delete backup;
        backup = nullptr;
}

//======================================RULE-OF-5======================================    

// DESTRUCTOR

WareHouse::~WareHouse(){
    // Delete old stuff
    for (Customer *customer : customers)
        if (customer){ //deleting the Element
            delete customer;
            customer = nullptr;
        }
    customers.clear(); //deleting the pointer

    // Delete dynamically allocated volunteers
    for (Volunteer *volunteer : volunteers)
        if (volunteer){
            delete volunteer;
            volunteer = nullptr;
        }
    volunteers.clear();

    // Delete dynamically allocated PendingOrders
    for (Order *order : pendingOrders)
        if (order){
            delete order;
            order = nullptr;
        }
    pendingOrders.clear();

    // Delete dynamically allocated InProcessOrders
    for (Order *order : inProcessOrders)
        if (order){
            delete order;
            order = nullptr;
        }
    inProcessOrders.clear();

    // Delete dynamically allocated completedOrders
    for (Order *order : completedOrders)
        if (order){
            delete order;
            order = nullptr;
        }
    completedOrders.clear();

    // Delete dynamically allocated actions
    for (BaseAction *action : actionsLog)
        if (action){
            delete action;
            action = nullptr;
        }
    actionsLog.clear();
}

// Copy Constructor

WareHouse::WareHouse(const WareHouse &other)
    : isOpen(other.isOpen),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter) {

    for (auto &vol : other.volunteers)
        volunteers.push_back(vol->clone());
    for (BaseAction *base : other.actionsLog)
        actionsLog.push_back(base->clone());
    for (Order *order : other.pendingOrders)
        pendingOrders.push_back(new Order(*order));
    for (Order *inProc : other.inProcessOrders)
        inProcessOrders.push_back(new Order(*inProc));
    for (Order *comp : other.completedOrders)
        completedOrders.push_back(new Order(*comp));
    for (Customer *cust : other.customers)
        customers.push_back(cust->clone());
}

//Copy Assignment Operator

WareHouse &WareHouse::operator=(const WareHouse &other)
{
    if (&other != this)
    {
        for (Customer *customer : customers){
            delete customer;
            customer = nullptr;
        }
        customers.clear();
        for (Volunteer *volunteer : volunteers){
            delete volunteer;
            volunteer = nullptr;
        }
        volunteers.clear();
        for (Order *order : pendingOrders){
            delete order;
            order = nullptr;
        }
        pendingOrders.clear();
        for (Order *inProc : inProcessOrders){
            delete inProc;
            inProc = nullptr;
        }
        inProcessOrders.clear();
        for (Order *comp : completedOrders){
            delete comp;
            comp = nullptr;
        }
        completedOrders.clear();
        for (BaseAction *action : actionsLog){
            delete action;
            action = nullptr;
        }
        actionsLog.clear();        
        
        // Add new stuff
        for (Volunteer *vol : other.volunteers)
            volunteers.push_back(vol->clone());
        for (BaseAction *base : other.actionsLog)
            actionsLog.push_back(base->clone());
        for (Order *order : other.pendingOrders)
            pendingOrders.push_back(new Order(*order));
        for (Order *inProc : other.inProcessOrders)
            inProcessOrders.push_back(new Order(*inProc));
        for (Order *comp : other.completedOrders)
            completedOrders.push_back(new Order(*comp));
        for (Customer *cust : other.customers)
            customers.push_back(cust->clone());

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        isOpen = other.isOpen;
    }
    return *this;
}

// Move Constructor ("Move" the ownership of the items.)

WareHouse::WareHouse(WareHouse &&other) noexcept
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter) {

    other.customers.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.actionsLog.clear();
}

// Move Assignment Operator

WareHouse &WareHouse::operator=(WareHouse &&other) noexcept
{
    if (this != &other) {
        // Clear existing data
        for (Customer *customer : customers){
            delete customer;
            customer = nullptr;
        }
        customers.clear();
        for (Volunteer *volunteer : volunteers){
            delete volunteer;
            volunteer = nullptr;
        }
        volunteers.clear();
        for (Order *order : pendingOrders){
            delete order;
            order = nullptr;
        }
        pendingOrders.clear();
        for (Order *inProc : inProcessOrders){
            delete inProc;
            inProc = nullptr;
        }
        inProcessOrders.clear();
        for (Order *comp : completedOrders){
            delete comp;
            comp = nullptr;
        }
        completedOrders.clear();
        for (BaseAction *action : actionsLog){
            delete action;
            action = nullptr;
        }
        actionsLog.clear(); 

        // Transfer ownership of resources from 'other' to the current object
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;

        customers = std::move(other.customers);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        actionsLog = std::move(other.actionsLog);

        // Reset 'other' to a valid state (resources are moved, no need to delete)
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
    }
    return *this;
}
