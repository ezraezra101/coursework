#include "header.h"

bool Node::usingPriorityQueue = USING_PRIORITY_QUEUE;
Queue<Node*> Node::nodes = * new Queue<Node*>();
LinkedList<Packet> Node::eventList = * new LinkedList<Packet>();
Field Node::field = * new Field(0);
int Node::totalEventCount = 0;

//Constructor
//Written by Artian Kica
Node::Node()
{
	if(DEBUG_MESG) std::cout << "Creating a node" << std::endl;
	this->id = nodes.length() + 1;
	nodes.enqueue(this);
}

//Constructor
//Written by Artian Kica
Node::Node(int id)
{
	this->id = id;
}

//Helper for nodeStep's traversing
//Written by Ezra Davis
int getNodeToAct(Node **n, int time)
{
	(*n)->nodeActions(time);
	return time;
}


//runs nodeActions on every node
//Written by Ezra Davis
void Node::nodeStep(int time)
{
	nodes.traverse(getNodeToAct, time);
}

//Calls moveActions on a Node
//Written by Artian Kica
int moveNodes(Node **n, int dummy=0)
{
	(*n)->moveActions();
	return dummy;
}

//Calls moveActions on a Node
//Written by Artian Kica
void Node::moveStep()
{
	if(VERBOSE)
	{
		std::cout << "Moving Nodes to new locations" << std::endl;
	}
	
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
//Written by Ezra Davis
void Node::packetActions(int time)
{	
	if(DEBUG_MESG) std::cout << "Entered packetActions.\t Eventlist has length " << eventList.length() << std::endl;
	while(headValLessThanEqTime(eventList, time))
	{
		if(DEBUG_MESG) std::cout << "Popping packet off eventList" <<std::endl;
		eventList.pop().goToNextNode();
	}//end while
	
}//end function packetActions


//Helper function for allDone (is a visitor function)
//Written by Ezra Davis
bool mergesIsDones(Node **n, bool pastVal)
{
	if(DEBUG_MESG) std::cout << "Running isDone on a node" <<std::endl;
	return (*n)->isDone() and pastVal;
}


//Checks to see if everything is done
//Written by Ezra Davis
bool Node::allDone()
{

	if(!eventList.isEmpty())
		return false;

	return nodes.traverse(mergesIsDones, true);
}//end function allDone


///////////////////Printing section///////////////////////////

//Written by Ezra Davis
//Sums a list together
long double sumList(double* num, long double sumSoFar)
{
	return sumSoFar + *num;
}

//Written by Ezra Davis
//Returns the variance of a queue of doubles
long double varianceOfList(Queue<double> list)
{
	//std::cout << "Taking in a list of length: " << list.length() << std::endl;
	
	
	long double sum = list.traverse(sumList, (long double) 0);
	long double count = list.length();
	
	//std::cout << "Sum: " << sum << std::endl;
	
	long double average = sum / count;
	
	long double varianceTimesCount = 0;
	
	while(!list.isEmpty())
	{
		long double num = list.dequeue();
		
		long double difference = num - average;
		
		varianceTimesCount += difference * difference;
	}
	
	return varianceTimesCount / (long double) count;
} //gets the variance of a queue of doubles


//Sadly written by Ezra Davis
//Returns the size and count of each type of packet: small medium and large
Queue<double> getSumPktTimesAndCountsBySize(Node **n, Queue<double> a)
{
//Returns a queue of the form:
//Sum of response times of of: small, medium, and large packets,
//then: the count of: small, medium, and large packets.

	if(a.length() != 6)
	{
		std::cout << "a is length: " << a.length() << std::endl;
		throw std::runtime_error("a isn't length 6");
	}
	
	Queue<double> toAdd = (*n)->averageResponseTimeByPacketSize();
	
	
	Queue<double> sums;
	//Sums them together
	for(int i = 0; i<6; i++)
	{
		//std::cout << a.headVal() << " + " << toAdd.headVal() << std::endl;
		//std::cout << "A's length is " << a.length() << std::endl;
		long double aSum = a.dequeue() + toAdd.dequeue();
		sums.enqueue( aSum );
	}

	return sums;
}

//Written by Ezra Davis
//Returns the average response times for the different packet sizes
Queue<double> getAvgRespoTimesByPktSize(Queue<Node*> nodes)
{
	//Implementation is a bit of a kludge
	
	Queue<double> base;
	base.enqueue(0); base.enqueue(0); base.enqueue(0);
	base.enqueue(0); base.enqueue(0); base.enqueue(0);
	
	Queue<double> avgs = nodes.traverse(getSumPktTimesAndCountsBySize, base);
	
	
	if(! (avgs.length() == 6))
		throw std::runtime_error("Sum isn't of length 6");
	
	
	double smallResponseTimes = avgs.dequeue();
	double midResponseTimes = avgs.dequeue();
	double largeResponseTimes = avgs.dequeue();
	double smallCount = avgs.dequeue();
	double midCount = avgs.dequeue();
	double largeCount = avgs.dequeue();
	
	//Print out to check function:
	/*
	std::cout << "\nSMR:" << smallResponseTimes << "\n";
	std::cout << "\nMMR:" << midResponseTimes << "\n";
	std::cout << "\nLMR:" << largeResponseTimes << "\n";
	std::cout << "\nSC:" << smallCount << "\n";
	std::cout << "\nMC:" << midCount << "\n";
	std::cout << "\nLC:" << largeCount << "\n";
	*/
	
	//std::cout << "Srt: " << smallResponseTimes << "Mrt: " << midResponseTimes << "SC: " << smallCount;
	
	if(!avgs.isEmpty())
		throw std::runtime_error("Worse code than expected in getAvgRespoTimesByPktSize");
	
	//Sum is now the average
	avgs.enqueue( smallResponseTimes / smallCount);
	avgs.enqueue( midResponseTimes / midCount);
	avgs.enqueue( largeResponseTimes / largeCount);
	
	return avgs;
}



//Helper function for printResults
//Written by Artian Kica
int printReceivers(Node **n, int sumPackets )
{
	return sumPackets + (*n)->printReceiver();
}//end function printReceivers

//Helper function for printResults
//Written by Ezra Davis
int printMules(Node **n, int metadataOrOtherVal )
{
	(*n)->printMule();
	return 0;
}//end function printMules

//Written by Ezra Davis
//Sums all the packet times of all the packets in the simulation
int sumPacketTimes(Node **n, int sumPacketTimes)
{
	return (*n)->getSumPacketDelay() + sumPacketTimes;
}

//prints Results of receivers and mules
//Written by Ezra Davis
void Node::printResults()
{
	field.printField(std::cout);
	int sumPackets = nodes.traverse(printReceivers, 0);
	
	std::cout << std::endl;
	nodes.traverse(printMules, 0);
	
	std::cout << std::endl;
	std::cout << "The total number of packets: " << sumPackets << std::endl;
	std::cout << "The total number of events: " << totalEventCount << std::endl;

	int sumPacketTime = nodes.traverse(sumPacketTimes, 0);

	std::cout << "The average delay for all packets: " << sumPacketTime / (double) sumPackets << std::endl;


	std::cout << "The variance of the mean transmission delay: " << varianceOfList(getAvgRespoTimesByPktSize(nodes)) << std::endl;
	std::cout << "\t(Mean in this case means average for all packets of a given size)" << std::endl;

}//end function printResults




//Written by Artian Kica
//Creates and initializes nodes
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


/////////////// Initializing sources functions

#include <cmath>

//Written by Ezra Davis
//Returns the Euclidean distance between two 2 dimensional points
inline double euclidianDist(double x1, double y1, double x2, double y2)
{
	return std::sqrt(   std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2)  );
} //end euclidianDist

