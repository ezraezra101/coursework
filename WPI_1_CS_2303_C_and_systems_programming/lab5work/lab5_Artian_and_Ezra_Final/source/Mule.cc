#include "header.h"

//Mules are nodes that are used as stepping stones for packets
//to reach their final destination. They receive and send packets.
Mule::Mule(int transTime, int propTime, string name)
{
	this->transTime = transTime;
	this->propTime = propTime;
	this->name = name;
	this->busyUntil = 0; //Initialized to what is hopefully the start time of the simulation
	this->maxQueueSize = 0;
	
	nodes.insert(this);
}

//acceptPacket enqueues a packet to the Mule's packet queue
//and checks to see if it has surpassed its maxQueueSize so far
void Mule::acceptPacket(Packet *p)
{
	
	queue.enqueue(*p);
	
	if (queue.length() > maxQueueSize)
	{
		maxQueueSize = queue.length();
	}//end if
	
	if(DEBUG_MESG) std::cout << "accepting packet at time " << p->getNextActionTime()<<std::endl;
}//end function acceptPacket

//nodeActions for mules involves dequeuing a packet off the mule and 
//inserting it into the event list to be sent to the next node.
//Written by Ezra Davis and Artian Kica
void Mule::nodeActions(int time)
{
	while(!queue.isEmpty() && busyUntil <= time) //while the queue is not empty and the node is not busy
	{
		if(DEBUG_MESG) std::cout << "Mule sending along packet" << std::endl;
		
		Packet unqueued = queue.dequeue();
		unqueued.setNextActionTime(time + propTime + transTime);
		eventList.insert(unqueued);
		
		busyUntil = time + transTime;
	}//end while
}//end function nodeActions

//isDone checks if a mule's queue is empty. If it is, the mule is done.
//Written by Ezra Davis and Artian Kica
bool Mule::isDone()
{
	return this->queue.isEmpty();
}

//printMule prints out the maximum queue size of the mule
//Written by Ezra Davis and Artian Kica
void Mule::printMule()
{
	std::cout << "Maximum Queue Size of Mule Node " << this->name << ": " << maxQueueSize << std::endl;

}//end function printMule



