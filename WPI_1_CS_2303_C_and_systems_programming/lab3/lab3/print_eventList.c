//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Prints the linked list
void print_eventList(struct node *eventList)
{
	printf("Process ID\t| Arrival time\t| CPU time\t|\n");
	printf("----------------+---------------+---------------|\n");
	while(eventList != NULL)
	{
		print_node(eventList);			
		eventList = eventList->next;
		
	}
}//end print_eventlist