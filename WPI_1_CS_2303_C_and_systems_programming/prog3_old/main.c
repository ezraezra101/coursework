#include "header.h"

int main(int argc, char **argv)
{
	if(argc != 3){
		printf("usage: %s [sources] [time-slice]\n",argv[0]);
		return 1;
	} //end usage if
	
	int sources = atoi(argv[1]);
	int timeSlice = atoi(argv[2]); //used for Round Robin scheduling
	
	struct node *eventList = create_eventList(sources);
	
	struct node *fcfsList = listcpy(eventList);
	struct node *round_robinList = listcpy(eventList);
	
	
	printf("\nInput is:\n");
	print_eventList(eventList);
	printf("\n\n");

	//Creating the output folder:
	mkdir(OUTLOCATION, 0777);
	//mkdir doesn't set permissions correctly because of umask
	chmod(OUTLOCATION, 0777);
	
	if(timeSlice > 0){
		printf("Running Round Robin simulation\n");
		print_to_file(round_robin(round_robinList, timeSlice), OUTLOCATION "/round_robin_queue.txt");		
	}


	printf("Running FCFS simulation\n");
	print_to_file(fcfs(fcfsList), OUTLOCATION "/FCFS_queue.txt");

	printf("\n\nResults are in the " OUTLOCATION " folder\n" );
	
	return 0;
} //end main