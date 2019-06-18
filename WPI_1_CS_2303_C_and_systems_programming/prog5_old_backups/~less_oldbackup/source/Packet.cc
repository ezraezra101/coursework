#include "header.h"

//Packets contain information about their creation time, the next time they should be doing
//something, their creator, and their path to their destination
Packet::Packet(int creationTime, int nextActionTime, Source *creator, Queue<Node*> &path)
{
	this->creationTime = creationTime;
	this->nextActionTime = nextActionTime;
	this->creator = creator;
	this->path = path;
}

//Operator overloading for comparing packets (specifically comparing their next actionTimes
//greater than
bool Packet::operator>(const Packet& other) const
{
	return this->nextActionTime > other.nextActionTime;
}
//less than
bool Packet::operator<(const Packet& other) const
{
	return this->nextActionTime < other.nextActionTime;
}
//less than or equal to
bool Packet::operator<=(const Packet& other) const
{
	return !( *this > other);
}
//greater than or equal to
bool Packet::operator>=(const Packet& other) const
{
	return !( *this < other);
}

//getter for nextActionTime of a Packet
//Written by Ezra Davis and Artian Kica
int Packet::getNextActionTime() const
{
	return nextActionTime;
}

//setter for nextActionTime of a Packet
//Written by Ezra Davis and Artian Kica
void Packet::setNextActionTime(int setTime)
{
	nextActionTime = setTime;
}

//goToNextNode sends a packet to the next node on the list,
//dequeuing the current location from its path, thereby telling
//the packet its next destination
//Written by Ezra Davis and Artian Kica
void Packet::goToNextNode()
{
	if(DEBUG_MESG) std::cout << "Sending packet to next node on list" <<std::endl;
	path.dequeue()->acceptPacket(this);
}//end function goToNextNode

//gets responseTime of a packet by subtracting its
//creation time from its final nextActionTime
//Written by Ezra Davis and Artian Kica
int Packet::getResponseTime() const
{
	return nextActionTime - creationTime;
}

//gets the name (string) of a packet's source node creator
//Written by Ezra Davis and Artian Kica
string Packet::getCreatorName() const
{
	return creator->getName();
}