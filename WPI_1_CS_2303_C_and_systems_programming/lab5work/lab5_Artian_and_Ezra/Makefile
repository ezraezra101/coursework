#Makefile for lab5
#Written by Ezra Davis
#	with some help from the internet (the VPATH statement)

CPP = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
DEBUGFLAGS = -Wall -g

OBJECTS = main.o Mule.o Node.o Source.o Receiver.o Packet.o

HEADERS = Queue.tcc LinkedList.tcc header.h

#Adds source to the path of sourcefiles used in the dependencies
VPATH=source

lab5: $(OBJECTS) $(HEADERS)
	$(CPP) $(LFLAGS) -o $@ *.o
	
#Compiles all the object files needed
%.o: %.cc header.h
	$(CPP) $(CFLAGS) -o $@ $<

#Outputs a debuggable version of lab5
debuggable: $(OBJECTS) $(HEADERS)
	$(CPP) $(DEBUGFLAGS) -o $@ *.o

#Removes all object files
clean:
	rm -f *.o



#Builds program from scratch, cleans up after itself, and runs the program
run:
	@echo "Cleaning (just in case)"
	@make clean	
	@echo "\n  Compiling..."
	@make lab5
	@echo "\n  Cleaning up ..."
	@make clean
	@echo "\n  Running ..."
	./lab5