//Written by Ezra Davis
//Returns log based 2 of x
inline double log2(double x)
{
	const static double divisor = std::log(2);

	return log(x) / divisor;
}//end log2


//Written by Ezra Davis
//The propagation time between 2 points
//Returns the ceil of the log based 2 of the euclidian distance between two points
inline int propTime(double x1, double y1, double x2, double y2)
{
	return (int) std::ceil( log2( euclidianDist(x1,y1,x2,y2) ) );
} //end propTime

//Written by Ezra Davis
//Returns the prop time between two nodes
int Node::propTime(const Node *other)
{
	return ::propTime(this->row, this->column, other->row, other->column);
}

//Written by Ezra Davis
//Initializes all sources based on inputs
//Fails quietly and terribly given invalid input
void Node::initializeSources(int sources)
{

	//Reads in lines of input for Source Nodes
	for (int i = 0; i < sources; ++i)
	{
		//Queue<Node*> packetPath = * new Queue<Node*>();
		
		int sourceID, arrival_time, packets, pkt_size, SR_size;
		Queue<Node*> SR;

		std::cin >> sourceID >>arrival_time >> packets >> pkt_size >> SR_size;
		Source* source = (Source*)nodes.getByIndex(sourceID - 1);

		for (int j = 0; j < SR_size; ++j)
		{
			int next_in_route;
			std::cin >> next_in_route;

			// std::cout <<"next_in_route - 1: " << next_in_route - 1 << std::endl;
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