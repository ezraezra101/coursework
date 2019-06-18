#ifndef __OBJECT_LIST_H__
#define __OBJECT_LIST_H__

#include "Object.h"
#include "ObjectListIterator.h"

class ObjectListIterator;

namespace df {

const int MAX_OBJECTS = 5000;

class ObjectList {
private:
	int count;
	Object *list[MAX_OBJECTS];
public:
	friend class ObjectListIterator;

	// Default constructor.
	ObjectList();

	// Instert object pointer in list
	// Return 0 if ok, else -1.
	int insert(Object *p_o);

	// Remove object pointer from list.
	// Return 0 if found, else -1.
	int remove(Object *p_o);

	// Clear list (setting count to 0)
	void clear();

	// Return count of number of objects in list.
	int getCount() const;

	// Return true if list is empty, else false.
	bool isEmpty() const;

	// Return true if list is full, else false.
	bool isFull() const;
};

}
#endif
