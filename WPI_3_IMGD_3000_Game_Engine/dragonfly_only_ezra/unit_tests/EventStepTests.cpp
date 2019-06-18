#include <stdio.h>

#include "EventStep.h"

int main() {

	int failed_tests = 0;

	df::EventStep step = df::EventStep(42), defaultStep = df::EventStep();

	if(step.getStepCount() != 42) {
		printf("EventStep initialization isn't working...\n");
		failed_tests++;
	}

	step.setStepCount(2);
	if(step.getStepCount() != 2) {
		printf("EventStep setting isn't working\n");
		failed_tests++;
	}

	return failed_tests;
}