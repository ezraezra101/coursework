#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Color.h"
#include "Object.h"
#include "EventKeyboard.h"
#include "EventOut.h"
#include "EventCollision.h"

#define SOUND_FOLDER "unit_tests/sounds/"

int global_collision_count = 0;
int global_out_count = 0;

class Mobile : public df::Object {
	df::Color color;
public:
	Mobile() {
		setType("Mobile");
		setXVelocity(-0.25);
		setSolidness(df::HARD);
		color = df::COLOR_DEFAULT;
	}
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::OUT_EVENT) {
			LOG("Out! %d %f \n", getId(), getXVelocity());
			df::ResourceManager::getInstance().getSound("fire")->play();
			// Bounce!
			setXVelocity(getXVelocity() * -0.8);
			return 1;
		} else if (p_e->getType() == df::COLLISION_EVENT) {
			LOG("Collision! %d %f \n", getId(), getXVelocity());
			
			// Bounce fast!
			setXVelocity(getXVelocity() * -1.2);
			return 1;
		} else {
			return df::Object::eventHandler(p_e);
		}
	}

	void setColor(df::Color c) {
		color = c;
	}

	void draw() {
		df::GraphicsManager & gm = df::GraphicsManager::getInstance();
		gm.drawCh(getPosition(), '*', color);

	}
};

class PressButtonToQuit : public df::Object {

public:
	PressButtonToQuit() {
		setType("PressButtonToQuit");
		setPosition(df::Position(30,10));
	}
	int eventHandler(const df::Event *p_e) {
		if(p_e->getType() == df::KEYBOARD_EVENT) {
			df::GameManager::getInstance().setGameOver();
			return 1;
		} else {
			return df::Object::eventHandler(p_e);
		}
	}

	void draw() {
		df::GraphicsManager & gm = df::GraphicsManager::getInstance();
		gm.drawString(getPosition(), "Press any button to quit!", df::LEFT_JUSTIFIED, df::BLUE);
	}
};

int main() {
	printf("Running velocity test..."); fflush(stdout);
	df::GameManager &game_manager = df::GameManager::getInstance();
	game_manager.startUp();

	df::ResourceManager::getInstance().loadSound(SOUND_FOLDER "fire.wav", "fire");
	df::ResourceManager::getInstance().loadMusic(SOUND_FOLDER "start-music.wav", "gamestart");

	for(int i=0; i<30; i++) {
		Mobile *p_o = new Mobile();
		p_o->setPosition(df::Position(rand() % 80, rand() % 24));
		p_o->setColor((df::Color) (rand() % 5));
	}
	new PressButtonToQuit();

	df::WorldManager &world_manager = df::WorldManager::getInstance();

	df::ResourceManager::getInstance().getMusic("gamestart")->play();
	game_manager.run();

	game_manager.shutDown();

	printf("Done.\n");

	return 0;
}