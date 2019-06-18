#include "header.h"

LinkedList<Node*> Node::nodes = * new LinkedList<Node*>();
LinkedList<Packet> Node::eventList = * new LinkedList<Packet>();

//Helper for beginStep
int getNodeToAct(Node **n, int time)
{
	(*n)->nodeActions(time);
	return time;
}


//runs nodeActions on every node
void Node::beginStep(int time)
{
	nodes.traverse(getNodeToAct, time);
}

//Helper function to catch exceptions & determine whether the while loop in packetActions should continue
inline bool headValLessThanEqTime(LinkedList<Packet> &eventList, int time)
{
	try {
		return eventList.headVal().getNextActionTime() <= time;
	}
	catch(std::underflow_error* x)
	{
		if (DEBUG_MESG) std::cout << "Benign underflow error in headVal() (from Node.cc)" <<std::endl;
		return false;
	}
}


//Removes appropriate packets from the eventList, and performs their actions
void Node::packetActions(int time)
{	
	while(headValLessThanEqTime(eventList, time))
	{
		eventList.pop().goToNextNode();
	}
	
}


//Helper function for allDone (is a visitor function)
bool mergesIsDones(Node **n, bool pastVal)
{
	if(DEBUG_MESG) std::cout << "Running isDone on a node" <<std::endl;
	return (*n)->isDone() and pastVal;
}


//Checks to see if everything is done
bool Node::allDone()
{

	if(!eventList.isEmpty())
		return false;

	return nodes.traverse(mergesIsDones, true);
}

int printReceivers(Node **n, int metadataOrOtherVal )
{
	(*n)->printReceiver();
	return 0;
}

int printMules(Node **n, int metadataOrOtherVal )
{
	(*n)->printMule();
	return 0;
}

void Node::printResults()
{
	nodes.traverse(printReceivers, 0);
	
	nodes.traverse(printMules, 0);
}
