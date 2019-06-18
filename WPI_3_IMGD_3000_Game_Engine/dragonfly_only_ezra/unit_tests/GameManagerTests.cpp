#include <stdio.h>

#include "Clock.h"
#include "GameManager.h"
#include "LogManager.h"

int main() {

	int failed_tests = 0;

	df::GameManager &game_manager = df::GameManager::getInstance();

	if(game_manager.startUp()) {
		printf("GameManager didn't successfully startup!\n");
		failed_tests++;
	}
	if(!df::LogManager::getInstance().isStarted()) {
		printf("GameManager didn't start LogManager!\n");
		failed_tests++;
	}

	if(game_manager.getGameOver()) {
		printf("GameManager's game is over right after starting.\n");
		failed_tests++;
	}

	game_manager.setGameOver();
	if(!game_manager.getGameOver()) {
		printf("GameManager::setGameOver didn't work!\n");
		failed_tests++;
	}

	game_manager.run(); // Should exit immediately - game is over.

	// Test run!
	// I did some more tests of run(), but they were very brittle and needed code in GameManager.cpp.
	// run() will have to be tested in the integration tests.

	if(game_manager.getFrameTime() != df::FRAME_TIME_DEFAULT) {
		printf("GameManager's frame time isn't the default.\n");
		failed_tests++;
	}

	return failed_tests;
}