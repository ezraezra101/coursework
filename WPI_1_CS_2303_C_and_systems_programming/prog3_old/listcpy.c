#include "header.h"

//Copies a linked list
/*
struct node *listcpy(struct node *list)
{
	if(list != NULL)
	{
		//Creates space for the first struct in the new list
		struct node *newListHead = malloc(sizeof(struct node));
		struct node *newListTail = newListHead;
		
		//a pointer to traverse the old list
		struct node *nextPtr = list;
	
		//until it runs out of memory or reaches the end of the list
		while(newListTail != NULL){
			//Copies the the node to the new node
			memcpy(newListTail, nextPtr, sizeof(struct node));
			
			//Advance on the original list
			nextPtr = nextPtr->next;
			
			//Link the new list to a new location
			newListTail->next = malloc(sizeof(struct node));
			
			//advance the old list pointer
			newListTail = newListTail->next;
		
		}
	
		//Checking for running out of memory
		if(nextPtr != NULL)
		{
			printf("Out of memory, incomplete list copy");
		}
	
		return newListHead;
	} //end if
	else
	{
		return NULL;
	}

}// end listcpy

*/

struct node *listcpy(struct node *list)
{
	if(list != NULL)
	{
		struct node *newNode = malloc(sizeof(struct node));
		
		if(newNode != NULL){
			memcpy(newNode, list, sizeof(struct node));
		
			newNode->next = listcpy(list->next);
			if(VERBOSE)
			{
				printf("listcpy:\t");
				if(newNode->next != NULL)
				{
					print_node(newNode->next);
				}				
			}//end verbose if

		}
		else
		{
			printf("Out of memory, incomplete list copy");
		}

		return newNode;
	}
	else
	{
		return NULL;
	}
}

/*
struct node *nodecpy(struct node **lastNodeNext){
	struct node *newNode = malloc(sizeof(struct node));
	if(newNode != NULL){
		memcpy(newNode, (*lastNodeNext)->next, sizeof(struct node));
		
		return newNode;
	}
	else
	{
		printf("Out of memory, node not copied");
		return NULL;
	}
}


struct node *listcpy(struct node *list)
{
	if(list != NULL)
	{
		//create the first node
		struct node *head = malloc(sizeof(struct node));
		struct node *newNode;
		struct node *lastNode = head;

		//make the rest
		while(lastNode->next != NULL)
		{
			//create space for a new node
			newNode = malloc(sizeof(struct node));
		
			//if  we aren't out of memory
			if(newNode != NULL){
				//copy the node
				memcpy(newNode, lastNode->next, sizeof(struct node));
		
				lastNode->next = newNode;
			
				lastNode = newNode;
			}
			else
			{
				printf("Out of memory, incomplete list copy");
			}
		}//end while
		return head;
	}//end if
	else
	{
		return NULL;
	}
}*/
