#include "header.h"

Queue<Node*> Node::nodes = * new Queue<Node*>();
LinkedList<Packet> Node::eventList = * new LinkedList<Packet>();
Field Node::field = * new Field(0);

Node::Node()
{
	if(DEBUG_MESG) std::cout << "Creating a node" << std::endl;
	this->id = nodes.length() + 1;
	nodes.enqueue(this);
}

Node::Node(int id)
{
	this->id = id;
}

//Helper for nodeStep's traversing
//Written by Ezra Davis and Artian Kica
int getNodeToAct(Node **n, int time)
{
	(*n)->nodeActions(time);
	return time;
}


//runs nodeActions on every node
//Written by Ezra Davis and Artian Kica
void Node::nodeStep(int time)
{
	nodes.traverse(getNodeToAct, time);
}


int moveNodes(Node **n, int dummy=0)
{
	(*n)->moveActions();
	return dummy;
}

void Node::moveStep()
{

	if(DEBUG_MESG) std::cout << "Traversing movement" <<std::endl;
	nodes.traverse(moveNodes, 0);
}

//In file scope helper function to catch exceptions & determine whether the while loop in packetActions should continue
//Written by Ezra Davis
inline bool headValLessThanEqTime(LinkedList<Packet> &eventList, int time)
{
	try {
		return eventList.headVal().getNextActionTime() <= time;
	}
	catch(std::underflow_error x)
	{
		if (DEBUG_MESG) std::cout << "Benign underflow error in headVal() (from Node.cc)" <<std::endl;
		return false;
	}
}//end headValLessThanEqTime


//Removes appropriate packets from the eventList, and performs their actions
//Written by Ezra Davis and Artian Kica
void Node::packetActions(int time)
{	
	while(headValLessThanEqTime(eventList, time))
	{
		if(DEBUG_MESG) std::cout << "Poping packet off eventList" <<std::endl;
		eventList.pop().goToNextNode();
	}//end while
	
}//end function packetActions


//Helper function for allDone (is a visitor function)
//Written by Ezra Davis and Artian Kica
bool mergesIsDones(Node **n, bool pastVal)
{
	if(DEBUG_MESG) std::cout << "Running isDone on a node" <<std::endl;
	return (*n)->isDone() and pastVal;
}


//Checks to see if everything is done
//Written by Artian Kica and Ezra Davis
bool Node::allDone()
{

	if(!eventList.isEmpty())
		return false;

	return nodes.traverse(mergesIsDones, true);
}//end function allDone

//Helper function for printResults
//Written by Ezra Davis and Artian Kica
int printReceivers(Node **n, int metadataOrOtherVal )
{
	(*n)->printReceiver();
	return 0;
}//end function printReceivers

//Helper function for printResults
//Written by Artian Kica and Ezra Davis
int printMules(Node **n, int metadataOrOtherVal )
{
	(*n)->printMule();
	return 0;
}//end function printMules

//prints Results of receivers and mules
//Written by Ezra Davis and Artian Kica
void Node::printResults()
{
	field.printField(std::cout);
	nodes.traverse(printReceivers, 0);
	
	nodes.traverse(printMules, 0);
}//end function printResults

void Node::initializeNodes(int sources, int receivers, int mules, int dimension)
{

	field = Field(dimension);

	for (int i = 0; i < sources; ++i)
	{
		new Source();

	}//end initialize sources for

	for (int i = 0; i < mules; ++i)
	{
		new Mule();

	}//end intialize mules for

	for (int i = 0; i < receivers; ++i)
	{
		new Receiver();

	}//end initialize receivers for

	initializeSources(sources);
}




#include <cmath>

//Returns the Euclidean distance between two 2 dimensional points
inline double euclidianDist(double x1, double y1, double x2, double y2)
{
	return std::sqrt(   std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2)  );
} //end euclidianDist

//Returns log based 2 of x
inline double log2(double x)
{
	const static double divisor = std::log(2);

	return log(x) / divisor;
}//end log2

//The propagation time between 2 points
//Returns the ceil of the log based 2 of the euclidian distance between two points
inline int propTime(double x1, double y1, double x2, double y2)
{
	return (int) std::ceil( log2( euclidianDist(x1,y1,x2,y2) ) );
} //end propTime


int Node::propTime(const Node *other)
{
	return ::propTime(this->row, this->column, other->row, other->column);
}

void Node::initializeSources(int sources)
{

	//Reads in lines of input for Source Nodes
	for (int i = 0; i < sources; ++i)
	{
		//Queue<Node*> packetPath = * new Queue<Node*>();
		
		int sourceID, arrival_time, packets, pkt_size, SR_size;
		Queue<Node*> SR;

		std::cin >> sourceID >>arrival_time >> packets >> pkt_size >> SR_size;
		Source* source = (Source*)nodes.getByIndex(sourceID);

		for (int j = 0; j < SR_size; ++j)
		{
			int next_in_route;
			std::cin >> next_in_route;

			std::cout << next_in_route - 1;
			SR.enqueue(nodes.getByIndex(next_in_route - 1));


		}//End individual packetPath loop

		//Do something with all the variables gathered...



		source->initialize(arrival_time, packets, pkt_size, SR);

		//These should also be checked for legality (no negatives...), I guess
		if(DEBUG_MESG)
		{
			std::cout << "Source ID: " << sourceID << "\nArrival time: " << arrival_time\
				<< "\nPackets: " << packets << "\nPacket Size: " << pkt_size << "\nRoute size: " << SR_size;

			std::cout << "\nThe source route:";
			for (Node* k = SR.dequeue(); !SR.isEmpty(); k = SR.dequeue() )
			{
				std::cout << " " << k->id;
			}
			std::cout <<std::endl;
		}

	}//End input lines for sources loop

	if (DEBUG_MESG)
	{
		std::cout << "Sources initialized." << std::endl;
	}
}