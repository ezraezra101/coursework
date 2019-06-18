#include "header.h"

//creates a node, initiallizes node.next to NULL
struct node *create_node(int pid, int arrival, int cpu_time)
{
	struct node *newNode = malloc(sizeof(struct node));
	
	if(newNode != NULL)
	{
	
		newNode->next = NULL;//The next in the queue/linked list
	
		newNode->pid = pid; //process id
		newNode->arrival = arrival; //arrival time
		newNode->cpu_time = cpu_time; //CPU time required to finish the task

		newNode->type = 'A'; //'A' for arrivals/active, 'C' for completed
		newNode->completed = 0; //Completed time (unimportant for type 'A')

		return newNode;
	}
	else
	{
		printf("Out of space. Memory allocation failed.");
		return NULL;
	}//end conditional
}//end create_node