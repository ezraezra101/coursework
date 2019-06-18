////////////////////////////////////////////////////////////////////////////////
//Hilbert curve related functions
////////////////////////////////////////////////////////////////////////////////

//Creates a quad and puts it into 'backdropPoints'
void quad(float x, float y, float size) {
	if(NumQuads+1 > MaxNumPoints)
		return;

	backdropPoints[NumQuads*4+0] = point2(x,y);
	backdropPoints[NumQuads*4+1] = point2(x+size,y);
	backdropPoints[NumQuads*4+2] = point2(x+size,y+size);
	backdropPoints[NumQuads*4+3] = point2(x,y+size);
	NumQuads++;
}

//Adds all the colors to a already constructed hilbert curve.
void makeColors() {
	color3 red   = color3(1.0f, 0.4f, 0.4f);
	color3 green = color3(0.4f, 1.0f, 0.4f);
	color3 blue  = color3(0.4f, 0.4f, 1.0f);
	color3 white = color3(1.0f, 1.0f, 1.0f);

	color3 color_order[] = {
		red, green, blue, white,
		red, green, blue, white,
		white, blue, green, red,
		white, blue, green, red
	};

	for(int i=0; i<NumQuads; i++) {
		//RGBW RGBW WBGR WBGR
		colors[i] = color_order[i%16];
	}
}

//Creates the points for a single 'cup' in the hilbert curve. Also creates background points.
void drawHilbertSquiggle(unsigned int dir, float x, float y, float size)
{

	if(NumPoints+4 > MaxNumPoints) {
		return;
	}
	point2 newPoints[4] = {
		point2(x+size/4, y+size/4),		//bottom left
		point2(x+size/4, y+3*size/4),	//top left
		point2(x+3*size/4, y+3*size/4), //top right
		point2(x+3*size/4, y+size/4),	//bottom right
	};

	int order[4] = {0,0,0,0};

	switch(dir) {
	case 0:
		order[0] = 0;order[1]=1; order[2]=2; order[3] = 3;
		break;
	case 1:
		order[0] = 0;order[1]=3; order[2]=2; order[3] = 1;
		break;
	case 2:
		order[0] = 2;order[1]=3; order[2]=0; order[3] = 1;
		break;
	case 3:
		order[0] = 2;order[1]=1; order[2]=0; order[3] = 3;
		break;
	default:
		printf("Error: Invalid direction");
	}

	point2 quadPoints[4] = {
		point2(x,y),
		point2(x,y+size/2),
		point2(x+size/2, y+size/2),
		point2(x+size/2, y)
	};

	for(int i=0; i<4; i++) {
		quad(quadPoints[order[i]].x, quadPoints[order[i]].y, size/2);
	}

	points[NumPoints+0] = newPoints[order[0]];
	points[NumPoints+1] = newPoints[order[1]];
	points[NumPoints+2] = newPoints[order[2]];
	points[NumPoints+3] = newPoints[order[3]];

	NumPoints += 4;

	return;
}

//Recursive function for creating the points of a hilbert curve
void divideHilbert(unsigned int dir, int iterations, float x, float y, float size) {
	if(iterations < 0) {
		drawHilbertSquiggle(dir, x, y, size);

	} else {
		iterations--;
		size/=2;
		switch(dir) {
		case (0):
			divideHilbert(1, iterations, x, y, size);
			divideHilbert(0, iterations, x, y+size, size);
			divideHilbert(0, iterations, x+size, y+size, size);
			divideHilbert( 3, iterations, x+size, y, size);
			break; case(1):
			divideHilbert(0, iterations, x, y, size);
			divideHilbert( 1 , iterations, x+size, y, size);
			divideHilbert(1, iterations, x+size, y+size, size);
			divideHilbert(2, iterations, x, y+size, size);
			break; case(2):
			divideHilbert(3, iterations, x+size, y+size, size);
			divideHilbert(2, iterations, x+size, y, size);
			divideHilbert(2, iterations, x, y, size);
			divideHilbert(1, iterations, x, y+size, size);
			break; case(3):
			divideHilbert(2, iterations, x+size, y+size, size); 
			divideHilbert(3, iterations, x, y+size, size);
			divideHilbert(3, iterations, x, y, size);
			divideHilbert(0, iterations, x+size, y, size);
		}
	}
	return;
}

//Creates all the hilbert curve's points in memory.
void generateHilbert( void )
{
	NumPoints = 0;
	NumQuads = 0;
	DrawMode = HILBERT;

	//generate points for the hilbert curve.
	divideHilbert( 0, Iterations, -drawingSize, -drawingSize, 2.0f*drawingSize);

	makeColors();

	//Set it up for the GPU
	DrawType = GL_LINE_STRIP;
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(points), backdropPoints );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)*2, sizeof(colors), colors);

}

//Draws the hilbert background... slowly.
//Called in display function
void drawHilbertBackground() {

	glUseProgram(backgroundProgram);

	GLuint vColor = glGetUniformLocation(backgroundProgram, "vColor");

	for(int i=0; i<NumQuads; i++) {
		glUniform3fv( vColor, 1, colors[i]);

		glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(point2)*4, backdropPoints+i*4);
		glDrawArrays(GL_QUADS, 0, 4);
	}

	glUseProgram(program);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
}