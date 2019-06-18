
template <class T>
LinkedList<T>::LinkedList()
{
	head = NULL;
}

template <class T>
void LinkedList<T>::insert(const T &val)
{	
	if(head == NULL)
	{
		head = new LLNode();
		head->value = val;
		head->next = NULL;
	}
	else
	{
		//Insert into already created list, giving priority to the newer item if they evaluate equally
		if(head->value >= val)
		{
			LLNode *temp = new LLNode();
			temp -> value = val;
			temp->next = head;
			head = temp;
		}
		else
		{
			LLNode *temp = head;
			while (temp->next != NULL && temp->next->value < val)
			{
				temp = temp->next;
			}
			LLNode *insertNode = new LLNode();
			insertNode->value = val;
			insertNode->next = temp->next;
			temp->next = insertNode;
			
		}
	}//end head==NULL else
}//end insert

template <class T>
T LinkedList<T>::getByIndex(int index) const
{
	LLNode *temp = this->head;
		
	int i;
	for (i = 0; i < index; i++)
	{
		if (temp->next != NULL)
		{
			temp = temp->next;
		}
		else
			throw new std::out_of_range("Beyond bounds of this linked list.");
	}//end for
	
	return temp->value;
}//end function getByIndex

template <class T>
int LinkedList<T>::length() const
{
	int length = 0;
	LLNode *current = this->head;

	while (current != NULL)
	{
		length++;
		current = current->next;
	}
	
	return length;
}//end function length

template <class T>
bool LinkedList<T>::isEmpty() const
{
	if (head == NULL)
	{
		return true;
	}
	else 
		return false;
}

template <class T>
T LinkedList<T>::headVal() const
{
	if(head == NULL)
		throw new std::underflow_error("Tried to get value from empty list in LinkedList::headVal()");
	
	return head->value;
}


template <class T>
T LinkedList<T>::pop()
{
	if(head != NULL)
	{
		LLNode *temp = head;
		head = head->next;
		T headVal = temp->value;
		return headVal;
	}
	else
	{
		throw new std::underflow_error("Tried to get value from empty list in LinkedList::pop()");
	}
}


template <class T>
template <class Y>
Y LinkedList<T>::traverse(Y (*function)(T *, Y), Y pastVal)
{
	if(DEBUG_MESG) std::cout << "Beginning traversal" <<std::endl;
	LLNode *ptr = head;
	while(ptr != NULL)
	{
		pastVal = function(&(ptr->value), pastVal);
		ptr = ptr->next;
	}
	
	if(DEBUG_MESG) std::cout << "Ending traversal" <<std::endl;
	return pastVal;
}