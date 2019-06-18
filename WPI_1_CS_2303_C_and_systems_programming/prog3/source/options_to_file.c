//Ezra Davis
//Program 3 CS 2303
//February 2014

#include "header.h"

//prints the metadata of a simulation to a file
int options_to_file(struct node *simList, char* programName, int sources, int timeSlice, char* fileName)
{
	FILE *out = fopen(fileName, "w");
	
	fprintf(out, "Command line options:\t\t%s %d %d\n\n", programName, sources, timeSlice);
	
	fprintf(out, "Sources (number of processes):\t\t%d\n", sources);
	if(timeSlice > 0)
		fprintf(out, "Timeslice (used in Round Robin schedulers):\t%d\n", timeSlice);
	else
		fprintf(out, "\nTimeslice value is less than or equal to 0.\nRound Robin and Preemptive Round Robin schedulers disabled.\n");
	
	struct node *listElt = simList;
	
	fprintf(out, "\nInput data:\n");
	
	fprintf(out, "Process ID\tArrival time\tCPU time required\n");
	
	while(listElt != NULL)
	{
		fprintf(out, "%10d\t%12d\t%17d\n",
			listElt->pid, listElt->arrival, listElt->cpu_time);
		
		listElt = listElt->next;
	}
	
	
	fprintf(out, "\nAll times are in 100ms units\n");
	return fclose(out);
}
