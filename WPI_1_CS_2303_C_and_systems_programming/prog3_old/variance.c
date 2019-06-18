#include "header.h"

//Gets the variance of the response time
double variance(struct node *data)
{
	double mean_response_time = mean(data);
	
	//Compute variance: (why didn't he ask for standard deviation?)
	struct node *next_node = data;
	
	double variance = 0.0;
	
	int mean_difference;
	
	int count = 0;
	
	while(next_node != NULL)
	{
		mean_difference = (next_node->completed - next_node->arrival) - mean_response_time;
		variance += (mean_difference * mean_difference);
		
		next_node = next_node->next;
		count++;
	}
	
	return variance / (double) count;
}