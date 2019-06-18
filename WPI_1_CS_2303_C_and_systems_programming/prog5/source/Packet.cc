#include "header.h"

//Written by Artian Kica
//Packets contain information about their creation time, the next time they should be doing
//something, their creator, and their path to their destination
Packet::Packet(int creationTime, int nextActionTime, int packetSize, Source *creator, Queue<Node*> &path)
{
	this->creationTime = creationTime;
	this->nextActionTime = nextActionTime;
	this->size = packetSize;
	this->creator = creator;
	this->path = path;
}

//Operator overloading for comparing packets (specifically comparing their next actionTimes
//Written by Ezra Davis
//greater than
bool Packet::operator>(const Packet& other) const
{
	return this->nextActionTime > other.nextActionTime;
}
//Written by Ezra Davis
//less than
bool Packet::operator<(const Packet& other) const
{
	return this->nextActionTime < other.nextActionTime;
}
//Written by Ezra Davis
//less than or equal to
bool Packet::operator<=(const Packet& other) const
{
	return !( *this > other);
}

//Written by Ezra Davis
//greater than or equal to
bool Packet::operator>=(const Packet& other) const
{
	return !( *this < other);
}

//getter for nextActionTime of a Packet
//Written by Artian Kica
int Packet::getNextActionTime() const
{
	return nextActionTime;
}

//setter for nextActionTime of a Packet
//Written by Artian Kica
void Packet::setNextActionTime(int setTime)
{
	nextActionTime = setTime;
}

//goToNextNode sends a packet to the next node on the list,
//dequeuing the current location from its path, thereby telling
//the packet its next destination
//Written by Ezra Davis
void Packet::goToNextNode()
{
	if(DEBUG_MESG) std::cout << "Packet arriving at node " << path.headVal()->getId() <<std::endl;
	path.headVal()->acceptPacket(this);
}//end function goToNextNode

//gets responseTime of a packet by subtracting its
//creation time from its final nextActionTime
//Written by Artian Kica
int Packet::getResponseTime() const
{
	return nextActionTime - creationTime;
}

//gets the name (string) of a packet's source node creator
//Written by Ezra Davis
int Packet::getCreatorName() const
{
	return creator->getId();
}

//Written by Ezra Davis
//The inserts the packet into a list at the appropriate time
void Packet::insertIntoList(LinkedList<Packet> &list, int time)
{
	if (DEBUG_MESG) std::cout << "Inserting packet into eventList " <<std::endl << "Packet is going from " << path.headVal()->getId() << std::endl;
	try
	{
		nextActionTime = time + size + path.dequeue()->propTime(path.headVal());	
	}
	catch (std::underflow_error x)
	{
		std::cout << "Oh, dear, the packetPath is too short or didn't end in a receiver Node." << std::endl;
		throw x;
	}

	list.insert(*this);
}