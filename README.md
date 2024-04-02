# Warehouse Management System

Welcome to the Food Warehouse Management System project! The system simulates the operations of a warehouse where volunteers handle orders made by customers. It is designed as a command-line program and utilizes object-oriented programming principles, including classes, standard data structures, and the "Rule of 5" in C++.
## 

![image](https://github.com/miniNemiku/Warehouse-Management/assets/155912382/74f12c68-a0ad-4435-a63e-a8cc39da9a17)

## How To Run

### Installation
1. Clone the repository to your local machine.
2. Navigate to the project directory.

### Usage
1. Compile the project using `make`:
    ```bash
    make
    ```
2. Run the warehouse management system:
    ```bash
    ./bin/warehouse <path_to_configuration_file>
    ```
    

## Actions

---

### SimulateStep
Perform a step in the simulator, advancing one unit of time. 

Syntax: `step <number_of_steps>`
Example: `step 3`

---

### AddOrder
Create a new order with a specified customer ID.

Syntax: `order <customer_id>`
Example: `order 4`

---

### AddCustomer
Create a new customer.

Syntax: `customer <customer_name> <customer_type> <customer_distance> <max_orders>`
Example: `customer Dani soldier 7 3`

---

### PrintOrderStatus
Print information about a specific order, including its status, associated volunteers, and the customer who placed it.

Syntax: `orderStatus <order_id>`
Example: `orderStatus 3`

---

### PrintCustomerStatus
Print all information about a particular customer, including their ID, order statuses, and remaining orders they can place.

Syntax: `customerStatus <customer_id>`
Example: `customerStatus 6`

---

### PrintVolunteerStatus
Print all information about a specific volunteer, including their ID, busy status, current order, time left, and remaining orders they can handle.

Syntax: `volunteerStatus <volunteer_id>`
Example: `volunteerStatus 1`

---

### PrintActionsLog
Print all actions performed by the user since the warehouse was opened, excluding the current action.

Syntax: `log`

---

### Close
Print all orders with their statuses and close the warehouse, exiting the program.

Syntax: `close`

---

### BackupWarehouse
Save all warehouse information (customers, volunteers, orders, and action history).

Syntax: `backup`

---

### RestoreWarehouse
Restore the backed-up warehouse status and overwrite the current warehouse status.

Syntax: `restore`

## Authors
Matan Elkaim & Adi Shugal
