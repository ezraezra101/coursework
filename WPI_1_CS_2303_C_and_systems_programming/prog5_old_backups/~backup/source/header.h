//Don't include if it is already included:
#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

//Controls visibility of debug messages
#define DEBUG_MESG true

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <string>
#include <cstdlib>

using std::string;


class Source;
class Node;
class Field;

enum Direction {North, East, South, West};

//A sorted LinkedList
template<class T>
class LinkedList
{
protected:
	//LLNodes represent an entry in the linkedList
	struct LLNode
	{
		T value;		//Item in linked list
		LLNode *next;	//Link to next LLNode
		
	};
	
	//The first entry in the linkedList
	LLNode *head;
public:
	//Constructor
	LinkedList();
	
	//Adds an item into the correct place in the linkedList
	void insert(const T &);
	
	//Returns the first value from the LinkedList
	T headVal() const;
	
	//Removes and returns the first value from the LinkedList
	T pop();
		
	//Is the linkedList empty?
	bool isEmpty() const;

	//Returns the element specified with zero indexing 0...length-1.
	//Throws an error if index is out of range
	//O(n) time.
	T getByIndex(int index) const;

	//Gets the length of the LinkedList (O(n) time)
	int length() const;
	
	//Calls the function on every member of the LinkedList, not dissimilar to the visitor pattern
	template<class Y>
	Y traverse(Y (*function)(T *, Y), Y);
};

//A simple queue
template<class T>
class Queue : private LinkedList<T>
{	
	typename LinkedList<T>::LLNode *tail;
public:
	//Constructor
	Queue();
	
	//Copy constructor
	Queue(const Queue &);
	
	//Adds item to back of Queue
	void enqueue(const T &);
	
	//Pops item off queue
	T dequeue();
	
	//Gets the length of the queue
	int length() const;
	
	//Is the queue empty?
	bool isEmpty() const;

	//Return the front of the queue without removing it
	T headVal() const;
	
	//Calls the function on every member of the LinkedList, not dissimilar to the visitor pattern
	template<class Y>
	Y traverse(Y (*function)(T *, Y), Y);

	T getByIndex(int index) const;
};

class Packet
{
	int size;
	int creationTime; //When the packet is created
	int nextActionTime; //When the packet arrives at the next Node
						//Also used in calculating response time
	//The creator of this packet
	Source* creator;
	
	//The remaining Nodes the packet will pass through
	Queue<Node*> path;

public:
	//Constructor
	Packet(int creationTime, int nextActionTime, int size, Source *creator, Queue<Node*> &path);
	
	Packet(){} //For housekeeping
	
	//Comparators. Uses nextActionTime for comparison purposes
	bool operator>(const Packet &) const;
	bool operator<(const Packet &) const;
	bool operator<=(const Packet &other) const;
	bool operator>=(const Packet &other) const;
	
	//Returns the nextActionTime
	int getNextActionTime() const;

	//Sets nextActionTime
	void setNextActionTime(int setTime);
	
	//Tells the next node on the path to accept this packet
	void goToNextNode();

	//Returns the response time (should only be called once the packet has completed its journey)
	int getResponseTime() const;

	//Returns the creator's name
	int getCreatorName() const;

	int getTransmissionTime() {return size;}

	void insertIntoList(LinkedList<Packet> list, int time);

};


//A node that simulates a router (Mule), source, or receiver
class Node
{

protected:

	int id;
	int row;
	int column;

	//All the nodes in the universe
	static Queue<Node*> nodes;
	static Field field;

public:

	Node();
	Node(int id);

	//Contains all the Packets in transit
	static LinkedList<Packet> eventList;
	
	//Moves the packets in transit
	static void packetActions(int time);
	
	//Accepts packets from transit
	virtual void acceptPacket(Packet *p) = 0;

	//Takes appropriate packets and puts them in transit
	static void nodeStep(int time);

	//Takes this nodes packets and puts them in transit
	virtual void nodeActions(int time) = 0;


	//Moves all the mule nodes
	static void moveStep();

	virtual void moveActions() {};

	//Are all the nodes and packets finished?
	static bool allDone();
	//Is this Node have unfinished business?
	virtual bool isDone() = 0;
	
	//Prints out simulation results
	static void printResults();
	//Prints out during the receiver printout step (after the simulation)
	virtual void printReceiver() {};
	//Prints out during the receiver printout step (after the simulation)
	virtual void printMule() {};

	static void initializeNodes(int sources, int receivers, int mules, int dimension);

	static void initializeSources(int sources);

	int getId() { return id;}

	int propTime(const Node *other);
};

//Creates and sends out packets
class Source : public Node
{
	int packetsToSend;	//How many packets must be sent
	int busyUntil;		//When the source stops being busy and can transmit another packet
	int packetSize;

	Queue<Node*> packetPath;//The path all packets leaving this node take
	
public:
	//constructor
	Source();


	void initialize(int arrival_time, int packets, int pkt_size, Queue<Node*> sourceRoute);

	//Creates and launches a single packet
	void makePacket(int time);

	//Required for Node class membership. Throws an exception
	virtual void acceptPacket(Packet *p);

	//Determines whether a packet should be created
	virtual void nodeActions(int time);

	//How many more packets have to be sent?
	virtual bool isDone();

};

//Intermediate routing node
class Mule : public Node
{
	//int transTime;		//Amount of time it takes to transmit a packet
	
	enum Direction direction;
	int busyUntil;		//When the source stops being busy and can transmit another packet
	int maxQueueSize;	//The largest size the queue reaches
	//string name;		//Name of the node
	Queue<Packet> queue;//The queue of packets waiting to be transmitted

public:
	//Constructor
	Mule();

	//Receives a packet and puts it on the queue
	virtual void acceptPacket(Packet *p);

	//Determines if a packet should be sent out
	virtual void nodeActions(int time);

	//
	virtual void moveActions();

	//Are there any packets waiting to be sent?
	virtual bool isDone();

	//Prints out the maximum queue size
	virtual void printMule();
};

//Metadata for the receiver class
struct receiverMetaData
{
	int count;
	int num;
	int source;
};


//Receives and stores packets for analysis of the simulation
class Receiver : public Node
{
	Queue<receiverMetaData> metaData; //The metadata sorted by source of the packets

	Queue<Packet> queue; //All the packets that have arrived
public:
	Receiver(); //constructor
	virtual void acceptPacket(Packet *p); //Adds a packet to the queue
	virtual void nodeActions(int time); //Required for Node class membership
	virtual bool isDone();				//Receivers are always ready for the simulation to end
	virtual void printReceiver();		//Prints out this receivers data and analysis
	
	int queueLength() const;			//Returns the length of the queue (used for debugging)
};


class Field
{
	Node ***map; //Array containing the field
	
	int dimension;


public:

	
	Field(int dim);


	Node *getNode(int row, int column);

	bool setNode(int row, int column, Node* n);

	int getDimension()
		{ return dimension; }

	void printField(std::ostream &out);
	void moveNode(int &row, int &column, enum Direction &direction, Node* node = NULL);
};

//Including template files
#include "LinkedList.tcc"
#include "Queue.tcc"

//Ends the header inclusion if:
#endif