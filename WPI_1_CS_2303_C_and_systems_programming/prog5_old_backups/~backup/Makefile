#Makefile for lab5
#Written by Ezra Davis
#	with some help from the internet (the VPATH statement)

CPP = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
DEBUGFLAGS = -Wall -g

MAIN = manet-sim

OBJECTS = main.o Mule.o Node.o Source.o Receiver.o Packet.o Field.o

HEADERS = Queue.tcc LinkedList.tcc header.h

#Adds source to the path of sourcefiles used in the dependencies
VPATH=source

$(MAIN): $(OBJECTS) $(HEADERS)
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

backup:
	@echo -e "^[[31;43mBacking up to the ~backup folder^[[0m"
	mkdir -p ~backup
	rm -fr ~backup/*
	@#Regex fu from: Gareth Solbeck
	cp -r [^~]* ~backup/

test: $(MAIN) prog5.txt
	./$(MAIN) 20 10 24 30 < prog5.txt

#Builds program from scratch, runs the program, and cleans up after itself
run:
	@echo "Cleaning (just in case)"
	@make clean	
	@echo "\n  Compiling..."
	@make manet-sim
	@echo "\n  Running tests ..."
	@make test
	@echo "\n  Cleaning up ..."
	@make clean
