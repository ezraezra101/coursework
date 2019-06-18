#include "header.h"

//Inserts a node into the list sorted by arrival time, if there are ties, its sorted by process id
void insert_node(struct node **list, struct node *newNode)
{
	
	if(VERBOSE){
		printf("Inserting node: ");
		print_node(newNode);
	}

	
	//if the newNode belongs at the front of the list
	if(*list == NULL || (newNode->arrival <= (*list)->arrival && newNode->pid < (*list)->pid))
	{
		newNode->next = *list;
		*list = newNode;
	}
	else
	{ //if the list isn't empty
		
		struct node *tempNode = *list;
		
		//find newNode's place
		while(tempNode->next != NULL && (newNode->arrival > tempNode->next->arrival ||
			//if they have the same arrival time & newNode's process id is greater than that of the one it will be inserted before
			(newNode->arrival == tempNode->next->arrival && newNode->pid >= tempNode->next->pid)))
		{
			tempNode = tempNode->next;
		}
		
		//and insert it
		newNode->next = tempNode->next;
		tempNode->next = newNode;
	}
}