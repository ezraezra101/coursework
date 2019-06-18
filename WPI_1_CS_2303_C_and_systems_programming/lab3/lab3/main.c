//Ezra Davis
//Lab 3 CS 2303
//January 2014
#include "lab3.h"

//Takes an argument from the command line and builds and manipulates a linked list
int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("Incorrect number of arguments.\nlab3 epects a single integer argument.\n");
		return 1;
	}//end if
	
	int sources = atoi(argv[1]); //# of processes
	
	struct node *eventList = NULL;//The event list
	
	int i; //Counter
	
	int process_id, arrival_time, cpu_time; //Values for new task/process
	struct node *newNodePtr;
	
	for(i=0; i<sources; i++)
	{
		//Takes input here
		scanf("%d %d %d", &process_id, &arrival_time, &cpu_time);
		newNodePtr = create_node(process_id, arrival_time, cpu_time);
		add_event(&eventList, newNodePtr);
	}
	
	print_eventList(eventList);
	
	//There is a bit of unimportant memory leak in the next bit
	printf("\nThe first element's data:\n");
	struct node firstElt = get_nextevent(&eventList);
	print_node(&firstElt);
	
	printf("\nThe second element's data:\n");
	struct node secondElt = get_nextevent(&eventList);
	print_node(&secondElt);
	
	printf("\nThe rest of the list's data:\n");
	print_eventList(eventList);
}//end main

