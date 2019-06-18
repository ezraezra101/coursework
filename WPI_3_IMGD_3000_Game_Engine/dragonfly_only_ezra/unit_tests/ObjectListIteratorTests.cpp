#include <stdio.h>

#include "ObjectListIterator.h"

int main() {

	int failed_tests = 0;

	df::ObjectList list = df::ObjectList();
	df::Object o1 = df::Object();
	df::Object o2 = df::Object();
	list.insert(&o1);
	list.insert(&o2);

	df::ObjectListIterator iter = df::ObjectListIterator(&list);

	if(iter.currentObject() != &o1) {
		printf("ObjectListIterator didn't start with the first list element.\n");
		failed_tests++;
	}
	iter.next();
	if(iter.currentObject() != &o2) {
		printf("ObjectListIterator::next() doesn't work.\n");
		failed_tests++;
	}
	iter.next();
	if(!iter.isDone()) {
		printf("ObjectListIterator isn't done when it should be.\n");
		failed_tests++;
	}

	iter.first();
	if(iter.currentObject() != &o1) {
		printf("ObjectListIterator::first() didn't work.\n");
		failed_tests++;
	}

	iter.currentObject()->setPosition(df::Position(-25, 25));
	if(o1.getPosition().getX() != -25) {
		printf("ObjectListIterator didn't modify object in list properly! (%d)", o1.getPosition().getX());
		failed_tests++;
	}

	df::ObjectList empty_list = df::ObjectList();
	df::ObjectListIterator empty_iter = df::ObjectListIterator(&empty_list);
	if(!empty_iter.isDone() || empty_iter.currentObject() != NULL) {
		printf("ObjectListIterator doesn't work when empty.\n");
		failed_tests++;
	}

	return failed_tests;
}