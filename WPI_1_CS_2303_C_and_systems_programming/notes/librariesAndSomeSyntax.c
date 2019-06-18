
C uses a program execution stack
	calling too many functions causes a "stack overflow error"

<stdlib.h>
	has functions for converting numbers to text, memory allocation, random numbers, and other utilities

<string.h>
	Does string processing

<time.h>
	guess

Custom header files use: #include "filename.h"


rand (from stdlib.h)

i = rand();
		returns a 'random' number between 0 and RAND_MAX
		srand() - uses system clock

To get a random number between 1 and n:
	i = 1 + (rand() % n);


Call by Value:
	When arguments are passed by the calling routine to the called routine by value,
		A copy of the argument is passed, NOT the argument itself
Call by Reference:
	The original argument is passed to the called routine
		Any changes to the argument in the called function has an effect on the variable after the function was called





			static variables work in C