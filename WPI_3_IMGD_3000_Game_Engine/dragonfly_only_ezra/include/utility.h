#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Box.h"
#include "Position.h"
#include "Object.h"
namespace df {
	bool positionsIntersect(Position p1, Position p2);

	// Returns true if the boxes overlap/intersect, else false.
	bool boxIntersectsBox(Box a, Box b);

	// Convert relative bounding Box for Object to absolute world Box.
	Box getWorldBox(const Object *p_o);

	// Convert relative bounding Box for Object to absolute world Box.
	Box getWorldBox(const Object *p_o, Position where);

	// Convert world position to view position.
	Position worldToView(Position world_pos);

	// Convert int to a string, returning string.
	std::string toString(int i);
}

#endif