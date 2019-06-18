//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//prints a completed simulation to file
int print_to_file(struct node *simList, char* fileName)
{
	FILE *out = fopen(fileName, "w");
	
	struct node *listElt = simList;
	
	fprintf(out, "Process ID\tArrival time\tCPU time required\tCompletion time\tResponse time\n");
	
	while(listElt != NULL)
	{
		fprintf(out, "%10d\t%12d\t%17d\t%15d\t%13d\n",
			listElt->pid, listElt->arrival, listElt->original_cpu_time, listElt->completed, listElt->completed - listElt->arrival);
		
		listElt = listElt->next;
	}
	
	fprintf(out, "\nMean response time:\t\t%lf\n", mean(simList) );
	
	fprintf(out, "Variance in response time:\t%lf\n", variance(simList) );
	
	fprintf(out, "All times are in 100ms units\n");
	return fclose(out);
}
