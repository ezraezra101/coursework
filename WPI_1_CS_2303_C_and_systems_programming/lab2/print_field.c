//Ezra Davis
//CS2303 C04 2014 -- lab2
#include "motion.h"
#include <stdlib.h>

//Prints out the field with exactly 3 decimal digits for each integer element of field
void print_field()
{
 int i, j;
 //prints out the dashes above
 printf("\n ");
 for(j = 0; j<LENGTH; j++)
 {
  printf("----");
 }//End for
 printf(" \n");

 //prints out the digits
 for(i = 0; i<WIDTH; i++)
 {
  printf("|");//Edge of field
  for(j = 0; j<LENGTH; j++)
  {
  
	  //prints out EXACTLY 3 decimal digits for each location on the field (absolute values)
   int dispInt = abs(field[i][j] % 1000);
   if(dispInt<10)
   {
    printf("00%d ", dispInt);
   } else if (dispInt < 100)
   {
    printf("0%d ", dispInt);
   } else {
    printf("%d ", dispInt);
   } //End conditional
   
  } //End inner for
  printf("|\n");
 }//End outer for

 //Prints out dashes below
 printf(" ");
 for(j = 0; j<LENGTH; j++)
 {
  printf("----");
 } //end for
 printf(" \n");

}//end print_field
