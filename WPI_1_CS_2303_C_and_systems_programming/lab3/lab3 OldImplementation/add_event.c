//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Adds an event sorted by arrival time
void add_event(struct node **eventList, struct node *newNode)
{
	if((*eventList)->next == NULL)
	{
		//if eventList is the end of the list, add newNode to the end
		(*eventList)->next = newNode;
	}
	else if((*eventList)->next->arrival_time >= newNode->arrival_time)
	{
		newNode->next = (*eventList)->next;
		(*eventList)->next = newNode;
	}
	else
	{
		add_event(&((*eventList)->next), newNode);
	}//end conditional
}//end add_event