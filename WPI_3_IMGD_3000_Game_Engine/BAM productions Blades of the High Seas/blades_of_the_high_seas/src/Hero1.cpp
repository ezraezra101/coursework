#include "Hero1.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "LogManager.h"

Hero1::Hero1() {
	standing_sprite = "pirate1";
	moving_left_sprite = "pirate1_walk_left";
	moving_right_sprite= "pirate1_walk_right";
	stabbing_left_sprite = "pirate1_stab_left";
	stabbing_right_sprite= "pirate1_stab_right";
	shooting_left_sprite = "pirate1_shoot_left";
	shooting_right_sprite= "pirate1_shoot_right";
	hero_num = 1;

	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = rm.getSprite("pirate1");
	if(p_temp_sprite == NULL) {
		LOG("Hero1::Hero1 sprite 'pirate1' not found");
	} else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(4);
	}
}


void Hero1::kbd(const df::EventKeyboard *p_kbe) {

	if(act != standing && act != moving) {
		// Can't start new action
		return;
	}

	switch(p_kbe->getKey()) {
	case df::Input::A:
		act = moving;
		dir = left;
		break;
	case df::Input::D:
		act = moving;
		dir = right;
		break;
	case df::Input::W:
		act = jumping;
		break;
	case df::Input::E:
		act = stabbing;
		break;
	//case df::Input::Q:
	//	act = kicking;
	case df::Input::R:
		act = shooting;
		break;
	default:
		return; // Don't set action_time to 0
	}
	action_time = 0;	
}
