#include <stdio.h>
#include <stdlib.h>

struct node {
	struct node *next;
	int process_id, arrival_time, cpu_time;
};



//creates a node
struct node * create_node(int process_id, int arrival_time, int cpu_time){
	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	printf("pid: %d time: %d cpu: %d\n", process_id, arrival_time, cpu_time);
	newNode->process_id = process_id;
	newNode->arrival_time = arrival_time;
	newNode->cpu_time = cpu_time;
	newNode->next = NULL;
	return newNode;
}//end create_node

//Adds an event sorted by arrival time
void add_event(struct node **eventList, struct node *newNode)
{
	if((*eventList)->next == NULL)
	{
		//if eventList is the end of the list, add newNode to the end
		(*eventList)->next = newNode;
	}
	else if((*eventList)->next->arrival_time >= newNode->arrival_time)
	{
		newNode->next = (*eventList)->next;
		(*eventList)->next = newNode;
	}
	else
	{
		add_event(&((*eventList)->next), newNode);
	}//end conditional
}//end add_event

//pops the first element of eventList
struct node get_nextevent(struct node *eventList)
{
	struct node *popped = eventList->next;
	
	eventList->next = popped->next;
	
	return *popped;
}//end nextevent

void print_eventList(struct node *eventList)
{
	eventList = eventList->next; //Going past dummy first value
	printf("Pid\ttime\tcpu\n");
	while(eventList != NULL)
	{
		printf("%d\t%d\t%d\n",
			eventList->process_id, eventList->arrival_time, eventList->cpu_time);
			
		eventList = eventList->next;
		
	}
}//end print_eventlist


int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("Incorrect number of arguments.\nlab3 epects a single integer argument.\n");
		return 1;
	}//end if
	
	int sources = atoi(argv[1]); //# of processes
	
	struct node *eventList = create_node(0,0,0);//The event list (started with a dummy value)
	
	int i; //Counter
	
	int process_id, arrival_time, cpu_time; //Values for new task/process
	struct node *newNodePtr;
	
	for(i=0; i<sources; i++)
	{
		//Takes input here
		scanf("%d %d %d", &process_id, &arrival_time, &cpu_time);
		newNodePtr = create_node(process_id, arrival_time, cpu_time);
		add_event(&eventList, newNodePtr);
	}
	
	print_eventList(eventList);
	
	get_nextevent(eventList);
	print_eventList(eventList);
	get_nextevent(eventList);
	print_eventList(eventList);
}//end main