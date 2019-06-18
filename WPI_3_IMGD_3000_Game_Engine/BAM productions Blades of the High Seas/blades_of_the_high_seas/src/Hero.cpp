#include <stdlib.h>

#include "Hero.h"
#include "Bullet.h"
#include "EventLevelUp.h"
#include "Foe.h"
#include "game_utility.h"
#include "Level.h"

// Engine Includes
#include "EventCollision.h"
#include "EventStep.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

int Hero::heroes_alive = 0;

Hero::Hero() {
	heroes_alive++;
	df::WorldManager &wm = df::WorldManager::getInstance();

	setType("Hero");
	setSolidness(df::HARD);
	dir = center;
	act = standing; // Start standing and doing nothing.
	action_time = -1; // You're not preparing anything...
	setYVelocity(0.5); // Gravity

	bullet_count = 3;

	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::Sprite *p_temp_sprite = rm.getSprite("pirate1");
	if(p_temp_sprite == NULL) {
		LOG("Hero::Hero sprite 'pirate1' not found");
	} else {
		setSprite(p_temp_sprite);
		setSpriteSlowdown(4);
	}

	setAltitude(df::MAX_ALTITUDE-1);
	standing_sprite = "pirate1";
	moving_left_sprite = "pirate1_walk_left";
	moving_right_sprite= "pirate1_walk_right";
	stabbing_left_sprite = "pirate1_stab_left";
	stabbing_right_sprite= "pirate1_stab_right";
	shooting_left_sprite = "pirate1_shoot_left";
	shooting_right_sprite= "pirate1_shoot_right";
	hero_num = 0;
}

void Hero::kbd(const df::EventKeyboard *p_kbe) {
	// Defined by subclasses!
}

Hero::~Hero() {
	if(Level::getCurrentLevel()) {
		Level::getCurrentLevel()->heroHasDied(this);
	}
}

void Hero::move() {

	df::WorldManager &wm = df::WorldManager::getInstance();
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	int dx = (dir == left) ? -1 : 1;

	// If we've just started walking
	if(action_time == 0) {
	df::Sprite *p_new_sprite;
		if(dir == left) {
			p_new_sprite = rm.getSprite(moving_left_sprite);
		} else if(dir == right) {
			p_new_sprite = rm.getSprite(moving_right_sprite);
		}
		if(getSprite() != p_new_sprite) {
			setSprite(p_new_sprite);
		}
		setSpriteSlowdown(4);
	} else if(action_time >= 1) {
		stand();
		return;
	}
	// Actually move
	df::Position p(getPosition().getX() + dx, getPosition().getY());

	if(p.getX() > 3 && p.getX() < wm.getBoundary().getHorizontal()-1) {
		wm.moveObject(this, p);
	}
}
void Hero::jump() {
	if(action_time >= HERO_JUMP_WAIT) {
		df::Position p = df::Position(getPosition().getX(), getPosition().getY() - JUMP_HEIGHT);
		if(p.getY() > 0) {
			df::WorldManager::getInstance().moveObject(this, p);
		}
		stand();
	}
}

void Hero::stab() {
	if(action_time == 0) {
		df::ResourceManager &rm = df::ResourceManager::getInstance();
		if(rm.getSound("swing") ){
			rm.getSound("swing")->play();
		}

		if(dir == left) {
			setSprite(rm.getSprite(stabbing_left_sprite));
			setPosition(df::Position(getPosition().getX()-4, getPosition().getY()));
		} else {
			setSprite(rm.getSprite(stabbing_right_sprite));
		}
		setSpriteSlowdown(4);		
	} else if(action_time >= HERO_STAB_WAIT) {
		if(dir == left) {
			setPosition(df::Position(getPosition().getX()+4, getPosition().getY()));
		}
		stand();
	}
}


void Hero::stand() {
	act = standing;
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	if(dir == left) {
		setSprite(rm.getSprite(moving_left_sprite));
	} else {
		setSprite(rm.getSprite(moving_right_sprite));
	}
	setSpriteSlowdown(0);
	setSpriteIndex(0);
}

