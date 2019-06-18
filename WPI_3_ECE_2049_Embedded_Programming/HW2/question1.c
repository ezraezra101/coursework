1ia)

#include "msp430.h"
#include <stdlib.h>
int main(void)
{ // give size of these variable
	int arr[100], i; // arr is 1600 bits, and i is 16 bits
	long unsigned int sq_sum; // sq_sum is 32 bits
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    for (i=0; i<100; i++) // Loop 100 times
    {
       arr[i] = (rand() % 500)–150; // set arr[i] to a number between -150 and 350
       sq_sum += arr[i]*arr[i];   // sq_sum becomes the sum of the squares of all numbers in arr.
    }
}


1ib)

#include "msp430.h"
#include <math.h>
#include <stdlib.h>
void main(void)
{ // give size of these variable
	float arr[100]; // arr is 3200 bits
	int i;			// i is 16 bits
	float sq_sum;	// sq_sum is 32 bits
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	i = 0;
	while (i<100) {
         arr[i] = (rand() % 500)–150.5; // Sets arr[i] to a number between 349.5 and -150.5 (always ending in 0.5)
         sq_sum += pow(arr[i],2.0);   	// sets sq_sum to the sum of the squares of all numbers in arr.
         i++;   // Increment i. Causes loop to run 100 times
	 }
}


## TODO run code

iii)
Integer math on the msp430 is much faster than float or double math because, not only are floats and doubles larger than the processor's word size, float and double operations have to be simulated in software. Doubles are four times the size of integers, so they take up much more space. It is usually best to avoid using floats or doubles on a chip like the MSP430.
	



2a)
##TODO Do later
2b)
It uses both RAM and flash memory because they fundamentally serve different purposes - RAM is volatile memory (needs constant power) and is much faster than flash memory (which is non-volatile and can be used to save programs and data between resets)

2c)
##TODO

2d)
##TODO




