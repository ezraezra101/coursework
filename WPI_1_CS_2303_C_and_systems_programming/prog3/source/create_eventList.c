//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//creates a list sorted by time from the input stream
struct node *create_eventList(int samples)
{
	printf("Please put in the process ID, arrival time, and the required CPU processing time for each process.\n");
		
		
	if(samples >= 1)
	{
		int pid, arrival, cpu; //values for the new node
		
		//create the first node
		scanf("%d %d %d", &pid, &arrival, &cpu);
		struct node *head = create_node(pid, arrival, cpu);

		int i; //counter
		for(i=1; i<samples; i++){
			scanf("%d %d %d", &pid, &arrival, &cpu);
			insert_node(&head, create_node(pid, arrival, cpu));
		}
		
		printf("Input accepted\n");
		
		return head;
	}
	else
		return NULL;
	
}
