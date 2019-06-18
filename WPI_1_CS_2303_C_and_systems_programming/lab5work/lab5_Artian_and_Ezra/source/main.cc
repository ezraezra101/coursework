#include "header.h"

//Runs a packet transmission simulation
//Written by Artian Kica and Ezra Davis
int main()
{
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
