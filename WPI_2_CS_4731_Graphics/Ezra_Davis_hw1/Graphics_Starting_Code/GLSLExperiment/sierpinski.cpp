////////////////////////////////////////////////////////////////////////////////
//Sierpinski triangle related function
////////////////////////////////////////////////////////////////////////////////

// Creates the points in a Sierpinski gasket
void generateSierpinski( void )
{
	// Specifiy the vertices for a triangle
	points[0] = point2( -drawingSize, -drawingSize );
	points[1] = point2( 0.0, drawingSize );
	points[2] = point2( drawingSize, -drawingSize );

	points[3] = point2(0.0,0.0);
	point2 newPoint = points[3];
	//Make a lot of points inside the triangle.
	for(int i=4; i<MaxNumPoints; i++) {
		newPoint = (newPoint+points[rand() % 3])/2;
		points[i] = newPoint;
	}

	NumPoints = MaxNumPoints;
	DrawType = GL_POINTS;
	DrawMode = SIERPINSKI;

	glBufferSubData( GL_ARRAY_BUFFER,0, sizeof(points), points );
	display();
}