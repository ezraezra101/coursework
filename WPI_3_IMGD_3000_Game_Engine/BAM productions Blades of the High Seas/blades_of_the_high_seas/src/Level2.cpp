#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Foe.h"
#include "Corporal.h"
#include "Dolphin.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "Sprite.h"
#include "WorldManager.h"

Level2::Level2() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_forecastle");
	if(!sprite) {
		LOG("Level sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	setPosition(df::Position(x_pos, LEVEL_POSITION));
}

Level2::~Level2() {
		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {
		new Level3();
	}
}

void Level2::spawnFoes() {
	setFoeCount(2);
	// Spawn the correct number of enemies in the proper places
	new Corporal;
	Foe *f = new Foe;
	f->setPosition(df::Position(f->getPosition().getX()-40, f->getPosition().getY()));

	for(int i=0;i<RECOMMENDED_DOLPHIN_COUNT;i++) {
		new Dolphin;
	}
}