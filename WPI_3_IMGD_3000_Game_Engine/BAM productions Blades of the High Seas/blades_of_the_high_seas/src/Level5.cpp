
#include "Level5.h"
#include "LevelWin.h"
#include "Foe.h"
#include "Corporal.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "Sprite.h"
#include "WorldManager.h"

Level5::Level5() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_hold");
	if(!sprite) {
		LOG("Level sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	setPosition(df::Position(x_pos, LEVEL_POSITION));
}

Level5::~Level5() {
		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {
		new LevelWin();
	}
}

void Level5::spawnFoes() {
	setFoeCount(4);
	// Spawn the correct number of enemies in the proper places
	new Corporal;
	Corporal *f = new Corporal;

	f->setPosition(df::Position(f->getPosition().getX()-48, f->getPosition().getY()));
	Corporal *c = new Corporal;
	c->setPosition(df::Position(c->getPosition().getX()-24, c->getPosition().getY()));

	Foe *e = new Foe;
	c->setPosition(df::Position(e->getPosition().getX()-36, e->getPosition().getY()));
}