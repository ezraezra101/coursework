C++

TEMPLATES:
	Templates are like Java generics.
	
	eg:
#include <iostream>

template <class T, typename X> //For the most part, typename and class are interchangable here
T identity(T x, X y)
{	std::cout << y << std::endl;
	return x;}

int main(){
	std::cout << identity(5, 5.2);
}



CLASSES:
	have member functions and data members
		
	public:, private:, friend:, & protected:
	
	Classes can be allocated simply with: //calls the default constructor
	Classname instance;
	//You can then do stuff like this:
	instance.memberFunction();
	instance.publicData = 5;
	
	
	a default constructor uses has no arguments.
	Constructors return nothing (not even void)
		
		If a Class does not have a constructor, the compiler will make one
	
	
	
Getlines:
	cin.getline is different than just getline:
	
	getline(cin, stringObjectName);
	cString = cin.getline(MaxLenth);
	

Traditionally, there are getter and setter functions for private variables.