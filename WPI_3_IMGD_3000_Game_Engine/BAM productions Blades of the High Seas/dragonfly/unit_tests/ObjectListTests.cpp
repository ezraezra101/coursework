#include <stdio.h>

#include "ObjectList.h"

int main() {

	int failed_tests = 0;	

	df::Object *p_o = new df::Object();
	df::Object *p_o2 = new df::Object();
	df::ObjectList list = df::ObjectList();

	if(list.getCount() != 0) {
		printf("ObjectList length not initialized to 0!\n");
		failed_tests++;
	}
	if(!list.isEmpty()) {
		printf("ObjectList not initialized to empty!\n");
		failed_tests++;
	}
	if(list.isFull()) {
		printf("ObjectList is initialized to full!\n");
		failed_tests++;
	}

	if(list.insert(p_o)) {
		printf("ObjectList insert failed.\n");
		failed_tests++;
	}
	if(list.getCount() != 1 || list.isEmpty()) {
		printf("ObjectList insert left list empty!\n");
		failed_tests++;
	}
	if(list.remove(p_o)) {
		printf("ObjectList unable to remove pointer.\n");
		failed_tests++;
	}
	if(!list.isEmpty() || list.getCount() != 0) {
		printf("ObjectList didn't remove object!\n");
		failed_tests++;
	}

	// Testing lists with length 2
	if(list.insert(p_o) || list.insert(p_o2)) {
		printf("ObjectList unable to insert pointer!\n");
		failed_tests++;
	}
	if(list.remove(p_o)) {
		printf("ObjectList unable to remove pointer from list of length 2.\n");
		failed_tests++;
	}
	if(!list.remove(p_o)) {
		printf("ObjectList removed non-existent pointer.\n");
		failed_tests++;
	}
	if(list.getCount() != 1 || list.isEmpty()) {
		printf("ObjectList didn't remove item properly");
		failed_tests++;
	}

	list.clear();
	if(!list.isEmpty() || list.getCount() != 0) {
		printf("ObjectList didn't clear\n");
		failed_tests++;
	}

	// Tests on huge list
	for(int i=0; i< df::MAX_OBJECTS/2-1; i++) {
		if(list.insert(p_o) || list.insert(p_o2)) {
			printf("ObjectList unable to insert pointer!\n");
			failed_tests++;
		}
	}
	if(list.getCount() != df::MAX_OBJECTS - 2 || list.isEmpty()) {
		printf("ObjectList didn't count up properly. (%d)\n", list.getCount());
		failed_tests++;
	}
	if(list.insert(p_o) || list.insert(p_o2) || !list.isFull() || list.getCount() != df::MAX_OBJECTS) {
		printf("ObjectList didn't fill up properly. %d\n", list.getCount());
		failed_tests++;
	}
	if(!list.insert(p_o)) {
		printf("ObjectList inserted into full list\n");
		failed_tests++;
	}
	if(list.remove(p_o) || list.isFull() || list.getCount() != df::MAX_OBJECTS-1) {
		printf("ObjectList unable to remove pointer from large list.\n");
		failed_tests++;
	}

	list.clear();
	if(!list.isEmpty() || list.getCount() != 0) {
		printf("huge ObjectList didn't clear\n");
		failed_tests++;
	}

	return failed_tests;
}