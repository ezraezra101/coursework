//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//For a linkedlist/stack
//In this case, just remembers the servicing_time associated with a node on the interrupted_stack
struct intNode {
	int val;
	struct intNode *next;
};


//Runs a simulation of a preemptive round robin scheduling queue
struct node *preemptive_rr(struct node *eventList, int time_slice)
{
	
	int now = 0; //current time
	
	//Pointer to the list going into the queue
	struct node *list = eventList;
	
	//the scheduler queue
	struct node *queue_head = NULL; 
	struct node *queue_tail = NULL;
	
	//Special for preemptive RR ######
	struct node *interrupted_stack = NULL;
	struct intNode *interrupted_stack_servicing_times = NULL;
	
	struct intNode *temp_intNode = NULL;
	
	//List of completed items
	struct node *completed = NULL;
	
	int servicing_time = 0; //time the current process has been running
	struct node *servicing_process = NULL; //the currently running process
	
	//Looping as time goes by
	while(list != NULL || servicing_process != NULL || interrupted_stack != NULL) //modified for preemptive RR ####
	{
		//Add items from eventList to queue
		//Modified for Preemptive RR #####
		while(list != NULL && list->arrival <= now){
			
			//Push servicing_process onto the top of the preempted stack
			if(servicing_process != NULL)
			{
				servicing_process->next = interrupted_stack;
				interrupted_stack = servicing_process;
				
				//adds the current servicing time to the appropriate stack
				temp_intNode = malloc(sizeof(struct intNode));
				if(temp_intNode==NULL){printf("Allocation of memory failed.");}
				temp_intNode->val = servicing_time;
				temp_intNode->next = interrupted_stack_servicing_times;
				interrupted_stack_servicing_times = temp_intNode;
			}
			
			servicing_process = list;
			servicing_time = 0;
			
			//temp = list;
			list = list->next;
			
			//enqueue(&queue_head, &queue_tail, temp);

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
		// Modified for preemptive RR #####
		if(servicing_process != NULL && servicing_time % time_slice == 0 && servicing_time != 0)
		{
			servicing_process->cpu_time -= servicing_time;
			
			enqueue(&queue_head, &queue_tail, servicing_process);
			
			servicing_process = NULL;
			
			
		}//end swap out processes
		
		//Pick the next process to be run
		if(servicing_process == NULL && (queue_head != NULL || interrupted_stack != NULL))//Modified ##
		{
			if(VERBOSE)
			{
				printf("Beginning process: ");
				print_node(queue_head);
			}
			
			//Modified ####
			if(interrupted_stack == NULL)
			{ //add item from queue
				servicing_process = queue_head;
				servicing_time = 0;
			
				queue_head = queue_head->next;
			}
			else
			{ //Return item from the interrupted stack
				//getting back the servicing_time
				servicing_time = interrupted_stack_servicing_times->val;
				
				temp_intNode = interrupted_stack_servicing_times; //fixing up the stack of the interruped servicing times
				interrupted_stack_servicing_times = interrupted_stack_servicing_times->next;
				free(temp_intNode);
				
				servicing_process = dequeue(&interrupted_stack);
			}

		}

		if(VERBOSE)
		{
			if(servicing_process == NULL){
				printf("%d idle,\t\t\t", now);
			}
			else
				printf("At %d, has been doing process %d for %d units of time\t", now, servicing_process->pid, servicing_time);
			if(interrupted_stack == NULL){
				printf("Interrupted Stack empty\t\t\t");
			}
			else{
				printf("Interrupted Stack:\n");
				print_eventList(interrupted_stack);
			}
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
