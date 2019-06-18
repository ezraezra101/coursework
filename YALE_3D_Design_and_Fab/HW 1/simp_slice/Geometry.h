
#ifndef GEOMETRY
#define GEOMETRY
#include <iostream>

struct Point {
public:
	float x, y, z;
	Point();
	Point(float x, float y, float z);

	bool operator==(const Point &p) const;
	Point operator+(const Point &p) const;
	Point operator-(const Point &p) const;
	Point operator*(const float &f) const;
	// Dot product
	float operator*(const Point &p) const;

	friend std::istream &operator>>(std::istream &input, Point &p) {
		return input >> p.x >> p.y >> p.z;
	}

	friend std::ostream &operator<<(std::ostream &output, const Point &p) {
		return output << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	}
};
#define Vector Point

class Plane {
	float d;
	Vector normal;
public:
	Plane();
	Plane(Point p, Vector normal);

	// Does the plane intersect the line segment between a and b?
	bool intersectsLine(Point a, Point b);
	Point intersectsLineWhere(Point a, Point b);
};

#endif