# README
### Assignment: Project 2: Implementing Reliable Data Transfer Protocol
#### Author: Ezra Davis (edavis@wpi.edu)
#### Course: CS3516 B15
#### Professor: Krishna Venkatasubramanian



## Short Description:

This project creates a simulation of the Go-Back-N protocol described in our textbook with a window size of 8.


## Building the project:

Run `make` to build the project.

Alternately, you can run `gcc -Wall project2.c student2A.c student2B.c student_common.c -o p2`

## Running the project.

Simply type `./p2` to start the project, then answer the prompts to run it.


### Note:
This program was primarily tested with high amounts of packet loss (0.0) corruption (0.9), out-of-order packets (0 to 0.9) and large volumes of packets (~490).