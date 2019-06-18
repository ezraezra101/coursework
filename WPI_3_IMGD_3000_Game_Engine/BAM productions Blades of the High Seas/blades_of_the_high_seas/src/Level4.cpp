#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Foe.h"
#include "Corporal.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "Sprite.h"
#include "WorldManager.h"

Level4::Level4() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_brig");
	if(!sprite) {
		LOG("Level sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	setPosition(df::Position(x_pos, LEVEL_POSITION));
	
	df::GraphicsManager::getInstance().setBackgroundColor(df::BLACK);
	df::ResourceManager::getInstance().getSprite("ground")->setColor(df::WHITE);
}

Level4::~Level4() {
		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {
		new Level5();
	}
}

void Level4::spawnFoes() {
	setFoeCount(4);
	// Spawn the correct number of enemies in the proper places
	new Corporal;
	Corporal *f = new Corporal;
	f->setPosition(df::Position(f->getPosition().getX()-60, f->getPosition().getY()));
	Foe *c = new Foe;
	c->setPosition(df::Position(c->getPosition().getX()-40, c->getPosition().getY()));
	Foe *e = new Foe;
	e->setPosition(df::Position(e->getPosition().getX()-20, e->getPosition().getY()));
}