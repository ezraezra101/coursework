#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
//A unit of time is 100 milliseconds

//Debug option
#define VERBOSE false

#define OUTLOCATION "SimulationResults"

struct node {
	struct node *next; //The next in the queue/linked list
	int pid; //process id
	int arrival; //arrival time
	int cpu_time; //CPU time required to finish the task
	
	char type; //'A' for arrivals/active, 'C' for completed
	int completed; //Completed time
};

//Is the queue empty?
int isEmpty(struct node *head);

// Pops the front of the queue
struct node *dequeue(struct node **head, struct node **tail);

//Adds a node to the back of the queue
void enqueue(struct node **head, struct node **tail, struct node *newNode);

struct node *create_eventList(int samples);

//Inserts a node into the list sorted by arrival time, if there are ties, its sorted by process id
void insert_node(struct node **list, struct node *newNode);

//copies a linked list
struct node *listcpy(struct node *list);

//Prints the values of a single node
void print_node(struct node *nodePtr);

//Prints the linked list
void print_eventList(struct node *eventList);

//creates a node, initiallizes node.next to NULL
struct node *create_node(int pid, int arrival, int cpu_time);

//Runs a simulation of a FCFS scheduling queue
struct node *fcfs(struct node *eventList);

//Inserts a node into the queue based on arrival time
void insert_node_queue(struct node **head, struct node **tail, struct node *newNode);

//Runs a simulation of a round robin scheduling queue
struct node *round_robin(struct node *eventList, int time_slice);

//prints a completed simulation to file
int print_to_file(struct node *simList, char* fileName);

//returns the mean of the response times
double mean(struct node *data);

//Gets the variance of the response time
double variance(struct node *data);
	
	