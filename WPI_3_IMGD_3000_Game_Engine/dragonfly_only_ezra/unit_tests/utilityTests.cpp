#include <stdio.h>

#include "utility.h"


int main() {

	int failed_tests = 0;

	df::Position x(1,2), y(1,2), alpha(1,3);

	if(!df::positionsIntersect(x, y) || df::positionsIntersect(y,alpha) || df::positionsIntersect(alpha,x)) {
		printf("positionsIntersect in utility.cpp doesn't work!\n");
		failed_tests++;
	}

	df::Box a(df::Position(1,1), 0,0), b(df::Position(0,0), 1,1), c(df::Position(2,2), 100, 100);

	if(!df::boxIntersectsBox(a, b) ||
		df::boxIntersectsBox(b, c)) {
		printf("boxIntersectsBox in utility.cpp doesn't work!\n");
		failed_tests++;
	}

	return failed_tests;
}