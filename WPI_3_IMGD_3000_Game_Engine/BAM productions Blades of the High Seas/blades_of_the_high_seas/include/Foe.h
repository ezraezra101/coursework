#ifndef FOE_H
#define FOE_H

#include "EventKeyboard.h"
#include "Object.h"

#include "Hero.h"

class Foe : public df::Object {
protected:
	int action_time; // Time doing current action.
	int loop_state;
	FACING dir;
	ACTION act;

	std::string standing_sprite;
	std::string moving_left_sprite;
	std::string moving_right_sprite;
	std::string stabbing_left_sprite;
	std::string stabbing_right_sprite;
	std::string shooting_left_sprite;
	std::string shooting_right_sprite;

	void move();
	void jump();
	void stab();
	void stand();
	void shoot();
	virtual void onStep();

	virtual void makeDecision();

public:
	Foe();
	~Foe();

	int eventHandler(const df::Event *p_e);

	void setAction();
	void setDir();

	bool isSwingingSwordAt(df::Object *p_o);
};

#endif