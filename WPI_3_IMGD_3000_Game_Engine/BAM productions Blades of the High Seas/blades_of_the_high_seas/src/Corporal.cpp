#include "Corporal.h"
#include "game_utility.h"

Corporal::Corporal() {
	standing_sprite = "corporal";
	moving_left_sprite = "corporal_walk_left";
	moving_right_sprite= "corporal_walk_right";
	stabbing_left_sprite = "corporal_stab_left";
	stabbing_right_sprite= "corporal_stab_right";
	shooting_left_sprite = "corporal_shoot_left";
	shooting_right_sprite= "corporal_shoot_right";

	setType("Corporal");
}

void Corporal::onStep() {
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

void Corporal::makeDecision() {
	loop_state = (loop_state+1) % 23;

	// Move(4 times at 1/4 speed) Stab Stab Shoot

	if(loop_state < 20) {
		if(loop_state % 4) {
			act = standing;
		} else {
			act = moving;
		}
	} else if(loop_state < 22) {
		act = stabbing;
	} else {
		if( not detectEnemies(nearestHero(getPosition()), this) ) {
			act = shooting;
		} else {
			act = stabbing;
		}
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
