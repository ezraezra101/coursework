#include "header.h"


#include <cmath>

//Returns the Euclidean distance between two 2 dimensional points
double euclidianDist(double x1, double y1, double x2, double y2)
{
	return std::sqrt(   std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2)  );
} //end euclidianDist

//Returns log based 2 of x
double log2(double x)
{
	const static double divisor = std::log(2);

	return log(x) / divisor;
}//end log2

//The propagation time between 2 points
//Returns the ceil of the log based 2 of the euclidian distance between two points
int propTime(double x1, double y1, double x2, double y2)
{
	return (int) std::ceil( log2( euclidianDist(x1,y1,x2,y2) ) );
} //end propTime



//Runs a packet transmission simulation
//Written by Artian Kica and Ezra Davis
int main(int argc, char* argv[])
{

	//Reading in input:
	if(argc != 5)
	{
		std::cout << "Usage:\n" << argv[0] << " [sources] [receivers] [mules] [dimension]" <<std::endl;
		return 1;
	}
	int sources = std::atoi(argv[1]);
	int receivers = std::atoi(argv[2]);
	int mules = std::atoi(argv[3]);
	int dimension = std::atoi(argv[4]);

	if(DEBUG_MESG) std::cout << "Inputs received:\n" << sources << " " << receivers << " " << mules << " " << dimension << std:: endl;

	//Some input validation
	if(mules % 4 != 0  ||  mules > dimension * dimension)
	{
		std::cout << "Illegal number of Mules." << std::endl;
		return 1;

	}
	if(receivers > dimension)
	{
		std::cout << "There are more Receivers than there is space to place them" << std::endl;
		return 1;
	}
	if(sources > dimension)
	{
		std::cout << "There are more Sources than there is space to place them" << std::endl;
		return 1;
	}
	if(receivers < 0 || mules < 0 || dimension < 0 || sources < 0)
	{
		std::cout << "Negative input values are illegal" << std::endl;
		return 1;
	}
	//End input validation


	std::cout << "Receivers are numbered 1 to " << sources\
	 << "\nMules are " << sources + 1 << " to " << sources + mules\
	 << "\n And Receivers are " << sources + mules + 1 << " to " << sources + mules + receivers << std::endl;

	//Reads in lines of input for Source Nodes
	//This part will be heavily changed & probably moved to the Node class or Source class:
	for (int i = 0; i < sources; ++i)
	{
		Queue<int> packetPath = * new Queue<int>();
		
		int sourceID, arrival_time, packets, pkt_size, SR_size;
		Queue<int> SR;

		std::cin >> sourceID >>arrival_time >> packets >> pkt_size >> SR_size;

		for (int j = 0; j < SR_size; ++j)
		{
			int next_in_route;
			std::cin >> next_in_route;
			SR.enqueue(next_in_route);


		}//End individual packetPath loop

		//Do something with all the variables gathered...

		//####

		//These should also be checked for legality (no negatives...), I guess

		//At the moment, just printing them out:
		std::cout << "Source ID: " << sourceID << "\nArrival time: " << arrival_time\
			<< "\nPackets: " << packets << "\nPacket Size: " << pkt_size << "\nRoute size: " << SR_size;

		std::cout << "\nThe source route:";
		for (int k = SR.dequeue(); !SR.isEmpty(); k = SR.dequeue() )
		{
			std::cout << " " << k;
		}
		std::cout <<std::endl;

	}//End input lines for sources loop






	//Creating Nodes:
	Mule *m1 = new  Mule(2, 3, "M1");
	Mule *m2 = new Mule(2, 5, "M2");
	Receiver *r1 = new Receiver();
	
	//Creating route for Packets to take
	Queue<Node*> packetPath = * new Queue<Node*>();
	
	packetPath.enqueue((Node*) m1);
	packetPath.enqueue((Node*) m2);
	packetPath.enqueue((Node*) r1);
	

	if(DEBUG_MESG) std::cout << "Now creating source nodes" << std::endl;
	
		//Constructor arguments: Source(transTime, propTime, startTime, packetPath, name)
	Source s1(1, 2, 0, packetPath, "S1");
	Source s2(1, 2, 3, packetPath, "S2");
	

	//Running simulation:
	int time = 0;
	while (!Node::allDone())
	{
		if(DEBUG_MESG) std::cout << "Time: " << time << std::endl;
		
		Node::nodeStep(time);
		Node::packetActions(time); 
		
		//Keeps packets from waiting one tick in the queue before sending if the queue is empty
		//(Useful for Mule nodes only)
		Node::nodeStep(time); 
		
		time++;
	}
	
	//Reporting on final state:
	if(DEBUG_MESG) std::cout << "the eventlist is " << Node::eventList.length() << " packets long\n";
	if(DEBUG_MESG) std::cout << "The receiver's queue is " << r1->queueLength()<< " packets long" << std::endl;
	
	std::cout << "\nSimulation results:\n" << std::endl;

	Node::printResults();

	std::cout << "Time of the end of simulation in sims: " << time << std::endl;

	std::cout << "All units of time are in 'sims' (100 millisecond increments)" << std::endl;

	std::cout << std::endl;
}
