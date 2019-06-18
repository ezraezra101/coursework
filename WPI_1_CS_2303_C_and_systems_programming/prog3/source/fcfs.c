//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//Runs a simulation of a FCFS scheduling queue
struct node *fcfs(struct node *eventList)
{
	
	int now = 0; //current time
	
	//Pointer to the list going into the queue
	struct node *list = eventList;
	
	//the scheduler queue
	struct node *queue_head = NULL; 
	struct node *queue_tail = NULL;
	
	//List of completed items
	struct node *completed = NULL;
	
	//Temporary value
	struct node *temp = NULL;
	
	int servicing_time = 0; //time the current process has been running
	struct node *servicing_process = NULL; //the currently running process
	
	//Looping as time goes by
	while(list != NULL || servicing_process != NULL)
	{
		//Add items from eventList to queue
		while(list != NULL && list->arrival <= now)
		{
			
			temp = list;
			list = list->next;
			
			enqueue(&queue_head, &queue_tail, temp);
		}
		
		
		//Decide whether a process has ended
		if(servicing_process != NULL && servicing_process->cpu_time <= servicing_time)
		{
			servicing_process->type = 'C';
			servicing_process->completed = now;
			
			if(VERBOSE)
			{
				printf("Completed process: ");
				print_node(servicing_process);
			}
			
			insert_node(&completed, servicing_process);
			
			servicing_process = NULL;
			
		}
		
		
		
		//Pick the next process to be run
		if(servicing_process == NULL && queue_head != NULL)
		{
			if(VERBOSE)
			{
				printf("Beginning process: ");
				print_node(queue_head);
			}
			
			servicing_process = dequeue(&queue_head);
			servicing_time = 0;
		}


		if(VERBOSE)
		{
			if(servicing_process == NULL){
				printf("%d idle,\t\t\t", now);
			}
			else
				printf("At %d, has been doing process %d for %d units of time\t", now, servicing_process->pid, servicing_time);
			if(queue_head == NULL)
				printf("queue empty\n\n");
			else{
				printf("Queue:\n");
				print_eventList(queue_head);
				printf("\n");
			}
		}//end verbose section

		
		servicing_time++;
		now++;

	}//end while loop
	return completed;
}