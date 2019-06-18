#include "header.h"

// Pops the front of the queue
struct node *dequeue(struct node **head, struct node **tail)
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