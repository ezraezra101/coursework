#include <stdio.h>

#include "Position.h"

int main() {

	int failed_tests = 0;

	df::Position pos;

	if(pos.getX() || pos.getY()) {
		printf("Position default (x,y) position isn't (0,0)\n");
		failed_tests++;
	}
	pos.setXY(1,2);
	if(pos.getX() != 1 || pos.getY() != 2) {
		printf("Position setXY isn't working\n");
		failed_tests++;
	}

	df::Position pos2 = df::Position(3,4);
	if(pos2.getX() != 3 || pos2.getY() != 4) {
		printf("Position constructor is broken.\n");
		failed_tests++;
	}

	pos2.setX(5);
	if(pos2.getX() != 5 || pos2.getY() != 4) {
		printf("Position setX is broken.\n");
		failed_tests++;
	}

	pos2.setY(7);
	if(pos2.getX() != 5 || pos2.getY() != 7) {
		printf("Position setY is broken.\n");
		failed_tests++;
	}

	return failed_tests;
}