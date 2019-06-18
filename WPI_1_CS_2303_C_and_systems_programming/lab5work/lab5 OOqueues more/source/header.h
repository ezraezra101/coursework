#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#define DEBUG_MESG false

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

using std::string;

class Source;
class Node;


//A sorted LinkedList
template<class T>
class LinkedList
{
protected:
	//LLNodes represent an entry in the linkedList
	struct LLNode
	{
		T value;
		LLNode *next;
		
	};
	
	//The first entry in the linkedList
	LLNode *head;
public:
	LinkedList();
	
	void insert(const T &); //Inserts the argument into the correct place in the linkedList
	
	T headVal() const;
	
	T pop();
		
	bool isEmpty() const;
	T getByIndex(int index) const;
	int length() const;
	
	template<class Y>
	Y traverse(Y (*function)(T *, Y), Y);
};

template<class T>
class Queue : private LinkedList<T>
{	
	typename LinkedList<T>::LLNode *tail;
public:
	Queue();
	
	Queue(const Queue &);
	
	void enqueue(const T &);
	
	T dequeue();
	
	int length() const;
	
	bool isEmpty() const;
	
	template<class Y>
	Y traverse(Y (*function)(T *, Y), Y);
};



template <class T, class nameClass>
class MetaListMem : public LinkedList<T>
{
	nameClass name;
public:
	//Default constructor required to be in a LinkedList
	MetaListMem() : LinkedList<T>() {}
	
	MetaListMem(nameClass name) : LinkedList<T>()
		{ this->name = name; }
	
	bool operator>(const MetaListMem<T, nameClass> &other) const {return name > other.name;}
	bool operator<(const MetaListMem<T, nameClass> &other) const {return name < other.name;}
	bool operator>=(const MetaListMem<T, nameClass> &other) const {return ! (*this < other);}
	bool operator<=(const MetaListMem<T, nameClass> &other) const {return ! (*this > other);}
	
	bool operator>(const nameClass &other) const {return name > other;}
	bool operator<(const nameClass &other) const {return name < other;}
	bool operator>=(const nameClass &other) const {return ! (*this < other);}
	bool operator<=(const nameClass &other) const {return ! (*this > other);}
	
	
	bool nameEq(const nameClass &other) const {return other == name;}
	nameClass getName() const {return name;}
};

/*
template <class T, class nameClass>
bool MetaListMem<T, nameClass>::operator>(const MetaListMem<T, nameClass> &other) const
{
	return name > other.name;
}
*/
	
template <class T, class nameClass>
class MetaList : public LinkedList<MetaListMem<T, nameClass> >
{
public:
	MetaList() : LinkedList<MetaListMem<T, nameClass> >()
		{}
	
	void insert(const T &toInsert, const nameClass name);
	
	template <class Y>
	Y metaTraverse(Y (*function)(T *, Y), Y pastVal);
	
	int metaLength() const;
};




class Packet
{
	int creationTime;
	int nextActionTime;
	Source *creator;
	
	Queue<Node*> path;

public:
	Packet(int creationTime, int nextActionTime, Source *creator, Queue<Node*> &path);
	
	Packet(){} //For housekeeping
	
	bool operator>(const Packet &) const;
	bool operator<(const Packet &) const;
	bool operator<=(const Packet &other) const;
	bool operator>=(const Packet &other) const;
	
	int getNextActionTime() const;
	void setNextActionTime(int setTime);
	
	void goToNextNode();
	int getResponseTime() const;
	string getCreatorName() const;

};

class Node
{
	
protected:
	static LinkedList<Node*> nodes;

public:
	static LinkedList<Packet> eventList;
	
	static void packetActions(int time);
	
	virtual void acceptPacket(Packet *p) = 0;
	static void beginStep(int time);
	virtual void nodeActions(int time) = 0;
	static bool allDone();
	virtual bool isDone() = 0; 
	
	static void printResults();
	virtual void printReceiver() {};
	virtual void printMule() {};
};


class Source : public Node
{
	int transTime;
	int propTime;
	int startTime;
	int packetsToSend;
	int busyUntil;
	string name;

	Queue<Node*> packetPath;
	
public:
	Source(int transTime, int propTime, int startTime, Queue<Node*> packetPath, string name);

	void makePacket(int time);
	virtual void acceptPacket(Packet *p);
	virtual void nodeActions(int time);
	virtual bool isDone();
	string getName() const;
};


class Mule : public Node
{
	int transTime;
	int propTime;
	int busyUntil;
	int maxQueueSize;
	Queue<Packet> queue;

public:
	Mule(int transTime, int propTime);
	virtual void acceptPacket(Packet *p);
	virtual void nodeActions(int time);
	virtual bool isDone();
	virtual void printMule();
};

class Receiver : public Node
{
	MetaList<Packet, string> receivedPacketsBySource;
public:
	Receiver();
	virtual void acceptPacket(Packet *p);
	virtual void nodeActions(int time);
	virtual bool isDone();
	virtual void printReceiver();
	
	int queueLength() const;
};


//Including template files
#include "LinkedList.tcc"
#include "Queue.tcc"
#include "MetaList.tcc"

#endif