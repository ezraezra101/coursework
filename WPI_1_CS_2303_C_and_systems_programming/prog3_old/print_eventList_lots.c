//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "header.h"

//Prints the linked list
void print_eventList(struct node *eventList)
{
	printf("Pid\ttime\tcpu\n");
	while(eventList != NULL)
	{
		print_node(eventList);			
		eventList = eventList->next;
		
	}
}//end print_eventlist