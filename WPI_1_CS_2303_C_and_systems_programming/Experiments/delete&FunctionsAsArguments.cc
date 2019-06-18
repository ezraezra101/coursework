#include <iostream>
using namespace std;
class bad
{
	
	public:
	int x;

	bad()
	{
		x = 5;
		cout << "deleting the this pointer in the constructor.\n";
		delete this;
		cout << "the this pointer is still: " << this << endl;
	}
	
	void remove()
	{
		delete this;
		//return *this;
	}
	
	
};



template<class T>
int sum(T x, T y)
{
	return x+y;
}

template <class Y>
Y metaFunc(Y (*function)(Y, Y), Y x, Y y)
{
	return function(x,y);
}

int main()
{
	
	bad *x = new bad();
	
	cout << "\nThe object still exists. Here is it's member's value: " << x->x << endl << endl;
	
	//Breaks it!
	//x->remove();
	
	//cout << x->x << endl;
	
	
	
	cout << "Running a template function that uses a function as an argument:" << metaFunc(sum, 1, 2);
}
