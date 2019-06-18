#include "GraphicsManager.h"
#include "LogManager.h"

#include <string>

int main() {
	df::LogManager &lm = df::LogManager::getInstance();
	lm.startUp();

	int failed_tests = 0;

	df::GraphicsManager &gm = df::GraphicsManager::getInstance();

	if(gm.startUp()) {
		failed_tests++;
		printf("GraphicsManager didn't startup properly!\n");
	}

	// DrawCh and drawString are tested elsewhere

	if( gm.getHorizontal() != df::WINDOW_HORIZONTAL_CHARS_DEFAULT ||
		gm.getVertical()   != df::WINDOW_VERTICAL_CHARS_DEFAULT   ||
		gm.getHorizontalPixels() != df::WINDOW_HORIZONTAL_PIXELS_DEFAULT ||
		gm.getVerticalPixels()   != df::WINDOW_VERTICAL_PIXELS_DEFAULT   ) {
		failed_tests++;
		printf("GraphicsManager doesn't have proper dimensions set!\n");
	}

	gm.shutDown(); // No segfaulting, hopefully!


	return failed_tests;
}