#ifndef BULLET_H
#define BULLET_H

#include "Object.h"

#include "Hero.h"


class Bullet : public df::Object {
private:
	FACING dir;
	df::Color color;

public:
	Bullet(FACING direction, df::Position p, df::Color new_color);

	int eventHandler(const df::Event *p_e);

	// If the bullet is heading towards the object, returns true
	bool headingTowards(df::Object *p_o);

	void draw();
};

#endif