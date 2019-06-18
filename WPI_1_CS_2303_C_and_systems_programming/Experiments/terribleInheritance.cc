#include <iostream>
using namespace std;

class X;
class Q
{
public:
	int aVal;
	
	//Called twice in this example:
	Q(){cout << "In Q's constructor\n";}
	Q(int val) : aVal( val )
		{cout << "In Q's constructor, the kind called from Y\n";}
	
	
	void print(){cout << "This is the other base, Q.\n";}
};

class Y : public Q
{
public:
	
	Y() : Q(5) {}
	
	virtual void print(){cout << "im in yr base (Y)\n";}
};


class X : public Q, public Y
{
public:
	void print() {cout << "I'm in the derived class, X.\n";}
};

int main()
{
	X aX;
	
	Y &aXY = aX;
	
	cout << "Printing class X: (which inherits from Y)\n\t";
	aX.print();
	
	cout << "Printing class X cast as class Y:\n\t";
	aXY.print();
	
	cout << "Printing class X's Y::print() function:\n\t";
	aX.Y::print();
	
	cout << aX.Y::aVal;
	//cout << aX.Q::aVal; //ambiguous
	
}