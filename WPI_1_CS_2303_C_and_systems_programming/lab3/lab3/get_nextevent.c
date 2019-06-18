//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//pops the first element of eventList
struct node get_nextevent(struct node **eventList)
{
	struct node *popped = *eventList;
	
	*eventList = popped->next;
	
	return *popped;
}//end nextevent
