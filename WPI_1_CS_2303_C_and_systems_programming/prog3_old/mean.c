#include "header.h"
//returns the mean of the response times
double mean(struct node *data)
{
	double sum_response_time = 0;
	
	struct node *next_node = data;
	
	int count = 0;
	while(next_node != NULL)
	{
		sum_response_time += next_node->completed - next_node->arrival;
		
		
		
		count++;
		next_node = next_node->next;
		
	}
	
	return sum_response_time / (double) count;
}