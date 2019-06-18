#include "EventOut.h"

int main() {

	int failed_tests = 0;

	df::EventOut oe;

	if(oe.getType() != df::OUT_EVENT) {
		printf("Out event doesn't set it's type!\n");
		failed_tests++;
	}

	// Sending EventOuts is tested in the integration test for collisions.

	return failed_tests;
}