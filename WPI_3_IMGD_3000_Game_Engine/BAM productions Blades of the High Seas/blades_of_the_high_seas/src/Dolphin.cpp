#include "EventOut.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

#include "Dolphin.h"
#include "EventLevelUp.h"
#include "Ground.h"

void Dolphin::out() {
	setPosition(df::Position(0, rand() % GROUND_POSITION));
	setXVelocity((float) (4.0/(rand() % 10 + 1)));
}


Dolphin::Dolphin() {
	setType("Dolphin");
	setSolidness(df::SPECTRAL);
	setAltitude(0);
	df::Sprite *sprite = df::ResourceManager::getInstance().getSprite("dolphin");
	if(!sprite) {
		LOG("Dolphin sprite not found!");
		return;
	} else {
		setSprite(sprite);
		setSpriteSlowdown(4);
		setSpriteIndex(rand() % 4);
	}
	
	out();	
}

int Dolphin::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	} else if(p_e->getType() == LEVEL_UP_EVENT) {
		df::WorldManager::getInstance().markForDelete(this);
	}
	return 0;
}