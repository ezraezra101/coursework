//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Prints the linked list
void print_eventList(struct node *eventList)
{
	eventList = eventList->next; //Going past dummy first value
	printf("Pid\ttime\tcpu\n");
	while(eventList != NULL)
	{
		printf("%d\t%d\t%d\n",
			eventList->process_id, eventList->arrival_time, eventList->cpu_time);
			
		eventList = eventList->next;
		
	}
}//end print_eventlist