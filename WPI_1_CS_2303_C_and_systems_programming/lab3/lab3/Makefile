#Ezra Davis
#Lab 3 CS 2303
#January 2014

edavis_lab3: main.o print_eventList.o get_nextevent.o add_event.o create_node.o print_node.o lab3.h
	gcc -o edavis_lab3 main.o print_eventList.o get_nextevent.o add_event.o create_node.o print_node.o
	
main.o: main.c lab3.h
	gcc -Wall -c main.c

print_eventList.o: print_eventList.c lab3.h
	gcc -Wall -c print_eventList.c

get_nextevent.o: get_nextevent.c lab3.h
	gcc -Wall -c get_nextevent.c
	
add_event.o: add_event.c lab3.h
	gcc -Wall -c add_event.c
	
create_node.o: create_node.c lab3.h
	gcc -Wall -c create_node.c

print_node.o: print_node.c lab3.h
	gcc -Wall -c print_node.c

clean:
	rm *.o
	
#builds and runs tests	
run: edavis_lab3 #for my text editor & testing purposes
	make
	make test
	make clean
	
#Runs tests
test: edavis_lab3 lab3.dat lab3-2.dat
	./edavis_lab3 4 <lab3-2.dat >eventlist-2.txt
	./edavis_lab3 10 <lab3.dat >eventlist.txt
	cat eventlist-2.txt

##end of Makefile
