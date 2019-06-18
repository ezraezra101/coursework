#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

#include <iostream>
#include <fstream>

template<class T>
class LinkedList
{
	
	class LLNode
	{
		T value;
	public:
		LLNode *next;
		
		LLNode(T);
		
		void insert(T);
		
		T getVal();
	};
	
	LLNode *head;
public:
	LinkedList();
	
	void insert(T );
	
	T headVal();
	
	T pop();
};


class Packet
{
	int creationTime;

public:
	bool operator>(Packet*);
	bool operator<(Packet*);

};

class Node
{
	
};


class Source : public Node
{
	int transTime;
	int propTime;
	
public:
	Source(int transTime, int propTime, int startTime);//Route

	void makePackets();
};


class Mule : public Node
{
	int transTime;
	int propTime;

public:
	Mule(int transTime, int propTime);
};

class Receiver : public Node
{

};

#endif