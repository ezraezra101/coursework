////////////////////////////////////////////////////////////////////////////////
// Polyline associated functions (and includes)
////////////////////////////////////////////////////////////////////////////////

//Includes used for string manipulations & file openning
#include <fstream>
#include <string>

// Loads the points from the specified file and prepares for rendering
void drawFile(char *filename)
{
	//Initialize drawing mode
	NumPoints = 0;
	DrawType  = GL_LINES;
	DrawMode  = POLYLINE;

	//Load file
	ifstream file;
	file.open(filename);
	if(!file.good()) {
		printf("File not found");
		return;
	}

	//Ignore comments
	string line;
	getline(file, line);
	while(line[0] != '*') {
		getline(file, line);
	}

	//Figure out extent & # of polylines:
	int numPolylines;
	getline(file, line);
	sscanf(line.c_str(), "%f %f %f %f", extents, extents+1, extents+2, extents+3);
	float width  = extents[2] /*right*/ - extents[0]/*left*/;
	float height = extents[1] /*bottom*/ - extents[3]/*top*/;

	getline(file, line);
	sscanf(line.c_str(), "%d", &numPolylines);

	printf("Extents: %f, %f, %f, %f\n%d Polylines\n\n", extents[0],extents[1],extents[2],extents[3], numPolylines);

	//Read in each polyline:
	for(int j=0; j<numPolylines; j++) {
		int polylinePoints = 0;
		getline(file,line);
		sscanf(line.c_str(), "%d", &polylinePoints);

		//Initialize first point
		float oldX, oldY;
		float x,    y;
		getline(file, line); sscanf(line.c_str(), "%f %f", &oldX, &oldY);
		oldX = (oldX -extents[0])*2/width-1;
		oldY = (oldY -extents[3])*2/height-1;
		//Read in points
		for(int k=0; k<polylinePoints-1; k++) {
			getline(file,line); sscanf(line.c_str(), "%f %f", &x, &y);

			//Change x and y to be mapped to the interval [-1..1]
			x = (x -extents[0]/*left*/)*2/width-1;
			y = (y -extents[3]/*bottom*/)*2/height-1;
			if(x > 1.0 || y > 1.0) {
				printf("Error: point is outside extents");
			}

			points[NumPoints++] = point2(oldX*drawingSize, oldY*drawingSize); //drawingSize ensures that the polylines don't touch the edge of the box.
			points[NumPoints++] = point2(x*drawingSize, y*drawingSize);		//		It looks better that way
			oldX = x; oldY = y;
		}
	}
	//Move points to buffer
	glBufferSubData( GL_ARRAY_BUFFER,0, sizeof(points), points );
}

//Calculates size of each viewport in a polyline render
point2 polylineViewportSize() {
	float imgHeight = extents[1]-extents[3];
	float imgWidth  = extents[2]-extents[0];
	float width =  glutGet(GLUT_WINDOW_WIDTH)/ 4.0f;
	float height = glutGet(GLUT_WINDOW_HEIGHT) / 4.0f;

	if(width / imgWidth < height / imgHeight) {
		height = imgHeight * (width/ imgWidth);
	} else {
		width =  imgWidth  * (height/imgHeight);
	}

	return point2(width, height);
}