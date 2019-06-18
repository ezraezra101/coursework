//////////////////////////////////////////////////////////////////////////////////////////////////////////////

Author:				Team 4J: Ezra Davis & Artian Kica

Date:				March 5 2014

Version:			1.0

Project ID:			prog5

CS Class:			CS 2303

Programming Language:		C++

OS/Hardware dependencies:	No Significant dependencies

Problem Description:	
	Simulates the sending and receiving of packets on a Manet map (Mobile Ad hoc NETwork).

	
More information:
		http://web.cs.wpi.edu/~rek/Systems/C14/Program5_C14.pdf

//////////Implementation Status: /////////////////////////////

	The delay variance per sender and priority queueing extra credit were both implemented.
	
	The extra variance of the means required for the priority queueing extra credit gives an incorrect value when compilied on the CCC servers.


//////////Options: /////////////////////////////

Options are at the top of source/header.h

They are:
	VERBOSE: Prints out all the events as they occur in the simulation.
	USING_PRIORITY_QUEUE: Determines whether the simulation is using a priority queue for the Mule nodes or not.
	DEBUG_MESG: Print out extra information for debugging.

//////////Design: //////////////////////////////
	

Represents nodes in the MANET map as classes that inherit from class Node.
 
Class Node has static functions and variables manages much of the control flow.

	
Most data is represented in the form of a LinkedList (sorted), or Queue, which are implemented using templates.

//////////Implementation Details://///////////////////////////

Node::eventList is a linkedList containing all Packets currently being transmitted or propagated.
	
Node::packetActions() checks each Packet on the eventList that should be leaving transit to go to the next location on its packetPath.

The Field class stores columns from -1 (where the sources are) to dimension (where the receivers are).


//////////How to build the program://////////////////////////////

use the make command from the command line:
		make

	
Alternatively, to build and run the program use:
		make run

//////////Source://////////////////////////////

All the files are in the source directory:
 
header.h
	
LinkedList.tcc (.tcc signifies that it is a template file)
		
Queue.tcc
		
main.cc

Field.cc

Mule.cc
		
Node.cc
		
Packet.cc
		
Receiver.cc
		
Source.cc


//////////Additional Files: //////////////////////////////

README.txt 
Makefile
prog5.txt (Test file)

	debuggable and manet-sim are the executable files, and may or may not be present.


//////////References: //////////////////////////////

	
Assignment PDF: http://web.cs.wpi.edu/~rek/Systems/C14/Program5_C14.pdf

	
Help with dependent type names: http://flamingdangerzone.com/cxx11/2012/05/27/dependent-names-bliss.html

//////////////////////////////////////////////////////////////////////////////////////////////////////////////