#include "Geometry.h"
#include <iostream>
#include <string>


inline bool roughlyZero(float f) {
	float epsilon = 0.0000001;
	return f < epsilon && f > -epsilon;
}

Point::Point() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

bool Point::operator==(const Point &p) const {
	Point origin = *this - p;
	return roughlyZero(origin.x) && roughlyZero(origin.y) && roughlyZero(origin.z);
}

// Dot product
float Point::operator*(const Point &p) const {
	return p.x*this->x + 
		p.y*this->y + 
		p.z*this->z;
}
Point Point::operator*(const float &f) const {
	return Point(f*this->x, f*this->y, f*this->z);
}

Point Point::operator+(const Point &p) const {
	return Point(p.x + this->x, p.y + this->y, p.z + this->z);
}
Point Point::operator-(const Point &p) const {
	return Point(this->x - p.x, this->y - p.y, this->z - p.z);
}

// A plane of the form Normal*point + d = 0.
Plane::Plane() {
	this->d = 0;
	this->normal = Vector(0,0,1);

}
Plane::Plane(Point p, Vector normal) {
	this->d = - (normal * p); // Solution to Ax + By + Cz + d = 0
	this->normal = normal;
}

// Where does this plane intersect the line segment between A and B?
Point Plane::intersectsLineWhere(Point a, Point b) {

	// Where does normal * <x,y,z> + d = 0 match  a + (b-a) * alpha ?
	// Find alpha where: Normal * (a+(b-a)*alpha) + d = 0
	// Normal * a + d + Normal * (b-a) * alpha = 0
	// (Normal*a+d) = -Normal*(b-a) *alpha
	// - (Normal*a+d)/ (Normal*(b-a)) = alpha
	// If alpha is between 0 and 1, then it works.

	// If the denominator is 0, then the line segment is parallel to the plane
	float q = (this->normal * (b - a));
	if(roughlyZero(q)) { // Fix later!
		if(roughlyZero(this->normal * a + d)) {
			throw std::runtime_error("Line is entirely within plane");
		} else {
			throw std::runtime_error("Line is parallel to plane");
		}
	}

	float alpha = -(this->normal * a + d) / q;
	if(alpha > 1 || alpha < 0) {
		throw std::runtime_error("Point does not intersect line segment!");
	}

	return a + (b-a) * alpha;
}

// Does this plane intersect the line segment between A and B?
bool Plane::intersectsLine(Point a, Point b) {

	// Where does normal * <x,y,z> + d = 0 match  a + (b-a) * alpha ?
	// Find alpha where: Normal * (a+(b-a)*alpha) + d = 0
	// Normal * a + d + Normal * (b-a) * alpha = 0
	// (Normal*a+d) = -Normal*(b-a) *alpha
	// - (Normal*a+d)/ (Normal*(b-a)) = alpha
	// If alpha is between 0 and 1, then it works.

	// If the denominator is 0, then the line segment is parallel to the plane
	float q = (this->normal * (b - a));
	if(roughlyZero(q)) {
		return roughlyZero(this->normal * a + d); // Is the whole segment in the plane?
	}

	float alpha = -(this->normal * a + d) / q;
	return alpha >= 0 && alpha <= 1;
}
