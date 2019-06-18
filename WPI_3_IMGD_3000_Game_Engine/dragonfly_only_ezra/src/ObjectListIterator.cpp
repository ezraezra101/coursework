#include "ObjectListIterator.h"

df::ObjectListIterator::ObjectListIterator() {
	p_list = NULL;
	index = 0;
}

df::ObjectListIterator::ObjectListIterator(const ObjectList *p_l) {
	p_list = p_l;
	index = 0;
}

//Set the iterator to first item
void df::ObjectListIterator::first() {
	index = 0;
}

// Iterate to next item.
void df::ObjectListIterator::next() {
	if(!isDone())
		index++;
}

// Return true if done iterating, else false.
bool df::ObjectListIterator::isDone() const {
	return p_list == NULL || p_list->count <= index;
}

// Return pointer to current Object, NULL if done/empty
df::Object *df::ObjectListIterator::currentObject() const {
	if(p_list == NULL || p_list->count <= index) {
		return NULL;
	}

	return p_list->list[index];
}
