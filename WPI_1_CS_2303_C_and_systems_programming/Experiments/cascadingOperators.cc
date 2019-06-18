
/*
If a function returns the reference to the this pointer (return *this;)
	it allows cascading of operators
		instance.func().func2(); //So long as fun returns *this

*/


#include <iostream>
using namespace std;

class Test
{
	int x;
	
	
public:
	Test(int val = 0) : x(val) {}
	
	const Test &print() const {cout << x; return *this; }

	Test &setX(int x)
		{ this->x = x; return *this;	}
	

};



int main()
{
	
	Test t(5);
	
	t.print();
	
	t.setX(1).print();
	
	t.print().print();
	//Doesn't work because of non-const:
	//t.print().setX(5);
}