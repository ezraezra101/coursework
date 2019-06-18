#include <stdio.h>

#include "Box.h"

int main() {
	int failed_tests = 0;

	df::Position p(1,2);
	df::Box b(p, 3,4), b_default;

	if(b_default.getCorner().getX() != 0 ||
		b_default.getHorizontal() != 0 ||
		b_default.getVertical() != 0) {
		printf("Box default attributes aren't properly set.\n");
		failed_tests++;
	}

	if(b.getCorner().getX() != p.getX() ||
		b.getHorizontal() != 3 ||
		b.getVertical() != 4) {
		printf("Box constructor doesn't work.\n");
		failed_tests++;
	}

	b.setCorner(df::Position());
	b.setHorizontal(5);
	b.setVertical(6);

	if(b.getCorner().getX() != 0 ||
		b.getHorizontal() != 5 ||
		b.getVertical() != 6) {
		printf("Box getters/setters don't work.\n");
		failed_tests++;
	}

	return failed_tests;
}