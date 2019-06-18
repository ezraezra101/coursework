//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//Frees the linked list
void free_list(struct node *head)
{
	struct node *toDelete;
	
	while(head != NULL)
	{
		toDelete = head;
		head = head->next;
		free(toDelete);
	}
}
