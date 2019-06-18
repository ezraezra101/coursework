#include "header.h"
#include "RootNode.cpp"

//Team 4J: Ezra Davis & Artian Kica

//Returns whether or not a character is punctuation
bool isPunctuation(char c)
{
	static const string punctuation = "? !\"()./-;:[]{}`,"; //This string contains all characters we consider to be "punctuation" and are not in words
	int i; //counter

	for(i=0; i < punctuation.length(); i++ ) //for each character in the punctuation string
	{
		//if the given character is punctuation
		if(punctuation[i] == c)
			return true;
	}//end string for
	
	return false; //character is not punctuation
}//end isPunctuation

//Inserts formats and inserts a string (and metadata) into the tree
//This function iterates through the line until it hits punctuation, at which
//point it will end the word and insert it into the given tree. Because of this,
//we consider words as a series of characters starting after and ending before
//a punctuation character (specified in the static constant in isPunctuation)
//Written by Ezra Davis & Artian Kica
void insertIntoTree(string &s, RootNode *aTree, unsigned short lineNum)
{
	string word = ""; //blank word
	
	//Iterator code from here:
	//http://www.cplusplus.com/reference/string/string/begin/
	for( std::string::iterator it = s.begin(); it!=s.end(); ++it)
	{
		if(!isPunctuation(*it)) //if the iterated character is NOT punctuation
		{
			word += tolower(*it); //build the word
		}//end if not punctuation
		
		else if (word != "") //if the word built up so far is not blank (at least 1 character)
		{
			//insert the word into aTree
			aTree->insert(word, 1, lineNum);	
		
			//reset the word
			word = "";
			
		} //end if punctuation and word isn't empty
		
	}//end iterator for
	
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
	
	while( not input.eof() ) //getLine and insert line into tree until end of file
	{
		lineNum++;
		
		getline(input, line);
		
		//Formats text (remove punctuation, set lowercase)
		//and puts line into binary tree
		insertIntoTree(line, aTree, lineNum);
	}

	input.close(); //close file

	//Write output
	cout << endl;
	
	aTree->printOut();
	
	return 0;
}



