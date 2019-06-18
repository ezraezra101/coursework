#include "header.h"

class Field
{
	Node ***map; //Array containing the field
	
	int dimension;


public:

	
	Field(int dim) : dimension(dim)
	{
		map = new Node**[dimension];

		for (int i = 0; i < dimension; ++i)
		{
			map[i] = new Node*[dimension + 2];
		}
	}//end constructor


	Node *getNode(int row, int column)
	{
		return map[row][column + 1];
	}

	bool setNode(int row, int column, Node* n)
	{
		if (map[row][column] != NULL) //if space is not empty
		{
			return false;
		}

		map[row][column + 1] = n;
		return true;
	}

	int getDimension()
	{
		return dimension;
	}

};


/*
int main()
{
	Field x(5);

	std::cout << (x.getNode(4,4) == NULL) << std::endl;
}
*/