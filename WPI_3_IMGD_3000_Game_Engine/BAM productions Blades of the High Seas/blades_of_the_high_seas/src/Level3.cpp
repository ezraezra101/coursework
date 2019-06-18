#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Foe.h"
#include "Corporal.h"
#include "Ground.h"
#include "Dolphin.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "ObjectListIterator.h"
#include "Sprite.h"
#include "GraphicsManager.h"
#include "WorldManager.h"

Level3::Level3() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_deck");
	if(!sprite) {
		LOG("Level sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	setPosition(df::Position(x_pos, LEVEL_POSITION));
}

Level3::~Level3() {
		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {
		new Level4();
	}
}

void Level3::spawnFoes() {
	setFoeCount(2);
	// Spawn the correct number of enemies in the proper places
	new Corporal;
	Corporal *f = new Corporal;
	f->setPosition(df::Position(f->getPosition().getX()-40, f->getPosition().getY()));

	for(int i=0;i<RECOMMENDED_DOLPHIN_COUNT;i++) {
		new Dolphin;
	}
}