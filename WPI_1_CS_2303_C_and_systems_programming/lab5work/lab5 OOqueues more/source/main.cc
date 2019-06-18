#include "header.h"

int main()
{
	
	Mule *m1 = new  Mule(2, 3);
	Mule *m2 = new Mule(2, 5);
	Receiver *r1 = new Receiver();
	
	Queue<Node*> packetPath = * new Queue<Node*>();
	
	packetPath.enqueue((Node*) m1);
	packetPath.enqueue((Node*) m2);
	packetPath.enqueue((Node*) r1);
	
	if(DEBUG_MESG) std::cout << "Now creating source nodes" << std::endl;
	
	//Source(transTime, propTime, startTime, packetPath, name)
	Source s1(1, 2, 0, packetPath, "S1");
	Source s2(1, 2, 3, packetPath, "S2");
	
	
	int time = 0;
	while (!Node::allDone())
	{
		if(DEBUG_MESG) std::cout << "Time: " << time << std::endl;
		
		Node::beginStep(time);
		Node::packetActions(time);
		
		//If this one isn't present, packets will wait for one tick in queue before sending.
		Node::beginStep(time);
		
		time++;
	}
	
	//Reporting on final state:
	if(DEBUG_MESG) std::cout << "the eventlist is " << Node::eventList.length() << " packets long\n";
	
	std::cout << "Time: " << time << std::endl;
	std::cout << "The receiver's queue is " << r1->queueLength()<< " packets long" << std::endl;
	
	Node::printResults();
	
	
	
	
	
	
	std::cout << std::endl << std::endl << std::endl << "Beginning frivolous tests:" << std::endl << std::endl;
	
	
	LinkedList<int> list;
	
	list.insert(5);
	list.insert(7);
	
	std::cout << list.headVal()<< std::endl;
	std::cout << list.pop() << std::endl;
	std::cout << list.pop() << std::endl;
	
	//Tests the throwing of an underflow error
	try
	{
		std::cout << list.headVal();
	}
	catch(std::underflow_error* x)
	{
		std::cout << "Yay!\nWe caught an error:" << std::endl;
		std::cout << x->what() << std::endl << std::endl;
	}
	
	
	//Testing destructor
	LinkedList<std::string> *list2 = new LinkedList<std::string>();
	list2->insert("hello");
	list2->insert("world");
	
	std::cout << "Testing nonexistant destructor..." << std::endl;
	delete list2;
}
