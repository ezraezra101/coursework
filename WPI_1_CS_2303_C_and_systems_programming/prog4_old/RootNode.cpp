#include "header.h"
#include "Tree.cpp"

//#########COMMENT
//A wrapper for the Tree class
class RootNode {
private:
	Tree *root;
public:
	RootNode(){};
	
	bool insert(string word, unsigned short count, unsigned short lineNum)
	{
		if(root == NULL)
		{
			root = new Tree(word, count, lineNum);
			return true;
		}
		
		return root->insert(word, count, lineNum);
	}//end insert
	
	bool printOut()
	{
		if(root == NULL)
		{
			cout << "Error: attempted to print out empty tree" << endl;
			return false;
			
		}else
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
		}
	}

};//End class RootNode