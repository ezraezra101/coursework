//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//creates and initializes a node
struct node * create_node(int process_id, int arrival_time, int cpu_time)
{
	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	if(newNode == NULL)
	{
		printf("Out of memory, malloc failed\n");
	}
	else
	{
		printf("pid: %d time: %d cpu: %d\n", process_id, arrival_time, cpu_time);
		newNode->process_id = process_id;
		newNode->arrival_time = arrival_time;
		newNode->cpu_time = cpu_time;
		newNode->next = NULL;
		return newNode;
	} //end conditional
	return newNode;
}//end create_node
