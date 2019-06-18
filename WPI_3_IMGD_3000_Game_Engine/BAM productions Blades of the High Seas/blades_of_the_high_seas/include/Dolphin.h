#ifndef DOLPHIN_H
#define DOLPHIN_H
#include "Object.h"

#define RECOMMENDED_DOLPHIN_COUNT 4

class Dolphin : public df::Object {
private:
	void out();
public:
	Dolphin();

	int eventHandler(const df::Event *squirrel);
};

#endif