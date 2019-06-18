#include "header.h"


Source::Source() : Node()
{
	this->row = rand() % Node::field.getDimension();
	this->column = -1;

	while(!field.setNode(this->row, this->column, this))
	{
		this->row = rand() % Node::field.getDimension();	
	}
}
/*
//Source is a node that sends packets and does not receive packets.
Source::Source(int transTime, int propTime, int startTime, Queue<Node*> packetPath, string name)
{
	this->transTime = transTime;
	this->propTime = propTime;
	this->startTime = startTime;
	
	this->packetsToSend = 100;
	
	this->busyUntil = startTime;
	this->name = name;
	
	this->packetPath = packetPath;
	
	nodes.insert(this);
}
*/

//makePacket takes a time and inserts a packet with that time and the source's transmission and propagation time
//Written by Ezra Davis and Artian Kica
void Source::makePacket(int time)
{
	if(DEBUG_MESG) std::cout << "Making packet at time " << time <<std::endl;
	//Packet will reach front of eventList when it arrives at the next Node
	eventList.insert(Packet(time, time + transTime + propTime, this, packetPath));
}

//Since sources are never supposed to receive packets, this function is 
//used to throw an exception if this function is called on a source node.
//Written by Ezra Davis and Artian Kica
void Source::acceptPacket(Packet *p)
{
	throw new std::runtime_error("Tried to put a packet in a source node");
}//end function acceptPacket

//nodeActions will make a packet when it is not busy and still has packets to send.
//Written by Ezra Davis and Artian Kica
void Source::nodeActions(int time)
{
	if(DEBUG_MESG) std::cout << "Time in Source::nodeActions: " << time << std::endl;
	
	if((time >= busyUntil) && (packetsToSend > 0)) //if source node is not busy and still has packets to send
	{
		makePacket(time);
		packetsToSend--;
		busyUntil = time + transTime;
	}//end if
}//end function nodeActions

//isDone determines if the source is completely done sending packets, which is when packetsToSend == 0
//Written by Ezra Davis and Artian Kica
bool Source::isDone()
{
	if(packetsToSend > 0)
	{
		if(DEBUG_MESG) std::cout << "Source isn't done" <<std::endl;
		return false;
	}//end if
	else
	{
		if (DEBUG_MESG) std::cout << "Source is done" <<std::endl;
		return true;
	}//end else
}//end function isDone

//getName returns the name of a source.
//Written by Ezra Davis and Artian Kica
string Source::getName() const
{
	return name;
}//end function getName
