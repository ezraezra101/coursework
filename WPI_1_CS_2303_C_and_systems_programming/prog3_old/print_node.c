//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "header.h"

//Prints the values of a single node
void print_node(struct node *nodePtr)
{
	printf("%d\t%d\t%d\t%c\t%d\n",
				nodePtr->pid,
				nodePtr->arrival,
				nodePtr->cpu_time,
				nodePtr->type,
				nodePtr->completed);

}//end print_node
