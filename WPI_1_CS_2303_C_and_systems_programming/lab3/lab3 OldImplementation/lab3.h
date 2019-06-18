//Ezra Davis
//Lab 3 CS 2303
//January 2014

#include <stdio.h>
#include <stdlib.h>

struct node {
	struct node *next;
	int process_id, arrival_time, cpu_time;
};

//pops the first element of eventList
struct node get_nextevent(struct node *eventList);

//Adds an event sorted by arrival time
void add_event(struct node **eventList, struct node *newNode);

//creates and initializes a node
struct node * create_node(int process_id, int arrival_time, int cpu_time);

//Prints the linked list
void print_eventList(struct node *eventList);