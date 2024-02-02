all:	clean compile link cleanObjAndRun
	
clean:
	rm -f ./bin/*

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Order.cpp -o ./bin/Order.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Customer.cpp -o ./bin/Customer.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Volunteer.cpp -o ./bin/Volunteer.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/Action.cpp -o ./bin/Action.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/WareHouse.cpp -o ./bin/WareHouse.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/main.cpp -o ./bin/main.o
link:
	g++ -g -Wall -Weffc++ -std=c++11 -o ./bin/warehouse ./bin/Order.o ./bin/Customer.o ./bin/Volunteer.o ./bin/Action.o ./bin/WareHouse.o ./bin/main.o


cleanObjAndRun:
	rm -f *.o
	clear
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse
 