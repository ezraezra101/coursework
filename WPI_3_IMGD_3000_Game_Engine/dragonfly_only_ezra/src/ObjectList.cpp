#include "ObjectList.h"

df::ObjectList::ObjectList() {
	count = 0;
}

int df::ObjectList::insert(Object *p_o) {
	if(count == df::MAX_OBJECTS)
		return -1;

	list[count] = p_o;
	count++;
	return 0;
}

int df::ObjectList::remove(Object *p_o) {
	for(int i=0; i<count; i++) {
		if(list[i] == p_o) {
			for(int j=i; j<count-1; j++) {
				list[j] = list[j+1];
			}
			count--;
			return 0;
		}
	}
	return -1;
}

void df::ObjectList::clear() {
	count = 0;
}

int df::ObjectList::getCount() const {
	return count;
}

bool df::ObjectList::isEmpty() const {
	return count == 0;
}

bool df::ObjectList::isFull() const {
	return count == df::MAX_OBJECTS;
}
