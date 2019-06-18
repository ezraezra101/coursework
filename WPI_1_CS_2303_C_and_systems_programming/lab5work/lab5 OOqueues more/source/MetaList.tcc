#include "header.h"

template <class T, class nameClass>
template <class Y>
Y MetaList<T, nameClass>::metaTraverse(Y (*function)(T *, Y), Y pastVal)
{
	typename LinkedList<MetaListMem<T, nameClass> >::\
		LLNode* temp = this->head;
	
	while(temp != NULL)
	{
		pastVal = temp->value.traverse(function, pastVal);
		
		temp = temp->next;
	}
	return pastVal;
}



template <class T, class nameClass>
void MetaList<T, nameClass>::insert(const T &toInsert, const nameClass name)
{

	typename LinkedList<MetaListMem<T, nameClass> >::\
		LLNode** temp = & this->head;
	
	while( (*temp != NULL)  && ( (*temp)->value < name ) )
	{
		temp = & (*temp)->next;
	}
	
	if(*temp == NULL)//If at end of list
	{
		
		*temp = new typename LinkedList<MetaListMem<T, nameClass> >::LLNode();
		(*temp)->value = MetaListMem<T,nameClass>(name);
		(*temp)->next = NULL;
		
		(*temp)->value.insert(toInsert);
	}
	else if ((*temp)->value.nameEq(name))
	{
		(*temp)->value.insert(toInsert);
	}
	else //insert inbetween
	{
		//Inserts in between 
		typename LinkedList<MetaListMem<T, nameClass> >::LLNode * newNode = new typename LinkedList<MetaListMem<T, nameClass> >::LLNode();
		
		newNode->next = (*temp)->next;
		newNode->value = MetaListMem<T,nameClass>(name);
		newNode->value.insert(toInsert);
		*temp = newNode;
	}

}//end insert for metaList

template <class T, class nameClass>
int sumLength(MetaListMem<T, nameClass> *list, int lastLength)
{
	return lastLength + list->length();
}

template <class T, class nameClass>
int MetaList<T, nameClass>::metaLength() const
{
	//Y LinkedList<T>::traverse(Y (*function)(T *, Y), Y pastVal);
	return ((LinkedList<MetaListMem<T, nameClass> > *) this)->traverse(sumLength, 0);
}

/*
int main()
{
	std::cout << "Didn't break yet!" << std::endl;
		
	MetaList<int, string> *x = new MetaList<int, string>();
	
	MetaListMem<int, string> *mem = new MetaListMem<int, string>("Name");
	
	x->insert(5, "x");
	std::cout << "5 inserted\n";
	x->insert(6, "x");
	std::cout << "6 inserted\n";
	x->insert(7, "y");
	
	std::cout << (string("x") < string("x"));
	
	std::cout << "Should be 1: " <<(( string("Name") < string("A")) ==(*mem < MetaListMem<int, string>("A")) ) << std::endl; 
	
	std::cout << "The length of the first list in the metalist: " << x->headVal().length() << " (should be 2)" <<std::endl;
	
	
	std::cout << "MetaList length, should be 2:" << std::endl;
	std::cout << x->length() << std::endl;
}
*/