//Team 4J: Ezra Davis & Artian Kica

#include "header.h"

//class LinkedList
//LinkedList is a class for storing the lines each word occurs on.
//It is used in Tree as a data member
class LinkedList {
	LinkedList *next;
	unsigned short lineNum;
	
public:
	//Constructor for LinkedList with 1 element
	//Written by Ezra Davis & Artian Kica
	LinkedList(unsigned short lineNum)
	{
		this->lineNum = lineNum;
		this->next = NULL;
	}//end constructor
	
	
	//Inserts a line number into the linked list. Ignores duplicates.
	//Written by Ezra Davis & Artian Kica
	void insert(unsigned short lineNum)
	{
		if(this->next != NULL)
		{
			this->next->insert(lineNum);
		}
		else if(lineNum != this->lineNum)
		{
			this->next = new LinkedList(lineNum);
		}
	}//end insert


	//Prints the contents of the linked list to cout
	//Written by Ezra Davis
	void printOut(unsigned short lengthSoFar)
	{
		//myLength is the number of characters this printout takes up.
		unsigned short myLength = digits(lineNum) + 1;
		lengthSoFar += myLength;
		
		//If this would cause the screen size to overflow
		if(lengthSoFar >= SCREEN_SIZE)
		{
			//Make a newline
			cout << endl;
			//Indent appropriately
			cout << string(COUNT_INDENT - 2, ' ');
			cout << "| " << string(LINES_INDENT - 2, ' ') << "| ";
			
			//Reset lengthSoFar
			lengthSoFar = LINES_INDENT + COUNT_INDENT + myLength;
		}
		//end handle linebreaks
		
		
		//Print out the number
		cout << lineNum << " ";
		
		//if not at the end of the LinkedList, continue
		if(this->next != NULL)
		{
			this->next->printOut(lengthSoFar);
		}
	}//end function printOut

};//end class LinkedList