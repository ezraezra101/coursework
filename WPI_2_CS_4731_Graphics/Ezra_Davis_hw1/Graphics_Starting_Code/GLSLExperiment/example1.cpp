/*
Completed homework 1 for CS 4731 (Graphics)
 Written by Ezra Davis 9/12/2014
 Draws a variety of items, a Sierpinski triangle, Hilbert's space filling curve, and a few polyline files.

 Files are located in the Graphics_Starting_Code\GLSL\ directory

 Files and contents:
	Angel.h		-	general purpose header file (untouched)
	CheckError.h -	OpenGL error reporting		(untouched)
	dragon.dat	-	Dragon polyline file		(untouched)
*	example1.cpp -	Main file of the program.
					Contains main method, setup functions,
					glut callbacks, and global variables.
					Also includes the rest of the required files.
	freeglut.dll -	Glut library				(untouched)
*	fshader1.glsl -	Primary fragment shader
*	fshader2.glsl - Fragment shader for Hilbert curve's background
	glew32.dll	  - Glew library				(untouched)
*	hilbert.cpp	  - Contains functions related to Hilbert's space filling curve.
	InitShader.cpp- Loads and compiles GLSL files (untouched)
	mat.h		-	Matrix related header		(untouched)
	mat_old_from_angel.h - I assume it does the same thing as mat.h (untouched)
*	polyLines.cpp - Contains functions related to polyline file drawing.
*	sierpinski.cpp- Contains functions related to sierpinski triangle.
	textfile.cpp  - Loads files into C-Strings	(untouched)
	usa.dat		  - USA polyline file			(untouched)
	vec.h		  - Vector related header		(untouched)
	vinci.dat	  - Vinci polyline file			(untouched)
*	vshader1.glsl - Primary vertex shader		(barely modified)
*	vshader2.glsl - Vertex shader for Hilbert curve's background

	Note: there are no modules or encapsulation. All shared variables are simple defined as global variables in example1.cpp

 Compiling & running:
	The program should be set up for visual studio to compile and run by opening Graphics_Starting_Code\GLSLExperiment.sln and pressing Ctrl-F5.

	Other .cpp files are intentionally not included in the visual studio project. they are included manually in example1.cpp
	Simply making sure that all the above files are in the same directory and compiling example1.cpp should compile the whole program.
*/

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut


//A few prototypes
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
char *textFileRead(char *fn);
void reshape(int width, int height);

//Typedefs, enums, and namespace
typedef vec2 point2;
typedef vec3 color3;

using namespace std;

//Things that can be drawn
enum drawing_options_t { SIERPINSKI = 's', HILBERT = 'h', INC_ITERATIONS = 'i', DEC_ITERATIONS = 'r', USA = 'u', DRAGON = 'd', VINCI = 'v', POLYLINE};


////////////////////////////// Constants //////////////////////////////////////////////////

// Maximum number of points in polyline
const int MaxNumPoints = 100000;

//Size of figure in viewport, 0.0 - 1.0, where 1.0 takes up the whole screen
const float drawingSize = 0.95f;

// Actual number of points in polyline
int NumPoints = MaxNumPoints;

//Iterations in the Hilbert curve
int Iterations = 2;

//Drawtype
int DrawType = GL_POINTS;

//The drawing currently being displayed:
int DrawMode = HILBERT;

//Extents of the polyline (only used in POLYLINE DrawMode)
float extents[4] = {0,0,1,1};

// Array for polyline
point2 points[MaxNumPoints];

//Arrays for backdrop in hilbert curve
point2 backdropPoints[4*MaxNumPoints];
color3 colors[MaxNumPoints];//Colors of quads
int NumQuads = 0;			  // # of quads in background
GLuint backgroundProgram;	  //The background's shaders' program

//The shaders' program (for everything but the hilbert curve's background)
GLuint program;

//Vertex array buffers
GLuint vao  = 0; //Main VAO

////////////////////////////// Includes //////////////////////////////////////////////////

#include "polylines.cpp"
#include "sierpinski.cpp"
#include "hilbert.cpp"


////////////////////////////// Functions //////////////////////////////////////////////////

