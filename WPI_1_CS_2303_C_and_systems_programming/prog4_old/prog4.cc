#include "header.h"
#include "RootNode.cpp"

//Returns whether or not a character is punctuation
bool isPunctuation(char c)
{
	static const string punctuation = "? !\"()./-;:[]{}`,";
	int i; //counter

	for(i=0; i < punctuation.length(); i++ )
	{
		if(punctuation[i] == c)
			return true;
	}
	
	return false;
}//end isPunctuation

//Inserts formats and inserts a string (and metadata) into the tree
void insertIntoTree(string &s, RootNode *aTree, unsigned short lineNum)
{
	string word = "";
	
	//Iterator code from here:
	//http://www.cplusplus.com/reference/string/string/begin/
	for( std::string::iterator it = s.begin(); it!=s.end(); ++it)
	{
		if(!isPunctuation(*it))
		{
			word += tolower(*it);
		}
		else if (word != "")
		{

			aTree->insert(word, 1, lineNum);	
		
			word = "";
		} //end if punctuation and word isn't empty
		
		
	}
}//end insertIntoTree

int main(int argc, char** argv)
{
	//Check command line arguments	
	if(argc != 2)
	{
		cout << "Wrong number of arguments." << std::endl;
		return -1;
	}	
	
	//Read input and place into binary tree
	
	fstream input;
	input.open(argv[1]);
	unsigned short lineNum = 0;
	
	string line;
	
	RootNode *aTree = new RootNode();
	
	while( not input.eof() )
	{
		lineNum++;
		
		getline(input, line);
		
		//Strip punctuation and		
		//puts line into binary tree
		insertIntoTree(line, aTree, lineNum);
	}

	input.close();

	//Write output
	
	cout << endl;
	
	aTree->printOut();
	
}



