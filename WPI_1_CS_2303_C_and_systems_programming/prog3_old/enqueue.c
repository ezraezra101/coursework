#include "header.h"

//Adds a node to the back of the queue
void enqueue(struct node **head, struct node **tail, struct node *newNode)
{
		
	if(isEmpty(*head) || isEmpty(*tail))
	{	
		//Put the pointer at the front of the queue
		*head = newNode;
		*tail = newNode;
		(*tail)->next = NULL;
	}
	else
	{
		//Put the program at the back of the queue
		(*tail)->next = newNode;
		*tail = newNode;
		(*tail)->next = NULL;
	}
	
	if(VERBOSE)
	{
		printf("Adding item to queue: ");
		print_node(newNode);
	}
	
}