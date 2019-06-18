#include "header.h"

Packet::Packet(int creationTime, int nextActionTime, Source *creator, LinkedList<Node*> &path)
{
	this->creationTime = creationTime;
	this->nextActionTime = nextActionTime;
	this->creator = creator;
	this->path = path;
}

bool Packet::operator>(const Packet& other) const
{
	return this->nextActionTime > other.nextActionTime;
}
bool Packet::operator<(const Packet& other) const
{
	return this->nextActionTime < other.nextActionTime;
}

bool Packet::operator<=(const Packet& other) const
{
	return !( *this > other);
}

bool Packet::operator>=(const Packet& other) const
{
	return !( *this < other);
}

int Packet::getNextActionTime() const
{
	return nextActionTime;
}

void Packet::setNextActionTime(int setTime)
{
	nextActionTime = setTime;
}

void Packet::goToNextNode()
{
	if(DEBUG_MESG) std::cout << "Sending packet to next node on list" <<std::endl;
	path.pop()->acceptPacket(this);
}

int Packet::getResponseTime() const
{
	return nextActionTime - creationTime;
}

string Packet::getCreatorName() const
{
	return creator->getName();
}