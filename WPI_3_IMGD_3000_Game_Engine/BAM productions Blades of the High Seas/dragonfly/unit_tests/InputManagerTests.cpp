#include "GraphicsManager.h"
#include "InputManager.h"
#include "LogManager.h"

#include <string>

int main() {
	df::LogManager &lm = df::LogManager::getInstance();
	lm.startUp();
	df::InputManager &im = df::InputManager::getInstance();

	int failed_tests = 0;

	if(!im.startUp()) {
		printf("InputManager started up improperly (without GraphicsManager).\n");
		failed_tests++;
	}

	df::GraphicsManager &gm = df::GraphicsManager::getInstance();
	gm.startUp();

	if(im.startUp()) {
		failed_tests++;
		printf("InputManager didn't startup properly!\n");
	}

	// getInput() is tested in integration_tests.

	im.shutDown(); // No segfaulting, hopefully!


	return failed_tests;
}