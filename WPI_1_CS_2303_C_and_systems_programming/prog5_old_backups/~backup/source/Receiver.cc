#include "header.h"

//Receivers are the final destination of packets. They only receive packets and put them into a queue for output.
Receiver::Receiver() : Node()
{
	this->row = rand() % Node::field.getDimension();
	this->column = Node::field.getDimension();

	while(!field.setNode(this->row, this->column, this))
	{
		this->row = rand() % Node::field.getDimension();	
	}
}

//acceptPacket takes a packet and places it in the receiver queue.
//it also collects information (metaData) about the packet. In our
//case, this is simply its response time and source.
//Written by Ezra Davis and Artian Kica
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
}//end function acceptPacket

//nodeActions for receivers does nothing because Receivers do not do anything but accept packets.
//"Written" by Ezra Davis and Artian Kica
void Receiver::nodeActions(int time)
{
	//Empty method
}

//isDone for receivers is always true because receivers never have anything to do themselves.
//Receivers are always done as long as all the other nodes are done.
//"Written" by Ezra Davis and Artian Kica
bool Receiver::isDone()
{
	return true;
}

//queueLength returns the length of the current Receiver.
//Written by Ezra Davis and Artian Kica
int Receiver::queueLength() const
{
	return this->queue.length();
}//end function queueLength

//printPacketResponse time takes a packet and prints out 
//its response time and source to the given output stream.
//Written by Ezra Davis
std::ostream *printPacketResponseTime(Packet* p, std::ostream *stream)
{
	*stream << "Packet Response time: " << p->getResponseTime();
	*stream  << "\tIts source: " << p->getCreatorName() << std::endl ;
	return stream;
}//end function printPacketResponseTime

//printMetaData prints out the average response time per source
//Written by Ezra Davis
std::ostream *printMetaData(receiverMetaData *m, std::ostream *stream)
{
	*stream << "Average response time from source " << m->source << ": " << m->num / (double) m->count << std::endl;

	return stream;
}//enf function printMetaData

//printReceiver prints out a list of the packet response time and sources by
//calling the above 2 functions (printPacketResponseTime and printMetaData)
//Written by Ezra Davis and Artian Kica
void Receiver::printReceiver()
{
	std::cout << "List of packet response times and sources:" << std::endl;

	//call printPacketResponseTime on each packet in the receiver queue
	queue.traverse(printPacketResponseTime, &std::cout);

	std::cout << std::endl;

	//call printMetaData on each metaData in the receiver metaData queue.
	metaData.traverse(printMetaData, &std::cout);
}//end function printReceiver