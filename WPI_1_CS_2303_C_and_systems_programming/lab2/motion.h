//Ezra Davis
//CS2303 C04 2014 -- lab2

#include <stdio.h>

#define LENGTH 18//Columns
#define WIDTH 12//Rows
#define SAMPLES 8 //Number of lines of input taken
enum direction {Right, Left, Up, Down};
int field [WIDTH][LENGTH];

//Prints the array field
void print_field();

//Starting in the specified row and column, motion prints nodeid, incrementing it and moving in the specified direction until it reaches an edge
int motion (int nodeid, int row, int column, int direction);