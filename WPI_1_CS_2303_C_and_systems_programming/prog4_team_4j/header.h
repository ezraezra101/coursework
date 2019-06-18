//Team 4J: Ezra Davis & Artian Kica

#ifndef HEADER_DEFINED //if header is not yet defined, define it. Otherwise, ignore.

#define HEADER_DEFINED

#include <iostream> //standard input/output
#include <fstream>	//for file input arg
#include <string>   //for string manipulation
#include <iomanip>  //Used for formatting the output
using std::cin;		//input
using std::cout;	//output
using std::fstream; //file stream
using std::string;
using std::endl;	//new line

//Decreasing any of these may cause segfaults :-)
//Written by Ezra Davis
#define SCREEN_SIZE 80
#define COUNT_INDENT 25
#define LINES_INDENT 10

//Gets the number of digits of a number.
//Written by Ezra Davis & Artian Kica
inline unsigned short digits(unsigned int in)
{
	unsigned short i = 0;
	unsigned int x = in;
	while(x >= 1){
		x /= 10;
		i++;
	}
	//cout <<endl<<"i:" << i << endl;
	
	return i;
}//end digits

#endif
