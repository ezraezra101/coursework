#include "header.h"

Receiver::Receiver()
{
	this->queue = *new Queue<Packet>();

	this->metaData = Queue<receiverMetaData>();
	
	nodes.insert(this);
}

void Receiver::acceptPacket(Packet *p)
{

	//Collect some metaData

	Queue<receiverMetaData> temp = Queue<receiverMetaData>();

	receiverMetaData * psMetaData = NULL;

	if(metaData.isEmpty())
	{
		//Make a new struct
		psMetaData = new receiverMetaData();
		psMetaData->source = p->getCreatorName();
	}
	else //If metaData isn't empty
	{
		receiverMetaData toAdd = metaData.dequeue();

		//Find a match for p's creator's name
		while(toAdd.source != p->getCreatorName() && !metaData.isEmpty())
		{
			temp.enqueue(toAdd);

			toAdd = metaData.dequeue();
		}

		//If a match was found
		if(toAdd.source == p->getCreatorName() )
		{
			psMetaData = new receiverMetaData(toAdd);
		}
		else //A match wasn't found
		{
			temp.enqueue(toAdd);

			psMetaData = new receiverMetaData();
			psMetaData->source = p->getCreatorName();
		}

		//Restoring metaData queue to 'original' state (It gets reordered)
		while(!temp.isEmpty())
		{
			metaData.enqueue( temp.dequeue());
		}
	}//End finding p's MetaData

	//Update the metaData
	psMetaData->count++;
	psMetaData->num += p->getResponseTime();

	//Put it back on the queue
	metaData.enqueue(*psMetaData);


	//Then put the packet on the queue

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

std::ostream *printPacketResponseTime(Packet* p, std::ostream *stream)
{
	*stream << "Packet Response time: " << p->getResponseTime();
	*stream  << "\tIts source: " << p->getCreatorName() << std::endl ;
	return stream;
}//end function printPacketResponseTime

std::ostream *printMetaData(receiverMetaData *m, std::ostream *stream)
{
	*stream << "Average response time from source " << m->source << ": " << m->num / (double) m->count << std::endl;

	return stream;
}

void Receiver::printReceiver()
{
	std::cout << "List of packet response times and sources:" << std::endl;

	queue.traverse(printPacketResponseTime, &std::cout);


	std::cout << std::endl;

	metaData.traverse(printMetaData, &std::cout);
}