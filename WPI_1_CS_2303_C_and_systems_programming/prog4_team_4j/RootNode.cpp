//Team 4J: Ezra Davis & Artian Kica

#include "header.h"
#include "Tree.cpp"

//Root Node is used as the root of the binary tree.
//A wrapper for the Tree class
class RootNode {
private:
	Tree *root;
public:
	RootNode(){}; //very interesting constructor
	
	//inserts a word into the tree. If tree is empty, starts tree with root.
	//Written by Ezra Davis & Artian Kica
	bool insert(string word, unsigned short count, unsigned short lineNum)
	{
		if(root == NULL) //if tree is empty
		{
			root = new Tree(word, count, lineNum); //start tree and insert word and its associated count and lineNum
			return true;
		}
		
		return root->insert(word, count, lineNum); //tree not empty, insert word and things
		
	}//end insert
	
	//prints out contents of root. 
	//Written by Ezra Davis
	bool printOut()
	{
		if(root == NULL) //if tree is empty
		{
			cout << "Error: attempted to print out empty tree" << endl;
			return false;
			
		}//end if root is null
		
		else //if tree is not empty
		{
			cout << "Word"
				<< string(COUNT_INDENT - 4 - 2, ' ')
				<< "| Count"
				<< string(LINES_INDENT - 5 -2, ' ')
				<< "| Lines" << endl;

			cout << std::setfill('-') << std::setw(80) << '-' << endl;
			
			unsigned short uniqueWords = root->printOut();
			
			cout << "Total Unique Words: \t" << uniqueWords << endl;
			
			return true;
			
		}//end else root not null
		
	}//end function printOut

};//End class RootNode