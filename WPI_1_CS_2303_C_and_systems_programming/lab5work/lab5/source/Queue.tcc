#include "header.h"



template <class T>
Queue<T>::Queue() : LinkedList<T>()
{
	//Calls the LinkedList constructor
}

template <class T>
void Queue<T>::enqueue(T &data)
{
	this->addToEnd(data);
}

template <class T>
T Queue<T>::dequeue()
{
	return this->pop();
}

