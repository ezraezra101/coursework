//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("usage: %s [sources] [time-slice]\n",argv[0]);
		return 1;
	}
	else if(atoi(argv[1]) == 0)
	{
		printf("usage: %s [sources] [time-slice]\nSources must be an integer greater than one.", argv[0]);
	} //end usage if
	
	int sources = atoi(argv[1]);
	int timeSlice = atoi(argv[2]); //used for Round Robin scheduling
	
	struct node *eventList = create_eventList(sources);
	
	//Reading back input
	if(VERBOSE){
		printf("\nInput is:\n");
		print_eventList(eventList);	
	}
	printf("\n\n");	

	//Creating the output folder:
	mkdir(OUTLOCATION, 0777);
	//mkdir doesn't set permissions correctly because of umask
	chmod(OUTLOCATION, 0777);
	
	//Removing old files:
	remove(OUTLOCATION "/inputData.txt");
	remove(OUTLOCATION "/round_robin_queue.txt");
	remove(OUTLOCATION "/preemptive_rr_queue.txt");
	remove(OUTLOCATION "/FCFS_queue.txt");
	
	
	options_to_file(eventList, argv[0], sources, timeSlice, OUTLOCATION "/inputData.txt");
	
	
	if(timeSlice > 0){
		printf("Running Round Robin simulation\n");

		struct node *round_robinList = listcpy(eventList);
		struct node *round_robin_result = round_robin(round_robinList, timeSlice);
		
		print_to_file(round_robin_result, OUTLOCATION "/round_robin_queue.txt");
		free_list(round_robin_result);
		
		
		printf("Running Pre-emptive Round Robin simulation\n");
		
		struct node *preemptiveList = listcpy(eventList);
		struct node *preemptiveResult = preemptive_rr(preemptiveList, timeSlice);
		
		print_to_file(preemptiveResult, OUTLOCATION "/preemptive_rr_queue.txt");
		free_list(preemptiveList);
	}


	printf("Running FCFS simulation\n");
	
	struct node *fcfsList = listcpy(eventList);
	
	struct node *fcfs_result = fcfs(fcfsList);
	
	print_to_file(fcfs_result, OUTLOCATION "/FCFS_queue.txt");

	free_list(fcfs_result);
	
	
	free_list(eventList);

	printf("\n\nResults are in the " OUTLOCATION " folder\n" );
	
	return 0;
} //end main
