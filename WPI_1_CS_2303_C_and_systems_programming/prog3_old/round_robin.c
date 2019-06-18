#include "header.h"

//Runs a simulation of a round robin scheduling queue
struct node *round_robin(struct node *eventList, int time_slice)
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
		while(list != NULL && list->arrival <= now){
			
			temp = list;
			list = list->next;
			
			if(queue_head == NULL)
			{
				queue_head = temp;
				queue_tail = temp;
				queue_tail->next = NULL;
			}
			else //queue tail != NULL
			{
				queue_tail->next = temp;
				queue_tail = temp;
				queue_tail->next = NULL;
			}
			
			if(VERBOSE)
			{
				printf("adding item to queue: ");
				print_node(temp);				
			}

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
		
		//If it is time to swap out processes
		if(servicing_process != NULL && servicing_time % time_slice == 0 && servicing_time != 0 && queue_head != NULL)
		{
			servicing_process->cpu_time -= servicing_time;
			
			
			if(queue_head == NULL)
			{
				queue_head = servicing_process;
				queue_tail = servicing_process;
			}
			else
			{
				queue_tail->next = servicing_process;
				servicing_process->next = NULL;
				queue_tail = servicing_process;
				servicing_process = NULL;
			}
		}//end swap out processes
		
		//Pick the next process to be run
		if(servicing_process == NULL && queue_head != NULL)
		{
			if(VERBOSE)
			{
				printf("Beginning process: ");
				print_node(queue_head);
			}
			
			servicing_process = queue_head;
			servicing_time = 0;
			
			queue_head = queue_head->next;
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