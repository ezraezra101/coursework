#include <iostream>
using namespace std;

//A terrifying way to do trees.
// emptyTreeInstance->insert(stuff) behaves oddly...
//Its also missing destructors :-(

//Uses generics & an Abstract class

template <class T>
class Tree {
public:
	virtual ~Tree() {}
	virtual T getData()=0;
	
	virtual Tree<T> *insert(T newData) {return 0;};
	
	virtual void inOrderTraverse(void (*function)(T)) {};
};

template <class T>
class EmptyTree;


template <class T>
class DataTree : public Tree<T> {
	T data;
	Tree<T> *lchild;
	Tree<T> *rchild;
public:
	DataTree(T newData)
	{
		data = newData;
		lchild = new EmptyTree<T>();
		rchild = new EmptyTree<T>();
	}

	T getData()	{return data;}

	//Recursive insertion of a new piece of data
	Tree<T> *insert(T newData)
	{
		if(newData > data){
			rchild =( rchild->insert(newData));
			return this;
		}//end greaterthan if
		else if(newData < data){
				lchild =( lchild->insert(newData));
				return this;
		}//end lessthan if
		return (Tree<T>*) this;
	}//end insert


	void inOrderTraverse( void (*function)(T) )
	{
		lchild->inOrderTraverse(function);
		function(data);
		rchild->inOrderTraverse(function);
	}

};//end Tree<T>

template <class T>
class EmptyTree : public Tree<T> {
	
public:
	
	EmptyTree(){};
	
	T getData() {cout << "attempted to get the value of an empty tree" << endl; return 0;};
	
	Tree<T> *insert(T newData){return new DataTree<T>(newData);}
};

template <typename T>
void printThing(T x)
{
	cout << x << endl;
}

int main()
{
	Tree<int> *x = new EmptyTree<int>();
	
	cout << x->getData() << endl << endl;
	
	x = x->insert(3);
	
	x = x->insert(7);
	
	x->inOrderTraverse( printThing );
	
	DataTree<char> y = DataTree<char>('o');
	
	y.insert('r');
	y.insert('p');
	y.insert('q');
	
	int i;
	for(i=0; i < 26; i++){
		y.insert('Z'-i);
	}
	
	y.inOrderTraverse( printThing );
}