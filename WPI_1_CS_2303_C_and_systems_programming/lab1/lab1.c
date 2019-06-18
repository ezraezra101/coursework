//Ezra Davis
//Averages a sequence of Integers.
//Assumes the last value read with scanf is the sentinel 9999.

#include <stdio.h>

// Function main begins program execution.
int main(void){

   int x = 0;
   float intCount = -1.0;
   int sum = 0;

   printf("%s", "Enter some integers to be averaged, press enter between each one and end with 9999\n");

   while (x!=9999){

      sum += x;
      intCount++;

      scanf( "%i", &x);

   }//End while

   printf("\n%f\n",sum/intCount);

} // end function main
