#include "header.h"



template <class T>
Queue<T>::Queue() : LinkedList<T>()
{
	//Calls the LinkedList constructor
	
	tail = NULL;
	
}

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
		this->insert(val);
		tail = this->head;
	}
}

template <class T>
T Queue<T>::dequeue()
{
	if(length() == 1)
		tail = NULL;
	return this->pop();
}

template <class T>
int Queue<T>::length() const
{
	return ((LinkedList<T> *) this)->length();
}


template <class T>
bool Queue<T>::isEmpty() const
{
	return ((LinkedList<T> *) this)->isEmpty();
}

template <class T>
template <class Y>
Y Queue<T>::traverse(Y (*function)(T *, Y), Y pastVal)
{
	return ((LinkedList<T> *) this)->traverse(function, pastVal);
}