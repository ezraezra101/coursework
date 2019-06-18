#include "Geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void readTriangleStripAndPlane(char *filename);

void validateFile(ifstream &file);
Plane makePlane(ifstream &file);
void findIntersections(ifstream &file, Plane plane);

static Point *segments[2];

int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "Usage: \n\t" << argv[0] << " [filename]" << endl;
		return 1;
	}

	ifstream file;
	file.open(argv[1]);

	validateFile(file);

	Plane plane = makePlane(file);

	findIntersections(file, plane);

	file.close();

	return 0;
}

void validateFile(ifstream &file) {

	float x;

	if(!file.good()) {
		cout << "File does not exist :-(" << endl;
		exit(1);
	}

	int count = 0;
	while(!file.eof()) {
		file >> x;
		if(file.fail()) break;
		count++;
	}
	if(count % 3 != 0) {
		cout << count;
		cout << "Illegal input file: Must have number of floats divisible by 3." << endl;
		file.close();
		exit(1);
	} else if(count < 3*4) { // Must be 4 points at least - 1 triangle + normal
		cout << "Not enough input points!" << endl;
		file.close();
		exit(1);
	}
	file.clear();
	file.seekg(0, ios::beg);

}

Plane makePlane(ifstream &file) {

	int numPoints = 0;
	Point pointSum = Point(0,0,0);
	Point p, nextP;
	file >> nextP;

	while(!file.eof()) {
		p = nextP;
		
		pointSum = pointSum + p;

		numPoints++;

		file >> nextP;
	}
	file.clear();
	file.seekg(0, ios::beg);

	// Remove the normal from the points calculation.
	pointSum = pointSum - p;
	numPoints--;

	Point avgPoint = Point(
		pointSum.x/numPoints,
		pointSum.y/numPoints,
		pointSum.z/numPoints
	);

	return Plane(avgPoint, p);
}

void findIntersections(ifstream &file, Plane plane) {
	Point points[3];
	int numLines = 0;
	int intersections = 0;

	stringstream output;

	// Read in the first two points. All later points introduce two new line segments
	file >> points[1];
	file >> points[0];

	numLines++;
	if(plane.intersectsLine(points[0], points[1])) {
		try {
			output << plane.intersectsLineWhere(points[0], points[1]);
		} catch(runtime_error) { // Line is within plane!
			output << points[0] << points[1];
			intersections++;
		}
	}

	Point nextPoint;
	file >> nextPoint;

	while(true) {
		points[2] = points[1];
		points[1] = points[0];
		points[0] = nextPoint;
		file >> nextPoint;

		if(!file.good()) { // points[0] is the normal and we've run off the end of the file
			break;
		}
		// Note: does not catch duplicate endpoints...
		numLines++;
		if(plane.intersectsLine(points[0], points[1])) {
			try {
				output << plane.intersectsLineWhere(points[0], points[1]);
			} catch(runtime_error) { // Line is within plane!
				output << points[0] << points[1];
			}
			intersections++;
		}
		numLines++;
		if(plane.intersectsLine(points[0], points[2])) {
			try {
				output << plane.intersectsLineWhere(points[0], points[1]);
			} catch(runtime_error) { // Line is within plane!
				output << points[0] << points[1];
			}
			intersections++;
		}
	}
	output << endl;

	if(numLines == intersections) {
		cout << "Error: Entire input is within plane!" << endl;
		file.close();
		exit(1);
	} else if(intersections == 0) {
		// I don't think this is even possible because the plane contains
		// an average point for the triangle strip, but this conditional costs be very little.
		cout << "Error: Input does not intersect plane!" << endl;
		file.close();
		exit(1);
	}
	
	cout << output.str();
	
	file.clear();
	file.seekg(0, ios::beg);
}
