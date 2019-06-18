#ifndef __OBJECT_LIST_ITERATOR_H__
#define __OBJECT_LIST_ITERATOR_H__

#include "Object.h"
#include "ObjectList.h"

namespace df{

class ObjectList;

class ObjectListIterator {
private:
	const ObjectList *p_list;	// Pointer to Object list iterating over
	int index;					// Index of current item.

	ObjectListIterator();
public:
	
	ObjectListIterator(const ObjectList *p_l);

	//Set the iterator to first item
	void first();

	// Iterate to next item.
	void next();

	// Return true if done iterating, else false.
	bool isDone() const;


	// Return pointer to current Object, NULL if done/empty
	Object *currentObject() const;

};

}
#endif
