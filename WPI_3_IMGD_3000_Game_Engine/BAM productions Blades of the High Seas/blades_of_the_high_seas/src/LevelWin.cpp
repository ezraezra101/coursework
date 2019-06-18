#include "LevelWin.h"

#include "ResourceManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "Sprite.h"
#include "WorldManager.h"

LevelWin::LevelWin() {
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("winrar");
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

	setLevelCountdown(10000);
	setSpriteSlowdown(20);
}

LevelWin::~LevelWin() {
	// Game is Over!	
}

void LevelWin::spawnFoes() {
	df::GameManager::getInstance().setGameOver();
}

void LevelWin::spawnHeroes() {
	// Do nothing!
}