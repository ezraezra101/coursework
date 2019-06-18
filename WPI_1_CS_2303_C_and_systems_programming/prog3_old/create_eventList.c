#include "header.h"

//creates a list sorted by time from the input stream
struct node *create_eventList(int samples)
{
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
		return head;
	}
	else
		return NULL;
	
}