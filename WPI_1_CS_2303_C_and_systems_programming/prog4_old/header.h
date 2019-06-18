#ifndef HEADER_DEFINED

#define HEADER_DEFINED

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using std::cin;
using std::cout;
using std::fstream;
using std::string;
using std::endl;

#define SCREEN_SIZE 80
#define COUNT_INDENT 25
#define LINES_INDENT 10

//Gets the number of digits of a number
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
