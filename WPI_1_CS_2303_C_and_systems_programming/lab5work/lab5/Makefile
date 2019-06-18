#Makefile for lab5

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
	
	
%.o: %.cc header.h
	$(CPP) $(CFLAGS) -o $@ $<

debuggable: $(OBJECTS) $(HEADERS)
	$(CPP) $(DEBUGFLAGS) -o $@ *.o
	
clean:
	rm -f *.o
	
run:
	@echo "Cleaning (just in case)"
	@make clean	
	@echo "\n  Compiling..."
	@make lab5
	@echo "\n  Cleaning up ..."
	@make clean
	@echo "\n  Running ..."
	./lab5