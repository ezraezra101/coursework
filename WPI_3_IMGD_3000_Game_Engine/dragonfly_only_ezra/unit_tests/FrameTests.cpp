#include <stdio.h>
#include <string>
#include "Frame.h"

int main() {

	int failed_tests = 0;

	df::Frame f = df::Frame(1,1, "X"), f_default = df::Frame();

	if(f.getWidth() != 1 || f.getHeight() != 1 || f.getString() != "X") {
		printf("Complex Frame constructor doesn't work!\n");
		failed_tests++;
	}
	if(f_default.getWidth() != 0|| f_default.getHeight() != 0 || f_default.getString() != "") {
		printf("Default Frame constructor doesn't work!\n");
		failed_tests++;
	}

	f.setWidth(4);
	f.setHeight(3);
	f.setString("123456789012");


	if(f.getWidth() != 4 || f.getHeight() != 3 || f.getString() != "123456789012") {
		printf("Frame setters/getters don't work!\n");
		failed_tests++;
	}

	return failed_tests;
}