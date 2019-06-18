#include "header.h"
//prints a completed simulation to file
int print_to_file(struct node *simList, char* fileName)
{
	FILE *out = fopen(fileName, "w");
	
	struct node *listElt = simList;
	
	fprintf(out, "Process ID\tArrival time\tCompletion time\tResponse time\n");
	
	while(listElt != NULL)
	{
		fprintf(out, "%10d\t%12d\t%15d\t%13d\n",
			listElt->pid, listElt->arrival, listElt->completed, listElt->completed - listElt->arrival);
		listElt = listElt->next;
	}
	
	fprintf(out, "\nMean response time:%lf\n", mean(simList) );
	
	fprintf(out, "Variance in response time:%lf\n", variance(simList) );
	
	fprintf(out, "All times are in 100ms units\n");
	return fclose(out);
}