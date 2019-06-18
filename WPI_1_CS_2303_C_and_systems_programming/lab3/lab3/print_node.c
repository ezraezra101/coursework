//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Prints the values of a single node
void print_node(struct node *nodePtr)
{
	printf("PID: %5d\t| Time:%5d\t| CPU: %5d\t|\n",
				nodePtr->process_id,
				nodePtr->arrival_time,
				nodePtr->cpu_time);

}//end print_node
