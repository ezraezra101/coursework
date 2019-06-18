#include "GraphicsManager.h"
#include "InputManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Clock.h"
#include "GameManager.h"
#include "EventStep.h"


// Mac and linux only
#include <time.h>
static void waitMicroseconds(int u_sec) {
	struct timespec sleep_time;
	sleep_time.tv_sec = u_sec / 1000000;
	sleep_time.tv_nsec= (u_sec % 1000000)*1000;

	nanosleep(&sleep_time, NULL);

	// Windows is just:
	// #include <Windows.h>
	// Sleep(u_sec/1000); // Only sleeps using milliseconds :-(
}


df::GameManager::GameManager() {
	setType("GameManager");

	game_over = false;
	frame_time = df::FRAME_TIME_DEFAULT;
}

df::GameManager::GameManager(df::GameManager const &other) {
	setType("GameManager");

	game_over = other.game_over;
	frame_time = other.frame_time;
}

void df::GameManager::operator=(GameManager const &other) {
	setType("GameManager");

	game_over = other.game_over;
	frame_time = other.frame_time;
}

df::GameManager::~GameManager() {
}

df::GameManager &df::GameManager::getInstance() {
	static df::GameManager *game_manager = new df::GameManager();
	return *game_manager;
}

int df::GameManager::startUp() {
	if(df::LogManager::getInstance().startUp())
		return -1;
	if(df::GraphicsManager::getInstance().startUp())
		return -1;
	if(df::InputManager::getInstance().startUp())
		return -1;
	if(df::WorldManager::getInstance().startUp())
		return -1;
	if(df::ResourceManager::getInstance().startUp())
		return -1;

	df::Box world_boundaries(df::Position(0,0), 
		df::GraphicsManager::getInstance().getHorizontal(),
		df::GraphicsManager::getInstance().getVertical());

	df::WorldManager::getInstance().setView(world_boundaries);

	game_over = false;

	if(df::Manager::startUp())
		return -1;
	

	return 0;
}

void df::GameManager::shutDown() {
	df::ResourceManager::getInstance().shutDown();
	df::InputManager::getInstance().shutDown();
	df::GraphicsManager::getInstance().shutDown();
	df::WorldManager::getInstance().shutDown();
	df::LogManager::getInstance().shutDown();
	setGameOver();
	df::Manager::shutDown();
}

void df::GameManager::run() {
	df::Clock clock;
	df::GraphicsManager &graphics_manager = GraphicsManager::getInstance();
	df::WorldManager &world_manager = df::WorldManager::getInstance();

	int loop_time = 0; // Time between start of loop, and draw to screen.
	int adjust_time = 0; // Inaccuracy between intended_sleep_time and actual_sleep_time.

	int loop_count = 0; // Number of times we've passed through the loop

	while(!game_over) {
		clock.delta();
		// Get input
		df::InputManager::getInstance().getInput();

		// Update world state
		df::EventStep step_event = df::EventStep(loop_count);
		onEvent(&step_event); // Send step event to all Objects.
		world_manager.update();
		loop_count++;

		// Draw current scene to back buffer
		world_manager.draw();

		// Swap back buffer to current buffer
		graphics_manager.swapBuffers();


		// Sleep appropriate amount of time
		loop_time = clock.split();
		int intended_sleep_time = frame_time*1000 - loop_time - adjust_time;
		clock.delta();
		if(intended_sleep_time > 0) {
			waitMicroseconds(intended_sleep_time);
		}
		int actual_sleep_time = clock.split();
		adjust_time = actual_sleep_time - intended_sleep_time;	}
}

void df::GameManager::setGameOver(bool new_game_over) {
	game_over = new_game_over;
}

bool df::GameManager::getGameOver() const {
	return game_over;
}

int df::GameManager::getFrameTime() const {
	return frame_time;
}
