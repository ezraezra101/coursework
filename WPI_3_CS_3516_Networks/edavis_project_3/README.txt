# README
### Assignment: Project 3: Distance Vector Routing Protocol
#### Author: Ezra Davis (edavis@wpi.edu)
#### Course: CS3516 B15
#### Professor: Krishna Venkatasubramanian



## Short Description:

This project implements an asynchronous distance vector routing protocol.


## Building the project:

Run `make` to build the project.

Alternately, you can run `gcc project3.c node0.c node1.c node2.c node3.c common.c -o project3`

## Running the project.

Simply type `./project3 1` to start the project, then answer the prompts to run it.
TraceLevel:
	< 0	Nothing is printed out
	0	Function calls and packets are sent out
	1	Distance tables are printed out after rtupdate
	2	Changes to distance tables printed out, as well as rcv events. Also prints out *full* distance tables.


### Details:

common.c and common.h contain almost all of the implementation of the assignment.
I've also implemented poisoned reverse.
