#include "header.h"

//Written by Artian Kica
//Constructor
Field::Field(int dim) : dimension(dim)
{
	map = new Node**[dimension];

	for (int i = 0; i < dimension; ++i)
	{
		map[i] = new Node*[dimension + 2];
	}
}//end constructor

//Written by Ezra Davis
//Gets a node from the field
Node *Field::getNode(int row, int column)
{
	return map[row][column + 1];
}

//Written by Ezra Davis
//Tries to set a location to a value. If already filled, return false and end early
bool Field::setNode(int row, int column, Node* n)
{
	if (map[row][column + 1] != NULL) //if space is not empty
	{
		return false;
	}

	map[row][column + 1] = n;
	//printField(std::cout);
	return true;
}

//Written by Artian Kica
//Moves a node to a new location
void Field::moveNode(int &row, int &column, enum Direction &direction, Node* node) //node is, by default NULL
{

	if(node == NULL)
	{
		node = getNode(row, column);
		map[row][column + 1] = NULL;
	}

		//Move in a direction:
	if(direction == East)
		column++;
	else if(direction == South)
		row++;
	else if(direction == West)
		column--;
	else if (direction == North)
		row--;
	else
		std::cout << "Invalid Direction!!!!" << std::endl;


	//If we moved too far, bounce:
	if (row < 0)
	{
		row = 1;
		direction = South;
	}
	else if (row > getDimension() - 1)
	{
		row = getDimension()-2;
		direction = North;
	}
	else if (column > getDimension() - 1)
	{
		column = getDimension()-2;
		direction = West;
	}
	else if (column < 0)
	{
		column = 1;
		direction = East;
	}


	//If there is a collision, hop:
	if(!setNode(row, column, node))
	{
		this->moveNode(row, column, direction, node);
	}
}//end function moveNode



//Ezra Davis
//Prints the field
void Field::printField(std::ostream &out)
{

	int i;//counter for columns
	int j;//counter for rows
	
	//prints the top border of the field
	for(j=-1; j <= dimension; j++){
		out << "---";
	}
		out << "---\n";
	
	//loops through rows
	for(i=0; i < dimension; i++){
		out << "|";
		
		//loops through columns
		for(j=-1; j <= dimension; j++){
			//Prints nodes at this location
			out << " ";

			if(map[i][j + 1] == NULL)
				out << "  ";
			else
				out << std::setw(2) << map[i][j + 1]->getId();
		}
		out << " |\n";
	}
	
	//prints the top border of the field
	for(j=-1; j <= dimension; j++){
		out << "---";
	}
		out << "---\n";
}//end printField
