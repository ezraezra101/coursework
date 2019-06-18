#include "header.h"

template <class T>
LinkedList<T>::LLNode::LLNode(T val)
{
	value = val;
	next = NULL;
}

template <class T>
LLNode<T> *LinkedList<T>::LLNode::insert(T val)
{

	if(*next <= *this)
	{
		LLNode<T> *prev = new LLNode(val);
		
		prev->next = this;
		
		return prev;
	}
	if(next == NULL)//If at end of list
	{
		next = new LLNode(val);
		return this;
	}	
	else
		next = next->insert(val);
}

template <class T>
T LinkedList<T>::LLNode::getVal()
{
	return value;
}





template <class T>
LinkedList<T>::LinkedList()
{
	head = NULL;
}

template <class T>
void LinkedList<T>::insert(T val)
{
	if(head == NULL)
		head = new LLNode(val);
	else
		head = head->insert(val);
}


template <class T>
T LinkedList<T>::headVal()
{
	return head->getVal();
}


template <class T>
T LinkedList<T>::pop()
{
	if(head != NULL)
	{
		LLNode *temp = head;
		head = head->next;
		T headVal = temp->getVal();
		delete(temp);
		return headVal;
	}
	else
	{
		std::cout << "ERROR: Tried to get value from empty list" << std::endl;
		return (T) NULL; //Awful terrible practice
	}
}