#include "Box.h"
#include "LogManager.h"

// Create tiny box at (0,0).
df::Box::Box() {
	corner = df::Position(0,0);
	setHorizontal(0);
	setVertical(0);
}

// Create box with initialized values.
df::Box::Box(df::Position init_corner, int init_horizontal, int init_vertical) {
	corner = init_corner;
	horizontal = init_horizontal;
	vertical = init_vertical;
}

// Get/set corner
void df::Box::setCorner(Position p) {
	corner = p;
}
df::Position df::Box::getCorner() const {
	return corner;
}

// Get/set horizontal and vertical size of box.
void df::Box::setHorizontal(int h) {
	if(h < 0) {
		LOG("Trying to set Box's width to a negative number! (%d)", h);
	}
	horizontal = h;
}
int df::Box::getHorizontal() const {
	return horizontal;
}

void df::Box::setVertical(int v) {
	if(v < 0)
		LOG("Trying to set Box's height to a negative number! (%d)", v);
	vertical = v;
}
int df::Box::getVertical() const {
	return vertical;
}
