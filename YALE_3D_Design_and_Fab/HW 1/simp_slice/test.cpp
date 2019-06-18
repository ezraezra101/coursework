#define BOOST_TEST_MODULE GeometryTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Geometry.h"

BOOST_AUTO_TEST_SUITE(PointTest)

BOOST_AUTO_TEST_CASE( constructor ) {
    Point p;
    p = Point(1,2,3);
    BOOST_CHECK(p.x == 1);
}
BOOST_AUTO_TEST_CASE( add ) {
    Point p;
    p = Point(1,2,3);
    p = p+p;
    BOOST_CHECK(p.x == 2.0);
    BOOST_CHECK(p.y == 4.0);
    BOOST_CHECK(p.z == 6.0);
}

BOOST_AUTO_TEST_CASE( subtract ) {
    Point p;
    p = Point(1,2,3);
    p = p-p;
    BOOST_CHECK(p.x == 0);
    BOOST_CHECK(p.y == 0.0);
    BOOST_CHECK(p.z == 0.0);
}
BOOST_AUTO_TEST_CASE( dotProduct ) {
    Point p;
    p = Point(1,2,3);
    float prod = p*p;
    BOOST_CHECK(1+4+9==prod);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PlaneTest)

BOOST_AUTO_TEST_CASE( constructor) {
	Point p = Point(1,2,3);
	Vector n = Vector(3,2,1);
	Plane plane = Plane(p,n);
}

BOOST_AUTO_TEST_CASE(intersectsLine) {
	Point p = Point(0,0,0);
	Point p2 = Point(0,0,1);

	Plane plane = Plane(p, p2);

	// Endpoint in plane
	BOOST_CHECK(plane.intersectsLine(p, p2));
	BOOST_CHECK(plane.intersectsLine(p2, p));

	// Endpoint not in plane
	Point p3 = Point(0,-1,-1);
	BOOST_CHECK(plane.intersectsLine(p2, p3));

	// Parallel
	Point p4 = Point(1,1,0);
	BOOST_CHECK(plane.intersectsLine(p, p4));
	Point p5 = Point(1,1,1); // Parallel, not in plane
	BOOST_CHECK(!plane.intersectsLine(p2, p5));

	// Not in plane
	BOOST_CHECK(!plane.intersectsLine(Point(2,2,2), p5));
}

BOOST_AUTO_TEST_CASE(intersectsLineWhere) {
	Point p = Point(0,0,0);
	Point p2 = Point(0,0,1);

	Plane plane = Plane(p, p2); // z = 0 plane

	// Endpoint in plane
	BOOST_CHECK(plane.intersectsLineWhere(p, p2) == p);
	BOOST_CHECK(plane.intersectsLineWhere(p2, p) == p);

	// Endpoint not in plane
	Point p3 = Point(0,-1,-1);
	Point p3_5 = Point(0, 1, 1);
	BOOST_CHECK(plane.intersectsLineWhere(p3_5, p3) == p);

	// Parallel
	Point p4 = Point(1,1,0);
	BOOST_CHECK_THROW(plane.intersectsLineWhere(p, p4), std::exception);
	Point p5 = Point(1,1,1); // Parallel, not in plane
	BOOST_CHECK_THROW(plane.intersectsLineWhere(p2, p5), std::exception);

	// Not in plane
	BOOST_CHECK_THROW(plane.intersectsLineWhere(p2, p5), std::exception);
}

BOOST_AUTO_TEST_SUITE_END()
