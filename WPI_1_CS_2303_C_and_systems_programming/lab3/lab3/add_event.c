//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Adds an event sorted by arrival time
void add_event(struct node **eventList, struct node *newNode)
{
	if(*eventList == NULL)
	{
		//if eventList is the end of the list, add newNode to the end
		*eventList = newNode;
	}
	else if( (*eventList)->arrival_time >= newNode->arrival_time)
	{
		//If eventList's first element arrives later than newNode, put newNode in front of it
		newNode->next = *eventList;
		*eventList = newNode;
	}
	else
	{
		//Otherwise, recurse using the next element of eventList
		add_event(&((*eventList)->next), newNode);
	}//end conditional
}//end add_event