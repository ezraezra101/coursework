#include "header.h"

#include "LinkedList.cpp"

int main()
{
	
	LinkedList<int> list;
	
	list.insert(5);
	list.insert(7);
	
	std::cout << list.headVal() << std::endl << list.pop() << std::endl << list.pop();
	
}