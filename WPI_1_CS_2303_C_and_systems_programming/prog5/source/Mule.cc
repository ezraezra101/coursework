#include "header.h"

//Written by Artian Kica
//Mules are nodes that are used as stepping stones for packets
//to reach their final destination. They receive and send packets.
Mule::Mule() : Node()
{
	this->row = rand() % Node::field.getDimension();
	this->column = rand() % Node::field.getDimension();

	while(!field.setNode(this->row, this->column, this))
	{
		this->row = rand() % Node::field.getDimension();
		this->column = rand() % Node::field.getDimension();		
	}

	this->direction = static_cast<Direction>((this->id) % 4);

	this->busyUntil = 0; //Initialized to what is hopefully the start time of the simulation
	this->maxQueueSize = 0;

}

//Written by Ezra Davis
//acceptPacket enqueues a packet to the Mule's packet queue
//and checks to see if it has surpassed its maxQueueSize so far
void Mule::acceptPacket(Packet *p)
{
	if(VERBOSE)
	{
		std::cout << "Packet from " << p->getCreatorName() << " arrived at node " << id << std::endl;
		totalEventCount++;
	}
	
	
	if (Node::usingPriorityQueue)
	{
		if (p->getTransmissionTime() == 1)
		{
			queue.enqueue(*p);
		}
		else if (p->getTransmissionTime() == 2)
		{
			queueMid.enqueue(*p);
		}
		else if (p->getTransmissionTime() == 3)
		{
			queueLarge.enqueue(*p);
		}
		else
			std::cout << "Incorrect Packet Size" << std::endl;
	}
	else
	{
		queue.enqueue(*p);
		
		if (queue.length() > maxQueueSize)
		{
			maxQueueSize = queue.length();
		}//end if
	}
	
	if(DEBUG_MESG) std::cout << "accepting packet at time " << p->getNextActionTime()<<std::endl;
}//end function acceptPacket

//nodeActions for mules involves dequeuing a packet off the mule and 
//inserting it into the event list to be sent to the next node.
//Written by Artian Kica
void Mule::nodeActions(int time)
{
	if(Node::usingPriorityQueue)
	{
		if(DEBUG_MESG) std::cout << "Mule sending along packet" << std::endl;
		
		if (busyUntil > time)
		{
			
		}
		else if (!queue.isEmpty())
		{
			Packet unqueued = queue.dequeue();
			
			unqueued.insertIntoList(eventList, time);
			
			busyUntil = time + unqueued.getTransmissionTime();
		}
		else if (!queueMid.isEmpty())
		{
			Packet unqueued = queueMid.dequeue();
			
			unqueued.insertIntoList(eventList, time);
			
			busyUntil = time + unqueued.getTransmissionTime();
		}//end mid else if
		else if (!queueLarge.isEmpty())
		{
			Packet unqueued = queueLarge.dequeue();
			
			unqueued.insertIntoList(eventList, time);
			
			busyUntil = time + unqueued.getTransmissionTime();
		}//end large else if
		else //all queues empty
		{
			//blank
		}
		
	}//end usingPriorityQueue if
	else //single queue
	{
		if(!queue.isEmpty() && busyUntil <= time) //while the queue is not empty and the node is not busy
		{
			
			if(VERBOSE && busyUntil == time)
			{
				std::cout << "Mule node " << id  << " finished transmitting packet at time " << time << std::endl;
				totalEventCount++;
			}
			
			if(DEBUG_MESG) std::cout << "Mule sending along packet" << std::endl;
			
			Packet unqueued = queue.dequeue();
			
			unqueued.insertIntoList(eventList, time);
			
			busyUntil = time + unqueued.getTransmissionTime();
		}//end while
	}
}//end function nodeActions

//Written by Ezra Davis
//Moves a mule
void Mule::moveActions()
{
	if(DEBUG_MESG) std::cout << id << " Is at " << row << "x" << column << " Facing: " << direction << std::endl;

	field.moveNode(row, column, direction);

}

//isDone checks if a mule's queue is empty. If it is, the mule is done.
//Written by Artian Kica
bool Mule::isDone()
{
	if(DEBUG_MESG) std::cout << id << "'s length: "<< queue.length() <<"\n";

	return this->queue.isEmpty();
}

//printMule prints out the maximum queue size of the mule
//Written by Ezra Davis
void Mule::printMule()
{
	if (!Node::usingPriorityQueue)
	{
		std::cout << "Maximum Queue Size of Mule Node " << this->id << ": " << maxQueueSize << std::endl;
	}
}//end function printMule



