#include <iostream>
#include <cstdlib>
#include <string>

//Ezra Davis
//February 12, 2014
//Lab 4
//CS2303


//Binary tree structure
struct Tree {
	std::string ID; //Student ID
	float g1;//First test grade
	float g2;//Second test grade
	float g3;//Third test grade
	double avg; //Average of the test grades
	Tree* l_child; //left child
	Tree* r_child; //right child
};

//Places a tree node into the binary tree
void place(struct Tree** tree, struct Tree *newNode)
{
	if(*tree == NULL)
	{
		//std::cout << "Adding node: " << newNode->ID << std::endl;
		*tree = newNode;
	}
	else
	{
		if((*tree)->ID.compare(newNode->ID) > 0)
		{
			place(&((*tree)->l_child), newNode);
		}
		else if((*tree)->ID.compare(newNode->ID) < 0)
		{
			place(&((*tree)->r_child), newNode);
		}
		//else, the IDs equal
		else
		{
			std::cout << "Error: Duplicate Student ID\n";
			std::exit(1);
		}
	}//end else
}

//Inserts a new tree struct into the binary tree
void insert(struct Tree** tree, std::string id, float g1, float g2, float g3)
{
	//Creates new struct
	struct Tree *newNode = new Tree;

	newNode->ID = id;
	newNode->g1 = g1;
	newNode->g2 = g2;
	newNode->g3 = g3;
	newNode->avg = (g1 + g2 + g3) / 3.0;
	newNode->l_child = NULL;
	newNode->r_child = NULL;
	
	//std::cout << "Inserting node " << id << std::endl;
	
	place(tree, newNode);
	
}

//Prints a single node of the tree
void print_node(struct Tree *node)
{
	std::cout
		<< node->ID << "\t"
		<< node->g1 << "\t"
		<< node->g2 << "\t"
		<< node->g3 << "\t"
		<< node->avg << "\t"
		<< std::endl;
}

//Inorder printing of the tree
void print_traversal(struct Tree *tree)
{
	if(tree == NULL){
		//std::cout << " NULL \n";
	}
	else
	{
		print_traversal(tree->l_child);
		print_node(tree);
		print_traversal(tree->r_child);
		
	}
}

// Inorder printing of binary tree, includes header
void print_tree(struct Tree *tree)
{
	std::cout << "Student ID\tGrade 1\tGrade 2\tGrade 3\tAverage" << std::endl;
	print_traversal(tree);
}

//Throws an error if the input is illegal
void validate_input(std::string id, float g1, float g2, float g3)
{
	try {
		if(g1 > 100.0 || g1 < 0.0)
			throw "First test score out of range";
		if(g2 > 100.0 || g2 < 0.0)
			throw "Second test score out of range";
		if(g3 > 100.0 || g3 < 0.0)
			throw "Third test score out of range";
		if(id.length() != 9)
			throw "Student ID length incorrect";
		int i;
		for(i=0; i < 9; i++){
			if(id[i] < '0' || id[i] > '9')
				throw "Non digit characters in student ID";
		}
	}
	catch(char const* error) {
		std::cout << "Error: invalid input.\n" << error << std::endl;
		std::exit(1);
	}
	
}


//Creates and prints a binary tree.
int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << "Expects one argument." << std::endl;
		
		return 1;
	}
	
	struct Tree * aTree = NULL;
	
	int i;
	int students = std::atoi(*(argv+1));
	std::string studentID;
	float g1 = 0.0;
	float g2 = 0.0;
	float g3 = 0.0;
	for(i=0; i < students;i++)
	{
		std::cin >> studentID;
		std::cin >> g1 ;
		std::cin >> g2 ;
		std::cin >> g3 ;
		
		validate_input(studentID, g1, g2, g3);
		insert(&aTree, studentID, g1, g2, g3);
		
	}
	
	if(aTree == NULL)
	{
		std::cout << "No Input";
	}
	
	print_tree(aTree);
}
