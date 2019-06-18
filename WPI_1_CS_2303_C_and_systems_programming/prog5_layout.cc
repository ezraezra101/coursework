#include <iostream>
#include <cstdlib>
#include <fstream>

class Node;

class Packet
{
	
public:
	
	//A constructor
	
	int getArrivalTime();
	void setArrivalTime(int);
	
	char getEventType();
	void setEventType(char);
	
	int getSize();
	
	Node* popNextDestination();
	
	bool operator>(Packet*);
	bool operator<(Packet*);
};


template<class T>
class LinkedList
{
public:
	LinkedList(T type);
	
	int length();
	
	bool end();
	
	LinkedList<T> *next();
	
	T getData();
};

//A node on the MANET map
class Node
{
	static LinkedList<Node*> nodes;
	
	static unsigned int time; //Does this make sense?
	
	static std::fstream outputLog;
	
public:
	//Ticks the entire list
	static void tick()
	{
		//Doing packet stuff
		LinkedList<Node*> *iterator;
		for(iterator  = &nodes; !iterator->end(); iterator = iterator->next())
		{
			iterator->getData()->tickActions();
		}
		
		//Moving MANET mules
		for(iterator  = &nodes; !iterator->end(); iterator = iterator->next())
		{
			iterator->getData()->moveActions();
		}
	};
	
	//If the entire simulation is done
	static bool isDone();
	
protected:
	
	//Receives a packet and places it on its eventList
	virtual void receive(Packet)=0;
	
	//If this process is done
	virtual bool amIDone()=0;
	
	
	//What happens to a node when it runs
	virtual void tickActions()=0;
	
	virtual void moveActions()=0;
	
	//Propagation Time between two nodes
	virtual int propTime(Node*); //Not sure if it should be virtual
	
};

//To the East
class Receiver : public Node
{
	//Created on a random, unique row
	
	//Records and deletes structures
};

//To the West
class Source : public Node
{
	//Created on a random, unique row
};

//Moving
class Mule : public Node
{
	//Placed on a random location and moves like smashball players (Hop boosting & wall bouncing)
	
	//Initial Mule direction is muleID % 4
	
	//Mule hopping takes place after *all* sending
};

int main(int argc, char** argv)
{
	std::cout << "Nothing implemented :-(" << std::endl;
	
	if(argc == 5)
	{
		int sources = atoi(argv[1]);
		int receivers = atoi(argv[2]);
		int mules = atoi(argv[3]);
		
		//side length of the MANET map
		int dimension = atoi(argv[4]);
		
		int i;
		for(i=0; i < sources; i++)
		{
			//Read in an input for the source nodes:
			//sourceID, arrival_time, packets, pkt_size, SR_size (length of following route), SR (route)
		}
	}
	
	
	
}