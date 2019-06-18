######
TODO::
Print avg packet response time per source
Format output text
Remove Frivolous tests
Create output file

######


Author:				Ezra Davis & Artian Kica
Date:				March 5 2014
Version:			##############
Project ID:			lab5
CS Class:			CS 2303
Programming Language:		C++
OS/Hardware dependencies:	No Significant dependencies

Problem Description:		a statement of the requirements
	#########
Overall Design:			main design decisions about the program
	#########
	System structure	
	Data representation	
	Algorithms 		

Program Assumptions 
      and Restrictions:		what needs to be true for the program to work
	  ########

Interfaces:			how the program interacts with users, data or programs
	#############
	User
	File/D-B
	Program/Module

Implementation Details:
	This implementation of lab 5 uses static variables in the Node class to keep track of what needs to happen when.
	
	Node::eventList is a linkedList containing all Packets currently being transmitted or propagated.
	Node::packetActions() checks each Packet on the eventList that should be leaving transit to go to the next location on its packetPath.

	Data			implementation details of data representation
	Variables		key variables and their scopes
	Algorithm		implementation details of algorithm(s) used

How to build the program:
	use the make command from the command line:
		make

Program Source:
	##############

Additional Files:
	README.txt Makefile ############

########
Results:			listing of sample run(s) of the program

Test Procedures:		how the program was tested
Test Data:			test cases

Performance Evaluation:		how well the program performs
	Time/Space
	User Interface

References:	
	Assignment PDF: http://web.cs.wpi.edu/~rek/Systems/C14/Program5_C14.pdf