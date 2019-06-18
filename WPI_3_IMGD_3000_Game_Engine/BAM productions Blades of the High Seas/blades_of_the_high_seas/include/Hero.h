#ifndef HERO_H
#define HERO_H

#include "EventKeyboard.h"
#include "Object.h"

const int HERO_JUMP_WAIT = 12;
const int HERO_STAB_WAIT = 12;
const int HERO_SHOOT_WAIT= 40; // Should be divisable by 5
const int JUMP_HEIGHT = 16;

enum FACING {
	center = 0, left, right
};

enum ACTION {
	moving, jumping, stabbing, shooting, kicking, standing,
};

class Hero : public df::Object {
protected:
	static int heroes_alive;
	int hero_num; // Whether we're hero 1 or 2
	int action_time; // Time doing current action.
	FACING dir;
	ACTION act;

	int bullet_count;

	std::string standing_sprite;
	std::string moving_left_sprite;
	std::string moving_right_sprite;
	std::string stabbing_left_sprite;
	std::string stabbing_right_sprite;
	std::string shooting_left_sprite;
	std::string shooting_right_sprite;

	virtual void kbd(const df::EventKeyboard *p_kbe);
	void move();
	void jump();
	void stab();
	void stand();
	void shoot();
	void onStep();

	void draw();

public:
	Hero();
	~Hero();

	int eventHandler(const df::Event *p_e);

	bool isSwingingSwordAt(Object *p_o);
};

#endif