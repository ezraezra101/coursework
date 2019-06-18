#Ezra Davis
#Lab4 CS 2303
#February 2014

CPP = g++
CFLAGS = -Wall -c
LFLAGS = -Wall

edavis_lab4: main.cpp
	$(CPP) $(LFLAGS) -o edavis_lab4 main.cpp

#builds, runs tests, and cleans
run: edavis_lab4
	make test

test: edavis_lab4 lab4.dat
	./edavis_lab4 10 <lab4.dat
	
