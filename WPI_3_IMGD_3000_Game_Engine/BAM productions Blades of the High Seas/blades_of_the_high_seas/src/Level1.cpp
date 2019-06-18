#include "Level1.h"
#include "Level2.h"
#include "Foe.h"
#include "Dolphin.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "Sprite.h"
#include "GraphicsManager.h"
#include "WorldManager.h"

Level1::Level1() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("the_gunwale");
	if(!sprite) {
		LOG("Level sprite not found!");
		return;
	} else {
		setSprite(sprite);
	}
	int x_pos = (df::WorldManager::getInstance().getView().getHorizontal() - sprite->getWidth())/2;
	setPosition(df::Position(x_pos, LEVEL_POSITION));

	df::GraphicsManager::getInstance().setBackgroundColor(df::BLUE);
	df::ResourceManager::getInstance().getSprite("ground")->setColor(df::BLACK);
}

Level1::~Level1() {
		// Start next level!
	if(!df::GameManager::getInstance().getGameOver()) {
		new Level2();
	}
}

void Level1::spawnFoes() {
	setFoeCount(1);

	new Foe;

	// Dolphins already created!
}