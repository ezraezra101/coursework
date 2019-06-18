//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//Inserts a node into the queue based on arrival time
void insert_node_queue(struct node **head, struct node **tail, struct node *newNode)
{
	insert_node(head, newNode);
	
	//If the list was empty or the newNode was added to the end, move tail.
	if(*tail == NULL || ((*tail)->next != NULL))
	{
		*tail = newNode;
	}
}
