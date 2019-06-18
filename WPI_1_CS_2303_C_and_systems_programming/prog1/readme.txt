
Author:				Ezra Davis (edavis)
Date:				January 22 2014
Version:			1.0
Project ID:			Program 1: Functions and basic variable types in C
CS Class:			CS2303
Programming Language:		C
OS/Hardware dependencies:	Compiles and runs C. Also uses math.h and stdio.h
				
Problem Description:		This program takes pairs of points, finds the distance between them and averages those distances.
							Next it computes the log(average)/log(2) and graphs the value
							see: http://web.cs.wpi.edu/~rek/Systems/C14/Program1_C14.pdf

Overall Design:			
	System structure	N/A
	Data representation	Simple ints and doubles.
	Algorithms 		Average = (sum of numbers) / (number of numbers)
					Whether a number is 2 less than a quarter point of another number = (Anumber % (AnotherNumber)%4 -2)) == 0)

Program Assumptions and Restrictions:
					Assumes a textbased input given according to prompts.

Interfaces:			
	User			Command line & text prompts
	File/D-B		N/A
	Program/Module	None

Source files:		 prog1.c

How to build the program:
					gcc prog1.c -o prog1 -lm
