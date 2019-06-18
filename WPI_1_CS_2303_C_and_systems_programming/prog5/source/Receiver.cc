#include "header.h"

//Written by Artian Kica
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
//Written by Ezra Davis
void Receiver::acceptPacket(Packet *p)
{

	if(VERBOSE)
	{
		std::cout << "Packet from " << p->getCreatorName() << " arrived at node " << id << std::endl;
		totalEventCount++;
	}

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
//"Written" by Artian Kica
void Receiver::nodeActions(int time)
{
	//Empty method
}

//isDone for receivers is always true because receivers never have anything to do themselves.
//Receivers are always done as long as all the other nodes are done.
//"Written" by Artian Kica
bool Receiver::isDone()
{
	return true;
}

//queueLength returns the length of the current Receiver.
//Written by Artian Kica
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
	*stream << "Total number of packets from source " << m->source << ": " << m->count << std::endl;
	*stream << "Average response time from source " << m->source << ": " << m->num / (double) m->count << std::endl;
	
	return stream;
}//end function printMetaData

//Written by Ezra Davis
//Computes the variance of response times of the packets in a queue
double computeVariance(double average, Queue<Packet> queue, int source, int count)
{	
	double variance = 0;
	
	Packet p;
	
	while(!queue.isEmpty())
	{
		p = queue.dequeue();
		
		if(p.getCreatorName() == source)
		{
			double difference = p.getResponseTime() - average;
			variance += difference * difference / count;
		}
	}//End for loop: packets in queue
	return variance;
}

//Written by Artian Kica
//Computes and prints the variances of the packet response times for all sources a receiver has received from
void computeVariances(Queue<Packet> queue, Queue<receiverMetaData> metaData)
{
	if(!metaData.isEmpty())
	{
		receiverMetaData i;
		
		while(!metaData.isEmpty())
		{
			 i = metaData.dequeue();
			
			double average = i.num / (double) i.count;
					
			std::cout << "Variance of packet delay from source " << i.source << ": " << computeVariance(average, queue, i.source, i.count) << std::endl;
		}//End for loop: metadata
	}//endif
}//end compute variance

///////////////////////////////////////////////

//Written by Ezra Davis
//Sums the response time of all the packets of a given size (in this receiver's queue)
double sumByPktSize(int size, Queue<Packet> queue)
{
	//int count = 0;
	int sumRespoTime = 0;
	
	while(!queue.isEmpty())
	{
		Packet p = queue.dequeue();
		
		if(p.getTransmissionTime() == size)
		{
			//count++;
			sumRespoTime += p.getResponseTime();
		}
	}
	return sumRespoTime;
}

//Written by Ezra Davis
//Counts packets of a given size in this receiver's queue
double countByPktSize(int size, Queue<Packet> queue)
{
	int count = 0;
	//int sumRespoTime = 0;
	
	while(!queue.isEmpty())
	{
		Packet p = queue.dequeue();
		
		if(p.getTransmissionTime() == size)
		{
			count++;
			//sumRespoTime += p.getResponseTime();
		}
	}
	return count;
}

//Sadly written by Ezra Davis (It's a kludge)
//Returns the response times and number of packets of each type (size)
Queue<double> Receiver::averageResponseTimeByPacketSize()
{
//Returns a queue of the form:
//Sum of response times of of: small, medium, and large packets,
//then: the count of: small, medium, and large packets.
	Queue<Packet> queue(this->queue);
	
	Queue<double> x;
	
	for(int i = 1; i <= 3; i++)
	{
		double y = sumByPktSize(i, queue);		
		x.enqueue(y);
	}
	for(int i = 1; i <= 3; i++)
	{
		double y = countByPktSize(i, queue);
		x.enqueue(y);
	}

	return x;
}
///////////////////////////////////



//printReceiver prints out a list of the packet response time and sources by
//calling the above 2 functions (printPacketResponseTime and printMetaData)
//Written by Artian Kica
int Receiver::printReceiver()
{	
	std::cout << "List of packet response times and sources received by receiver " << id << ":" << std::endl;

	//call printPacketResponseTime on each packet in the receiver queue
	queue.traverse(printPacketResponseTime, &std::cout);

	std::cout << std::endl;

	//Computes the variance for each source:
	computeVariances(queue, metaData);
	std::cout << std::endl;
	//call printMetaData on each metaData in the receiver metaData queue.
	metaData.traverse(printMetaData, &std::cout);

	return queue.length();
}//end function printReceiver

//Written by Ezra Davis
int sumPacketDelay(Packet* p, int packetDelaySoFar)
{
	return p->getResponseTime() + packetDelaySoFar;
}

//Written by Ezra Davis
int Receiver::getSumPacketDelay()
{
	return queue.traverse(sumPacketDelay, 0);
}