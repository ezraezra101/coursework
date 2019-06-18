#ifndef __BOX_H__
#define __BOX_H__

#include "Position.h"

namespace df{

// Note: a box that is width 0 and height 0 represents a 1x1 character on the grid.

class Box {
private:
	Position corner; // The upper left corner of the object
	int horizontal; // Horizontal dimension.
	int vertical;  // Vertical dimension.

public:
	// Create tiny box at (0,0).
	Box();

	// Create box with initialized values.
	Box(Position init_corner, int init_horizontal, int init_vertical);

	// Get/set corner
	void setCorner(Position p);
	Position getCorner() const;

	// Get/set horizontal and vertical size of box.
	void setHorizontal(int h);
	int getHorizontal() const;

	void setVertical(int v);
	int getVertical() const;
};

}
#endif