void Hero::shoot() {
	if(action_time == 0) {
		df::ResourceManager &rm = df::ResourceManager::getInstance();

		if(dir == left) {
			setSprite(rm.getSprite(shooting_left_sprite));
			setPosition(df::Position(getPosition().getX()-4, getPosition().getY()));
			
			// Your gun cannot be hit.
			df::Box b = getBox();
			b.setHorizontal(b.getHorizontal()-4);
			b.setCorner(df::Position(b.getCorner().getX()+4,b.getCorner().getY()));
		} else {
			setSprite(rm.getSprite(shooting_right_sprite));
			// Your gun cannot be hit.
			df::Box b = getBox();
			b.setHorizontal(b.getHorizontal()-4);
			setBox(b);
		}
		setSpriteSlowdown(HERO_SHOOT_WAIT/5);
	} else if(action_time >= HERO_SHOOT_WAIT) {
		// Shoot bullet
		df::ResourceManager &rm = df::ResourceManager::getInstance();
		rm.getSound("gunshot")->play();

		bullet_count--;

		setSpriteSlowdown(4);
		if(dir == left) {
			new Bullet(left, df::Position(getPosition().getX()-1,
				getPosition().getY() + 6), getSprite()->getColor());
		} else {
			new Bullet(right, df::Position(getPosition().getX() + 12,
				getPosition().getY() + 6), getSprite()->getColor());
		}

		// Undo position offset
		if(dir == left) {
			setPosition(df::Position(getPosition().getX()+4, getPosition().getY()));
		}

		stand();
	}
}

void Hero::onStep() {
	switch(act) {
	case moving:
		move();
		break;
	case jumping:
		jump();
		break;
	case stabbing:
		stab();
		break;
	case shooting:
		if(bullet_count) {
			shoot();			
		} else {
			act = standing;
		}
		break;
	case kicking:
	case standing:
	default:
		act = standing;
		break;
	}
	action_time++;
}

int Hero::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == LEVEL_UP_EVENT) {
		df::WorldManager &wm = df::WorldManager::getInstance();
		wm.markForDelete(this);
		return 1;
	} else if(p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard *ke = static_cast<const df::EventKeyboard *>(p_e);
		kbd(ke);
		return 1;
	} else if(p_e->getType() == df::STEP_EVENT) {
		onStep();
		return 1;
	} else if(p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision *ce = static_cast<const df::EventCollision *>(p_e);
		df::WorldManager &wm = df::WorldManager::getInstance();

		// If either object is a bullet, and it's heading towards 'us',
		//   Remove this
		Bullet *b =	(dynamic_cast<Bullet *>(ce->getObject1()));
		b = b ? b : (dynamic_cast<Bullet *>(ce->getObject2()));	
		if(b != NULL && b->headingTowards(this)) {
			wm.markForDelete(this);
			return 1;
		}

		Foe *f =	(dynamic_cast<Foe *>(ce->getObject1()));
		f = f ? f : (dynamic_cast<Foe *>(ce->getObject2()));
		if(f != NULL && f->isSwingingSwordAt(this)) {
			wm.markForDelete(this);
			return 1;
		}
	}

	//printf("%s\n", p_e->getType().c_str());
	return df::Object::eventHandler(p_e);
}

bool Hero::isSwingingSwordAt(df::Object *p_o) {
	if(act != stabbing || getSpriteIndex() != 2) {
		return false;
	}
	int start_x = p_o->getPosition().getX() + p_o->getBox().getCorner().getX();
	int end_x = start_x + p_o->getBox().getHorizontal();
	bool towards_x =	(dir == left && start_x < getPosition().getX()) ||
						(dir == right && end_x > getPosition().getX());
	
	// If their heights are within 6 of each other
	bool within_y = abs(getPosition().getY() - p_o->getPosition().getY()) < 6;
	return	towards_x && within_y;
}


void Hero::draw() {
	df::Object::draw();

	for(int i = 0; i<bullet_count; i++) {
		df::GraphicsManager::getInstance().drawString(df::Position(4+i*4, 5*hero_num), "=>", df::LEFT_JUSTIFIED, getSprite()->getColor());
	}
}
