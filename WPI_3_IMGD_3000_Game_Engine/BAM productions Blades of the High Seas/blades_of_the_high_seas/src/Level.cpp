
#include "EventKeyboard.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "WorldManager.h"

#include "EventLevelUp.h"
#include "Level.h"
#include "Ground.h"
#include "Hero.h"
#include "Hero1.h"
#include "Hero2.h"
#include "Foe.h"
#include "Corporal.h"

Level *Level::current_level = NULL;

Level *Level::getCurrentLevel() {
	if(current_level == NULL) {
		current_level = new Level();
	}
	return current_level;
}


Level::Level() {
	
	// Set its position & load sprite
	// df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_forecastle");
	// if(!sprite) {
	// 	LOG("Level sprite not found!");
	// 	return;
	// } else {
	// 	setSprite(sprite);
	// }
	// int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	// setPosition(df::Position(x_pos, LEVEL_POSITION));
	
	setSolidness(df::SPECTRAL);
	setAltitude(df::MAX_ALTITUDE-1);


	// Variable housekeeping
	hero1 = NULL;
	hero2 = NULL;
	alive_foe_count  = 0; // Changed by different levels

	level_countdown = 30;

	current_level = this;
}


Level::~Level() {

		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {

		EventLevelUp e;
		df::WorldManager::getInstance().onEvent(&e);
	}
}

void Level::spawnHeroes() {
	// Spawn the heroes
	hero1 = new Hero1;
	hero2 = new Hero2;

	hero1->setPosition(df::Position(16, GROUND_POSITION-20));
	hero2->setPosition(df::Position(32, GROUND_POSITION-20));
}

void Level::setFoeCount(int cnt) {
	alive_foe_count = cnt;
}

void Level::setLevelCountdown(int cnt) {
	level_countdown = cnt;
}

void Level::spawnFoes() {
}

int Level::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard *ke = static_cast<const df::EventKeyboard *>(p_e);
		// If we press ESC, quit game
		if(ke->getKey() == df::Input::ESCAPE) {
			df::GameManager::getInstance().setGameOver();
		}
		return 1;
	} else if(p_e->getType() == df::STEP_EVENT) {
		level_countdown--;

		if(level_countdown == 0) {
			spawnHeroes();
			spawnFoes();
			df::Sprite *s= new df::Sprite(1); // Not at all problematic...
			setSprite(s);
		}

		if(not hero1 and not hero2 && level_countdown <= 0) {
			// End game!
			df::GameManager::getInstance().setGameOver();
		}

		return 1;
	}

	return 0;
}

void Level::heroHasDied(Hero *h) {
	if(h == hero1) {
		hero1 = NULL;
	} else if(h == hero2) {
		hero2 = NULL;
	}
}

void Level::decrementFoeCount() {
	alive_foe_count--;
	if(alive_foe_count <= 0) {
		// Go to next level
		df::WorldManager::getInstance().markForDelete(this);
	}
}