#include "header.h"

//Constructor for source - note: doesn't initialize everything properly
//Written by Ezra Davis
Source::Source() : Node()
{
	this->row = rand() % Node::field.getDimension();
	this->column = -1;

	while(!field.setNode(this->row, this->column, this))
	{
		this->row = rand() % Node::field.getDimension();	
	}

	if (DEBUG_MESG) std::cout << "Placing source " << id << " at row " << this->row << std::endl;
}

//Written by Artian Kica
//Initializes the remaining data members in Source
void Source::initialize(int arrival_time, int packets, int pkt_size, Queue<Node*> sourceRoute)
{
	this->busyUntil = arrival_time;
	this->arrivalTime = arrival_time;
	this->packetsToSend = packets;
	this->packetSize = pkt_size;
	this->packetPath = sourceRoute;
}


//makePacket takes a time and inserts a packet with that time and the source's transmission and propagation time
//Written by Artian Kica
void Source::makePacket(int time)
{
	if(DEBUG_MESG) std::cout << "Making packet at time " << time <<std::endl;
	//Packet will reach front of eventList when it arrives at the next Node
	Packet packet = Packet(time, -5000, packetSize, this, packetPath); //-5000 is changed in the the insertIntoList function

	packet.insertIntoList(eventList, time);
}

//Since sources are never supposed to receive packets, this function is 
//used to throw an exception if this function is called on a source node.
//Written by Ezra Davis
void Source::acceptPacket(Packet *p)
{
	throw new std::runtime_error("Tried to put a packet in a source node");
}//end function acceptPacket

//nodeActions will make a packet when it is not busy and still has packets to send.
//Written by Artian Kica
void Source::nodeActions(int time)
{
	if(DEBUG_MESG) std::cout << "Time in Source::nodeActions: " << time << std::endl;
	
	if((time >= busyUntil) && (packetsToSend > 0)) //if source node is not busy and still has packets to send
	{
		
		if(arrivalTime == time)
		{
			std::cout << "Source node " << id  << " has started sending packets at time " << time << std::endl;			
		}
		else if(VERBOSE && busyUntil == time)
		{
			std::cout << "Source node " << id  << " has finished transmitting a packet at time " << time << std::endl;
			totalEventCount++;
		}
		
		makePacket(time);
		packetsToSend--;
		busyUntil = time + packetSize;
	}//end if
}//end function nodeActions

//isDone determines if the source is completely done sending packets, which is when packetsToSend == 0
//Written by Ezra Davis
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
