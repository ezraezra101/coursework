//Team 4J: Ezra Davis & Artian Kica

#include "header.h"
#include "LinkedList.cpp"

//The tree class is a binary tree sorted alphabetically
//by its words. It remembers the word's count and which lines
//the word appears on with a LinkedList.
class Tree {

	string word;
	unsigned short count;
	LinkedList *lines;
	Tree *left;
	Tree *right;

public:

	//Constructor for Tree with word, count and lineNum
	//Written by Ezra Davis & Artian Kica
	Tree(string word, unsigned short count, unsigned short lineNum)
	{
		this->word = word;
		this->count = count;
		
		this->lines = new LinkedList(lineNum);
		
		left = NULL;
		right = NULL;
	} //end constructor

	//Inserts a word and a count into the tree. If the word is 
	//already in the tree, it increases the word's count.
	//Written by Ezra Davis & Artian Kica
	bool insert(string word, unsigned short count, unsigned short lineNum)
	{
		//if new word is alphabetically later
		if(word > this->word)
		{	
			//if the right node is empty
			if(right == NULL){
				this->right = new Tree(word, count, lineNum);
			}
			//else right node is not empty
			else {
				this->right->insert(word, count, lineNum);
			}//end null if
		}
		//if new word is alphabetically earlier
		else if(word < this->word)
		{
			//if left node is empty
			if(left == NULL){
				this->left = new Tree(word, count, lineNum);
			}
			//else left node is not empty
			else{
				this->left->insert(word, count, lineNum);
			}//end null if
			
		}
		//else this word is the same as new word
		else
		{
			this->count += count;
			this->lines->insert(lineNum);
		}
		
		return true;
	}//end insert


	//Prints the contents of the tree to screen
	//Returns uniqueWords
	//Written by Ezra Davis
	unsigned short printOut()
	{
		unsigned short uniqueWords = 1;
		
		if(left != NULL) //if left is not null
			uniqueWords += left->printOut(); //sum unique words

		//printing
		cout << word << string( COUNT_INDENT - word.length() - 2, ' ') << "| "
			<< count << string(LINES_INDENT - digits(count) - 2, ' ') << "| ";
		lines->printOut(LINES_INDENT + COUNT_INDENT);
		cout << endl;

		if(right != NULL) //if right is not null
			uniqueWords += right->printOut(); //sum unique words
				
		return uniqueWords;
	}//end printout

};//End class Tree