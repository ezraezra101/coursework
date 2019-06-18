#include <sstream>

#include "WorldManager.h"
#include "utility.h"

bool df::positionsIntersect(Position p1, Position p2) {
	if( p1.getX() == p2.getX() &&
		p1.getY() == p2.getY()) {
		return true;
	} else {
		return false;
	}
}

// Is a between b1 and b2?
static inline bool isBetween(int b1, int a, int b2) {
	return (b1 >= a && a >= b2) || (b2 >= a && a >= b1);
}

// If the two line segments overlap
static inline bool linesOverlap(int a_start, int a_length, int b_start, int b_length) {
	return isBetween(b_start, a_start, b_start + b_length)
		|| isBetween(a_start, b_start, a_start + a_length);
}

// Returns true if the boxes overlap/intersect, else false.
bool df::boxIntersectsBox(df::Box a, df::Box b) {
	// Test horizontal and vertical edges overlap
	return linesOverlap(a.getCorner().getX(), a.getHorizontal(), b.getCorner().getX(), b.getHorizontal())
		&& linesOverlap(a.getCorner().getY(),   a.getVertical(), b.getCorner().getY(), b.getVertical())
		;
}

// Convert relative bounding Box for Object to absolute world Box.
df::Box df::getWorldBox(const df::Object *p_o) {
	return getWorldBox(p_o, p_o->getPosition());
}

// Return bounding box as if object is at position where
df::Box df::getWorldBox(const df::Object *p_o, df::Position where) {
	df::Box box = p_o->getBox();

	where.setXY(
		box.getCorner().getX() + where.getX(),
		box.getCorner().getY() + where.getY()
		);

	box.setCorner(where);

	return box;
}

// Convert world position to view position.
df::Position df::worldToView(df::Position world_pos) {
	df::Position view_origin = df::WorldManager::getInstance().getView().getCorner();

	return df::Position(world_pos.getX() - view_origin.getX(),
		world_pos.getY() - view_origin.getY());
}

// Convert int to a string, returning string.
std::string df::toString(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

