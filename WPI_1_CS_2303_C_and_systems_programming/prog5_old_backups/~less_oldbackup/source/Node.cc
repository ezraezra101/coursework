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

//In file scope helper function to catch exceptions & determine whether the while loop in packetActions should continue
//Written by Ezra Davis
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
}//end headValLessThanEqTime


//Removes appropriate packets from the eventList, and performs their actions
//Written by Ezra Davis and Artian Kica
void Node::packetActions(int time)
{	
	while(headValLessThanEqTime(eventList, time))
	{
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



}
