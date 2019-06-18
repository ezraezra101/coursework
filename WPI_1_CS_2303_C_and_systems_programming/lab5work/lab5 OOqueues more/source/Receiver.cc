#include "header.h"

Receiver::Receiver()
{
	this->receivedPacketsBySource = MetaList<Packet,string>();
	
	nodes.insert(this);
}


void Receiver::acceptPacket(Packet *p)
{
	this->receivedPacketsBySource.insert(*p, (*p).getCreatorName());
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
	return this->receivedPacketsBySource.metaLength();
}





int sumResponseTimeFromPacket(Packet * p, int sum)
{
	return sum + p->getResponseTime();
}
/*
int sumAndPrintAvgResponseTimeFromSource(MetaList<Packet, string> *list, int sum)
{
	int sumThis = list->traverse(sumResponseTimeFromPacket, 0)
		
	std::cout << list->getName() << " has an average response time of " << sum / (double) list->length();
	return list->traverse(sumResponseTimeFromPacket, sum + sumThis);
}

*/


int printPacketResponseTime(Packet* p, int i)
{
	std::cout << "Packet response time from source "\
		<< p->getCreatorName()<< ": " << p->getResponseTime() << std::endl;
	return i;
}//end function printPacketResponseTime

void Receiver::printReceiver()
{
	receivedPacketsBySource.metaTraverse(printPacketResponseTime, 0);
	
	//Prints average response time by source
	//this->receivedPacketsBySource.traverse(sumAndPrintAvgResponseTimeFromSource, 0);
}