#ifndef HERO2_H
#define HERO2_H

#include "Hero.h"

class Hero2 : public Hero {
protected:

	void kbd(const df::EventKeyboard *p_kbe);

public:
	Hero2();
};

#endif