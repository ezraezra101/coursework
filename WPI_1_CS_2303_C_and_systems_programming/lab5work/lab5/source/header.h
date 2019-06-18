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

template<class T>
class LinkedList
{
	
	struct LLNode
	{
		T value;
		LLNode *next;
		
	};
	
	LLNode *head;
	LLNode *tail;
public:
	LinkedList();
	
	void insert(const T &);
	
	T headVal();
	
	T pop();
		
	bool isEmpty();
	T getByIndex(int index);
	int length() const;
	
	template<class Y>
	Y traverse(Y (*function)(T *, Y), Y);
	
	void addToEnd(const T &val);
};

template<class T>
class Queue : public LinkedList<T>
{
public:
	Queue();
	
	void enqueue(T &);
	
	T dequeue();
};

class Packet
{
	int creationTime;
	int nextActionTime;
	Source *creator;
	
	LinkedList<Node*> path;

public:
	Packet(int creationTime, int nextActionTime, Source *creator, LinkedList<Node*> &path);
	
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

	LinkedList<Node*> packetPath;
	
public:
	Source(int transTime, int propTime, int startTime,  LinkedList<Node*> packetPath, string name);

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
	Queue<Packet> queue;
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

#endif