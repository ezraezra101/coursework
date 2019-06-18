#include <iostream>
#include <exception>
using namespace std;

//Array example:
class Array
{
public:
	
	friend ostream &operator<<( ostream &, const Array &);
	friend istream &operator>>( istream &, const Array &);
	
	
	Array(int = 10);
	
	//Copy constructor
	Array(const Array &other);
	
	
	~Array()
	{
		delete [] ptr;
	}
	
	int getSize() const
	{
		return size;
	}
	
	bool operator==(const Array& other) const;
	
	bool operator!=(const Array& other) const {return !(*this == other);}
	

	int &operator[]( int );
	
	int operator[]( int ) const;
private:
	int size;
	int *ptr;
};

Array::Array( int length) : size ( length )
{
	ptr = new int[ size ];
}


Array::Array(const Array &other) : size( other.size)
{	
	if( this != &other )
	{
		if(size != other.size)
		{
			delete [] ptr;
			size = other.size;
			ptr = new int[ size ];
		}
		else
			ptr = new int[ size ];
	
	
		for(int i=0; i < size; i++)
		{
			ptr[i] = other.ptr[i];
		}
	}
}//end copy constructor


bool Array::operator==(const Array& other) const
{
	if(! (this->size == other.size))
		return false;
	else
	{
		for(int i=0; i < size; i++)
		{
			if(ptr[i] != other.ptr[i])
				return false;
		}
		return true;
	}
}

int &Array::operator[]( int subscript)
{
	if( subscript >= 0 && subscript < size)
	{
		return *(ptr+subscript);
	}
	else
		throw std::out_of_range("Out of range of this array");
}

int Array::operator[]( int subscript) const
{
	if( subscript >= 0 && subscript < size)
	{
		return *(ptr+subscript);
	}
	else
		throw std::out_of_range("Out of range of this array");
}



ostream &operator<<( ostream &out, const Array &arr)
{
	for(int i=0; i < arr.size; i++)
	{
		out << arr[i] << " ";
	}
	return out;
}
istream &operator>>( istream &in, const Array &arr)
{
	for(int i=0; i < arr.size; i++)
	{
		in >> arr.ptr[i];
	}
	return in;
}


int main()
{
	Array arr(13);
	
	arr[10] = 42;
	
	cout << "Reading out a fairly new array:\n" << arr << endl << "Type in your array:\n";
	
	cin >> arr;
	
	cout << endl << "The new Array is:\n" << arr << endl;
}