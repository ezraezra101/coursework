//Ezra Davis
//CS2303 C04 2014 -- lab2
#include "motion.h"

//Starting in the specified row and column, motion prints nodeid, incrementing it and moving in the specified direction until it reaches an edge
int motion (int nodeid, int row, int column, int direction)
{
	int dist = 0;
	
	//repeat until the current row and column reaches the edge of the array
	while (row >= 0 && row < WIDTH && column >= 0 && column < LENGTH)
	{	//Sets the correct location of field to nodeid plus the number of movements undertaken already
		field[row][column] = dist + nodeid;
		dist++;//increments the next value a location in the field will be sent to
		
		//sets the next location to modify appropriately
		if (direction == Up){
			row--;
		}
		else if (direction == Right){
			column++;
		}
		else if (direction == Down){
			row++;
		}
		else if (direction == Left){
			column--;
		}
		else {
			printf("Oh dear... The direction given wasn't 0, 1, 2, or 3\n");
			break;
		}
			
	}//End while

	return 0; //I don't understand why motion has an int return type
}//End motion
