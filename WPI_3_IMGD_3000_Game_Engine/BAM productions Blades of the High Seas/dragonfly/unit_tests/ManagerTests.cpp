#include "Manager.h"

#include <string>

int main() {
	int failed_tests = 0;

	df::Manager manager;

	if(manager.getType() != df::UNDEFINED_MANAGER) {
		failed_tests++;
		printf("Manager::getType() isn't properly initialized.\n");
	}

	if(manager.isStarted()) {
		failed_tests++;
		printf("Manager thinks it's started before startup!\n");
	}
	if(manager.startUp()) {
		failed_tests++;
		printf("Manager didn't startup properly!\n");
	}

	manager.shutDown(); // No segfaulting, hopefully!

	// Manager::onEvent is tested by GameManager and integration tests.

	return failed_tests;
}