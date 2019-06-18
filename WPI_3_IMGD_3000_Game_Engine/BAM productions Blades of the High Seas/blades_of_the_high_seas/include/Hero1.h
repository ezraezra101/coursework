#ifndef HERO1_H
#define HERO1_H

#include "Hero.h"

class Hero1 : public Hero {
protected:

	void kbd(const df::EventKeyboard *p_kbe);

public:
	Hero1();
};

#endif