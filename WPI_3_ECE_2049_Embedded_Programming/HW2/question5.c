
/* Compiler directives  (includes and defines) */
#include "msp430x44x.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Function prototypes */
void setupP6();
void P6inOut();
/*** Implement your functions here ***/
void setupP6() {
	P6SEL = 0xF0; // Configure all pins as digital IO
	P6DIR = 0xF0; // Configure pins 3-0 as digital inputs, 7-4 as outputs
	// Not changing P6OUT, P6REN, or P6DS
}

void P6inOut() {
	// Could have just set P6OUT to P6IN << 4, but I wanted to be tidy.
	P6OUT = (P6IN << 4) + (0x0F & P6OUT);
}

/* Write your main() here */
void main()
{
	setupP6();
	
	while(1) {
		P6inOut()
	}
}

// c)
// If P6IN == 0xFA, P6OUT would be set to 0xAX where X is whatever P6OUT was originally set to.
// If P6IN == 0x55, P6OUT would be set to 0x5X where X is whatever P6OUT was originally set to.


6a)
Switches would be digital inputs. The switch in the right position would produce a digital 0, the left position would produce digital 0.

6b)
Switches could be used to input a sequence of 0s and 1s (e.g. you want to make a program that keeps a sum of all the numbers input in binary using switches & display the sum on the LCD). Switches would be better than buttons, because it is very hard to indicate with buttons a continuous state ## TODO REWORD

6c)
void switchConfig() {
	P6SEL &= ~(BIT5|BIT4|BIT3|BIT2); // Set P6.5-2 to digital IO
	P6DIR &= ~(BIT5|BIT4|BIT3|BIT2); // Set P6.5-2 to digital In
	// We don't need to set Pullup resistors
	// because the switch handles that for us.
}

// Outputs the inputs of P6.5-2 as a number 0 to 15
char switchIO() {
	return (P6IN >> 2) & 0x0F; // Shifts P6.5-2 to bits 3-0, and sets other bits to 0.
}
