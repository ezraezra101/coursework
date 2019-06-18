#include "EventCollision.h"

// Create collision event at (0,0) with o1 and o2 NULL.
df::EventCollision::EventCollision() {
	setType(COLLISION_EVENT);
	p_obj1 = NULL;
	p_obj2 = NULL;
	pos = df::Position(0,0);
}

// Create collision event between o1 and o2 at position p.
// Object o1 'caused' collision by moving into object o2.
df::EventCollision::EventCollision(df::Object *p_o1, df::Object *p_o2, Position p) {
	setType(COLLISION_EVENT);
	p_obj1 = p_o1;
	p_obj2 = p_o2;
	pos = p;
}

void df::EventCollision::setObject1(df::Object *p_new_o1) {
	p_obj1 = p_new_o1;
}

df::Object *df::EventCollision::getObject1() const {
	return p_obj1;
}

void df::EventCollision::setObject2(df::Object *p_new_o2) {
	p_obj2 = p_new_o2;
}

df::Object *df::EventCollision::getObject2() const {
	return p_obj2;
}

void df::EventCollision::setPosition(df::Position new_pos) {
	pos = new_pos;
}

df::Position df::EventCollision::getPosition() const {
	return pos;
}

