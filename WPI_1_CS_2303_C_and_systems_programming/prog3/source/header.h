//Ezra Davis
//Program 3 CS 2303
//February 2014

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
//A unit of time is 100 milliseconds

//Debug option
#define VERBOSE false

//Output folder
#define OUTLOCATION "SimulationResults"

struct node {
	struct node *next; //The next in the queue/linked list
	int pid; //process id
	int arrival; //arrival time
	int cpu_time; //CPU time required to finish the task
	int original_cpu_time; //CPU time required, when initialized, to finish the task
	
	char type; //'A' for arrivals/active, 'C' for completed
	int completed; //Completed time
};

//Reads input and creates eventList
struct node *create_eventList(int samples);

//creates a node, initiallizes node.next to NULL
struct node *create_node(int pid, int arrival, int cpu_time);

// Pops the front of the queue
struct node *dequeue(struct node **head);

//Adds a node to the back of the queue
void enqueue(struct node **head, struct node **tail, struct node *newNode);

//Runs a simulation of a FCFS scheduling queue
struct node *fcfs(struct node *eventList);

//Frees the linked list
void free_list(struct node *head);

//Inserts a node into the queue based on arrival time
void insert_node_queue(struct node **head, struct node **tail, struct node *newNode);

//Inserts a node into the list sorted by arrival time, if there are ties, its sorted by process id
void insert_node(struct node **list, struct node *newNode);

//Is the queue empty?
int isEmpty(struct node *head);

//copies a linked list
struct node *listcpy(struct node *list);

//Runs the simulations
int main(int argc, char** argv);

//returns the mean of the response times
double mean(struct node *data);

//prints the metadata of a simulation to a file
int options_to_file(struct node *simList, char* programName, int sources, int timeslice, char* fileName);

//Runs a simulation of a preemptive round robin scheduling queue
struct node *preemptive_rr(struct node *eventList, int time_slice);

//Prints the linked list
void print_eventList(struct node *eventList);

//Prints the values of a single node
void print_node(struct node *nodePtr);

//prints a completed simulation to file
int print_to_file(struct node *simList, char* fileName);

//Runs a simulation of a round robin scheduling queue
struct node *round_robin(struct node *eventList, int time_slice);

//Gets the variance of the response time
double variance(struct node *data);

