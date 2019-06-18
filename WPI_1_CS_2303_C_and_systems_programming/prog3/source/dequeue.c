//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

// Pops the front of the queue
struct node *dequeue(struct node **head)
{
	if(isEmpty(*head))
	{
		return NULL;
	}
	else
	{	//Will work for queues of one element.
		struct node *removedNode = *head;
		*head = (*head)->next;
		
		return removedNode;
	}
}
