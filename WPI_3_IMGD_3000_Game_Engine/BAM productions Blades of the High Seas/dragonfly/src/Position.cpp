#include "Position.h"

df::Position::Position(int init_x, int init_y) {
	x = init_x;
	y = init_y;
}

df::Position::Position() {
	x = 0;
	y = 0;
}

void df::Position::setX(int new_x) {
	x = new_x;
}

int df::Position::getX() const {
	return x;
}

void df::Position::setY(int new_y) {
	y = new_y;
}

int df::Position::getY() const {
	return y;
}

void df::Position::setXY(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}