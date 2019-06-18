#include "header.h"

Receiver::Receiver()
{
	this->queue = *new Queue<Packet>();
	
	nodes.insert(this);
}


void Receiver::acceptPacket(Packet *p)
{
	this->queue.enqueue(*p);
}

void Receiver::nodeActions(int time)
{
	//Empty method
}

bool Receiver::isDone()
{
	return true;
}


int Receiver::queueLength() const
{
	return this->queue.length();
}

int printPacketResponseTime(Packet* p, int i)
{
	std::cout << "Packet response time from source "\
		<< p->getCreatorName()<< ": " << p->getResponseTime() << std::endl;
	return i;
}//end function printPacketResponseTime

void Receiver::printReceiver()
{
	queue.traverse(printPacketResponseTime, 0);
}