#ifndef GROUND_H
#define GROUND_H

#include "Object.h"

const int GROUND_POSITION = 40; // Floor Y value.

class Ground : public df::Object {
public:
	Ground();
};
#endif