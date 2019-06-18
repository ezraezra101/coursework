//////////////////////////////////////////////////////////////////////////////////////////////////////////////

Author:				Team 4J: Ezra Davis & Artian Kica

Date:				March 5 2014

Version:			1.0

Project ID:			lab5

CS Class:			CS 2303

Programming Language:		C++

OS/Hardware dependencies:	No Significant dependencies


Problem Description:	
	A stepping stone to program 5 which simulates MANET nodes.

	
More information:
		http://web.cs.wpi.edu/~rek/Systems/C14/Lab5_C14.pdf

Overall 

//////////Design://////////////////////////////
	

Represents nodes in the MANET map as classes that inherit from class Node.
 
Class Node has static functions and variables manages much of the control flow.

	
Most data is represented in the form of a LinkedList (sorted), or Queue, which are implemented using templates



//////////Implementation Details:

/////////////////////////////

Node::eventList is a linkedList containing all Packets currently being transmitted or propagated.
	
Node::packetActions() checks each Packet on the eventList that should be leaving transit to go to the next location on its packetPath.




//////////How to build the program:
//////////////////////////////

use the make command from the command line:
		make

	
Alternatively, to build and run the program use:
		make run

Program 

//////////Source://////////////////////////////

All the files are in the source directory:

 
header.h
	
LinkedList.tcc (.tcc signifies that it is a template file)
		
Queue.tcc
		
main.cc
		
Mule.cc
		
Node.cc
		
Packet.cc
		
Receiver.cc
		
Source.cc



//////////Additional Files:
//////////////////////////////

README.txt 
Makefile 
packetsim.txt

	debuggable and lab5 are the executable files, and may or may not be present.




//////////Test Results:
	//////////////////////////////

Results are in the packetsim.txt file



//////////References:	//////////////////////////////

	
Assignment PDF: http://web.cs.wpi.edu/~rek/Systems/C14/Program5_C14.pdf

	
Help with dependent type names: http://flamingdangerzone.com/cxx11/2012/05/27/dependent-names-bliss.html

//////////////////////////////////////////////////////////////////////////////////////////////////////////////