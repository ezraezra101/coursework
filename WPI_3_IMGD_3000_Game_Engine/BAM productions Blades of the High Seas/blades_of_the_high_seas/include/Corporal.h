#ifndef CORPORAL_H
#define CORPORAL_H

#include "Foe.h"

class Corporal : public Foe {
protected:
	virtual void onStep();

	virtual void makeDecision();

public:
	Corporal();
};

#endif