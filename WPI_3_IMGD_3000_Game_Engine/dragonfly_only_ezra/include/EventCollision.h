#ifndef __EVENT_COLLISION_H__
#define __EVENT_COLLISION_H__

#include "Event.h"
#include "Object.h"

namespace df {

const std::string COLLISION_EVENT = "df::collision";

class EventCollision : public Event {
private:
	Position pos;	// Where collision occured
	Object *p_obj1; // Object moving, causing collision.
	Object *p_obj2; // Object being collided with.
public:
	// Create collision event at (0,0) with o1 and o2 NULL.
	EventCollision();

	// Create collision event between o1 and o2 at posiiton p.
	// Object o1 'caused' collision by moving into object o2.
	EventCollision(Object *p_o1, Object *p_o2, Position p);

	// Set/get object that caused the collision
	void setObject1(Object *p_new_o1);
	Object *getObject1() const;

	// Set/get object that was collided with.
	void setObject2(Object *p_new_o2);
	Object *getObject2() const;

	// Set/get position of collsiion.
	void setPosition(Position new_pos);
	Position getPosition() const;
};

}
#endif

