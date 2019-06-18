#include <stdlib.h>

#include "Foe.h"
#include "Bullet.h"
#include "EventLevelUp.h"
#include "game_utility.h"
#include "Ground.h"
#include "Level.h"

// Engine Includes
#include "EventCollision.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"


Foe::Foe() {
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	df::WorldManager &wm = df::WorldManager::getInstance();

	standing_sprite = "foe";
	moving_left_sprite = "foe_walk_left";
	moving_right_sprite= "foe_walk_right";
	stabbing_left_sprite = "foe_stab_left";
	stabbing_right_sprite= "foe_stab_right";
	shooting_left_sprite = "foe_shoot_left";
	shooting_right_sprite= "foe_shoot_right";

	setSprite(rm.getSprite(standing_sprite));
	setSpriteSlowdown(4);

	setType("Foe");
	setSolidness(df::SOFT);
	dir = center;
	act = standing; // Start standing and doing nothing.
	action_time = -1; // You're not preparing anything...
	setPosition(df::Position(rand()%16 + 128, GROUND_POSITION-13));

	setAltitude(df::MAX_ALTITUDE-2);
}

Foe::~Foe() {
	Level::getCurrentLevel()->decrementFoeCount();
}

void Foe::move() {

	df::WorldManager &wm = df::WorldManager::getInstance();
	df::ResourceManager &rm = df::ResourceManager::getInstance();
	int dx = (dir == left) ? -1 : 1;

	// If we've just started walking
	if(action_time == 0) {
	df::Sprite *p_new_sprite;
		if(dir == left) {
			p_new_sprite = rm.getSprite(moving_left_sprite);
		} else { // Dir isn't left (centered or moving right)
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

	// Prevent movement onto 'friendlies'
	if(nearestFoe(this, dir) && abs(nearestFoe(this, dir)->getPosition().getX()-getPosition().getX()) <= 16+4) {// swordswing + sprite_width
		act = standing;
		return; // Don't move if we're within 4 squares of a friend.
	}

	// Actually move
	df::Position p(getPosition().getX() + dx, getPosition().getY());

	if(p.getX() > 3 && p.getX() < wm.getBoundary().getHorizontal()-1) {
		wm.moveObject(this, p);
	}
}
void Foe::jump() {
	if(action_time >= HERO_JUMP_WAIT) {
		df::Position p = df::Position(getPosition().getX(), getPosition().getY() - JUMP_HEIGHT);
		if(p.getY() > 0) {
			df::WorldManager::getInstance().moveObject(this, p);
		}
		stand();
	}
}

void Foe::stab() {
	if(action_time == 0) {
		df::ResourceManager &rm = df::ResourceManager::getInstance();
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


void Foe::stand() {
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

void Foe::shoot() {
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
		if(dir == left) {
			new Bullet(left, df::Position(getPosition().getX()-1,
				getPosition().getY() + 6), df::RED);
		} else {
			new Bullet(right, df::Position(getPosition().getX() + 12,
				getPosition().getY() + 6), df::RED);
		}

		// Undo position offset
		if(dir == left) {
			setPosition(df::Position(getPosition().getX()+4, getPosition().getY()));
		}

		stand();
	}
}

void Foe::onStep() {
	if(act == standing) {
		makeDecision();
	}
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
		shoot();
		break;
	case kicking:
	case standing:
	default:
		stand();
		break;
	}
	action_time++;
}

void Foe::makeDecision() {
	loop_state = (loop_state+1);

	// Move on every odd number, stab if it's divisible by 10

	if(not (loop_state % 10)) {
		act = stabbing;
	} else if(loop_state % 2) {
		act = moving;
	} else {
		act = standing;
	}

	// Face the nearest hero:
	Hero *h = nearestHero(getPosition());
	if(h != NULL && h->getPosition().getX() < getPosition().getX()) {
		dir = left;
	} else {
		dir = right;
	}

	action_time = 0;
}

int Foe::eventHandler(const df::Event *p_e) {
	if(p_e->getType() == LEVEL_UP_EVENT) {
		df::WorldManager &wm = df::WorldManager::getInstance();
		wm.markForDelete(this);
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
		Hero *h =	(dynamic_cast<Hero *>(ce->getObject1()));
		h = h ? h : (dynamic_cast<Hero *>(ce->getObject2()));
		if(h != NULL && h->isSwingingSwordAt(this)) {
			wm.markForDelete(this);
			return 1;
		}
	}
	return df::Object::eventHandler(p_e);
}

bool Foe::isSwingingSwordAt(df::Object *p_o) {
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