#include "Hero2.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "LogManager.h"

Hero2::Hero2() {
	standing_sprite = "pirate2";
	moving_left_sprite = "pirate2_walk_left";
	moving_right_sprite= "pirate2_walk_right";
	stabbing_left_sprite = "pirate2_stab_left";
	stabbing_right_sprite= "pirate2_stab_right";
	shooting_left_sprite = "pirate2_shoot_left";
	shooting_right_sprite= "pirate2_shoot_right";
	hero_num = 2;

	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = rm.getSprite("pirate2");
	if(p_temp_sprite == NULL) {
		LOG("Hero2::Hero2 sprite 'pirate2' not found");
	} else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(4);
	}
}


void Hero2::kbd(const df::EventKeyboard *p_kbe) {
	// If we press ESC, quit game
	if(p_kbe->getKey() == df::Input::ESCAPE) {
		df::GameManager::getInstance().setGameOver();
	}

	if(act != standing && act != moving) {
		// Can't start new action
		return;
	}

	switch(p_kbe->getKey()) {
	case df::Input::J:
		act = moving;
		dir = left;
		break;
	case df::Input::L:
		act = moving;
		dir = right;
		break;
	case df::Input::I:
		act = jumping;
		break;
	case df::Input::O:
		act = stabbing;
		break;
	//case df::Input::Q:
	//	act = kicking;
	case df::Input::P:
		act = shooting;
		break;
	default:
		return; // Don't set action_time to 0
	}
	action_time = 0;	
}
