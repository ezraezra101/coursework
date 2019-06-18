#include "header.h"


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


void Source::makePacket(int time)
{
	if(DEBUG_MESG) std::cout << "Making packet at time " << time <<std::endl;
	//Packet will reach front of eventList when it arrives at the next Node
	eventList.insert(Packet(time, time + transTime + propTime, this, packetPath));
}

void Source::acceptPacket(Packet *p)
{
	throw new std::runtime_error("Tried to put a packet in a source node");
}
void Source::nodeActions(int time)
{
	if(DEBUG_MESG) std::cout << "Time in Source::nodeActions: " << time << std::endl;
	if((time >= busyUntil) && (packetsToSend > 0))
	{
		makePacket(time);
		packetsToSend--;
		busyUntil = time + transTime;
	}
}


bool Source::isDone()
{
	if(packetsToSend > 0)
	{
		if(DEBUG_MESG) std::cout << "Source isn't done" <<std::endl;
		return false;
	}
	else
	{
		if (DEBUG_MESG) std::cout << "Source is done" <<std::endl;
		return true;
	}
}

string Source::getName() const
{
	return name;
}
