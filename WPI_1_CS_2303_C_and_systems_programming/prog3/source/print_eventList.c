//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//Prints the linked list
void print_eventList(struct node *eventList)
{
	printf("Pid\ttime\tcpu\ttype\tcompleted\n");
	while(eventList != NULL)
	{
		print_node(eventList);			
		eventList = eventList->next;
		
	}
}//end print_eventlist
