//Ezra Davis
//CS2303 C04 2014 -- lab2
#include <stdio.h>
#include "motion.h"
//Reads a in a single line of input that contains the integers samples. Then reads in samples lines of test data. Each sample line of test data containts 4 integers corresponding to: nodeid, row, column, and direction.

//Fills the array with zeros
void clear_field(){
    int j;//row counter
    int k;//column counter
    for(j=0; j < WIDTH; j++){
  	  for(k=0; k < LENGTH; k++){
  		  field[j][k] = 0;
  	  }//end column for
    }//end row for
}//end clear_field

int main( void)
{

 int i;//SAMPLES counter
 
 for(i = 0; i<SAMPLES; i++)
 {
	 //Main reads in a line of data
  printf("Please input the nodeid, row, column, and direction\n> ");  
  int nodeid, row, column, direction;
  scanf("%d%d%d%d", &nodeid, &row, &column, &direction);

  //1. Clear the field (i.e., fill the field array with zeros).
  clear_field();
  //2. Call motion using the sample test data as arguments.
  motion(nodeid, row, column, direction);
  //3. Call print_field to print out the resultant field to stdout.
  print_field();
  
  
 } //End for loop
 return 0;
}//End main
