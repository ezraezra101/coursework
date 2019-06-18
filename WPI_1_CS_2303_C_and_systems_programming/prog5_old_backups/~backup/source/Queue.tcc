#include "header.h"


//A queue data structure
//Written by Ezra Davis

//Constructor
template <class T>
Queue<T>::Queue() : LinkedList<T>()
{
	//Calls the LinkedList constructor
	
	tail = NULL;
	
}

//Copy constructor
//Written by Ezra Davis
template <class T>
Queue<T>::Queue(const Queue<T> &other)
{
	
	if(DEBUG_MESG) std::cout << "Calling copy constructor on queue: " << this <<std::endl << "Other queue has length " << other.length() <<std::endl;
	if(other.head != NULL)
	{
		this->head = new typename LinkedList<T>::LLNode(* other.head);
	
		typename LinkedList<T>::LLNode * *toCopy = & this->head->next;
	
		while (*toCopy != NULL)
		{
			if (DEBUG_MESG) std::cout << "Copying value" << std::endl;
			*toCopy = new typename LinkedList<T>::LLNode(**toCopy);
			toCopy = & (*toCopy)->next;
		}
	}
	else
	{
		if(DEBUG_MESG) std::cout << "Trying to make blank Queue" <<std::endl;
		this->head = NULL; //##Code smell - ideally should call LinkedList's constructor
		tail = NULL;
	}
}

//Adds an item to the back of the queue
//Written by Ezra Davis
template <class T>
void Queue<T>::enqueue(const T &val)
{
	if(tail != NULL)
	{
		tail->next = new typename LinkedList<T>::LLNode();
		tail = tail->next;
		tail->value = val;
		tail->next = NULL;
	}
	else //If the list is empty:
	{
		this->head = new typename LinkedList<T>::LLNode();
		this->head->next = NULL;
		this->head->value = val;
		tail = this->head;
	}
}

//Pops item off queue
//Written by Ezra Davis
template <class T>
T Queue<T>::dequeue()
{
	if(length() == 1)
		tail = NULL;
	return this->pop();
}

//Gets the length of the queue
//Written by Ezra Davis
template <class T>
int Queue<T>::length() const
{
	return ((LinkedList<T> *) this)->length();
}


//Is the queue empty?
//Written by Ezra Davis
template <class T>
bool Queue<T>::isEmpty() const
{
	return ((LinkedList<T> *) this)->isEmpty();
}

//Calls the function on every member of the LinkedList, not dissimilar to the visitor pattern
//Written by Ezra Davis
template <class T>
template <class Y>
Y Queue<T>::traverse(Y (*function)(T *, Y), Y pastVal)
{
	return ((LinkedList<T> *) this)->traverse(function, pastVal);
}

//Return the front of the queue without removing it
//Written by Ezra Davis
template <class T>
T Queue<T>::headVal() const
{
	return ((LinkedList<T> *) this)->headVal();
}

template <class T>
T Queue<T>::getByIndex(int index) const
{
	return ((LinkedList<T> *) this)->getByIndex(index);
}