//Sets up the VAO and VBO
void initGPUBuffers( void )
{

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(backdropPoints), NULL, GL_DYNAMIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(backdropPoints), backdropPoints);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) * 2, sizeof(colors), colors );
}

//Loads and initializes the two shader programs
void shaderSetup( void )
{
	GLuint loc;
	// Load shaders and use the resulting shader program
	program = InitShader( "vshader1.glsl", "fshader1.glsl" );
	glUseProgram( program );


	//// Hilbert curve's background shader:
	backgroundProgram = InitShader( "vshader2.glsl", "fshader2.glsl");

	glUseProgram(backgroundProgram);
	//Setting up background points
	GLuint vPosition = glGetAttribLocation(backgroundProgram, "vPosition");
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	//Setting up color
	GLuint vColor = glGetAttribLocation(backgroundProgram, "vColor");
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(backdropPoints)) );


	//// Resuming main shader initialization:
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	loc = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc );
	glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );        // sets white as color used to clear screen
}

//Glut display callback
void display( void )
{
	// All drawing happens in display function
	if(DrawMode != POLYLINE) {
		glClear( GL_COLOR_BUFFER_BIT );                // clear window

		//Adding hilbert curve background
		if(DrawMode == HILBERT) {
			drawHilbertBackground();
		}

		glDrawArrays( DrawType, 0, NumPoints );        // draw the points
		glFlush();									   // force output to graphics hardware


	} else { //Polyline drawing method

		//Figuring out viewport size & padding
		point2 dimensions = polylineViewportSize();
		int paddingX = (glutGet(GLUT_WINDOW_WIDTH) - (4*(int) dimensions.x))/2;
		int paddingY = (glutGet(GLUT_WINDOW_HEIGHT)- (4*(int) dimensions.y))/2;
		printf("%d\t%d\n", paddingX, paddingY);

		glClear( GL_COLOR_BUFFER_BIT );
		for(int row=0; row<4; row++) {
			for(int col=0; col<4;col++) {
				glViewport(paddingX+(int)dimensions.x *col, paddingY+(int)dimensions.y * row,
					(GLsizei)dimensions.x, (GLsizei)dimensions.y);

				glDrawArrays( DrawType, 0, NumPoints );
			}
		}
		glFlush();
	}
}

//Glut keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler
	switch ( key ) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case SIERPINSKI:
		generateSierpinski();
		break;
	case HILBERT:
		generateHilbert();
		break;
	case INC_ITERATIONS:
		Iterations++;
		generateHilbert();
		break;
	case DEC_ITERATIONS:
		if(Iterations >= 0)
			Iterations--;
		generateHilbert();
		break;
	case USA:
		drawFile((char *)"USA.dat");
		break;
	case DRAGON:
		drawFile((char *)"dragon.dat");
		break;
	case VINCI:
		drawFile((char *)"vinci.dat");
		break;
	}

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	display();
}

//Glut reshape handler
void reshape(int width, int height)
{
	if(DrawMode == POLYLINE) {
		display();
		return;
	}

	//Finding new viewport size
	GLsizei squareSize = width > height ? height : width;

	//Placing viewport
	glViewport( (width-squareSize)/2, (height-squareSize)/2,squareSize, squareSize);
}


int main( int argc, char **argv )
{
	// main function: program starts here

	glutInit( &argc, argv );                       // intialize GLUT  
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); // single framebuffer, colors in RGB
	glutInitWindowSize( 640, 480 );                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100,150);               // Top left corner at (100, 150)
	glutCreateWindow( "Ezra Davis HW 1" );       // Create Window

	glewInit();										// init glew

	generateHilbert( );                           // Call function that generates points to draw
	initGPUBuffers( );							   // Create GPU buffers
	shaderSetup( );                                // Connect this .cpp file to shader files

	glutDisplayFunc( display );                    // Register display callback function
	glutKeyboardFunc( keyboard );                  // Register keyboard callback function
	glutReshapeFunc( reshape);					   // Register reshape callback function

	// enter the drawing loop
	glutMainLoop();
	return 0;
}
