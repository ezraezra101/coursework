#include "Ground.h"

#include "ResourceManager.h"
#include "LogManager.h"

Ground::Ground() {
	setType("Ground");

	setSolidness(df::HARD);

	setPosition(df::Position(0,GROUND_POSITION));

	df::Sprite *p_temp_sprite = df::ResourceManager::getInstance().getSprite("ground");
	if(p_temp_sprite == NULL) {
		LOG("Ground::Ground sprite 'ground' not found");
	} else {
		setSprite(p_temp_sprite);
	}
}
