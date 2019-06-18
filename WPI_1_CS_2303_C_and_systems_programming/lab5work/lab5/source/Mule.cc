#include "header.h"

Mule::Mule(int transTime, int propTime)
{
	this->transTime = transTime;
	this->propTime = propTime;
	this->busyUntil = 0; //Initialized to what is hopefully the start time of the simulation
	this->maxQueueSize = 0;
	
	nodes.insert(this);
}


void Mule::acceptPacket(Packet *p)
{
	queue.enqueue(*p);
	
	if (queue.length() > maxQueueSize)
	{
		maxQueueSize = queue.length();
	}//end if
	
	if(DEBUG_MESG) std::cout << "accepting packet at time " << p->getNextActionTime()<<std::endl;
}

void Mule::nodeActions(int time)
{
	while(!queue.isEmpty() && busyUntil <= time)
	{
		if(DEBUG_MESG) std::cout << "Mule sending along packet" << std::endl;
		
		Packet unqueued = queue.dequeue();
		unqueued.setNextActionTime(time + propTime + transTime);
		eventList.insert(unqueued);
		
		busyUntil = time + transTime;
	}
}

bool Mule::isDone()
{
	return this->queue.isEmpty();
}

void Mule::printMule()
{
	std::cout << "Maximum Queue Size of Mule Node: " << maxQueueSize << std::endl;

}//end function printMule



