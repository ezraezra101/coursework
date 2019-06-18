/*
Homework #2
Graphics - CS 4731
Ezra Davis

Loads a series of PLY files and allows the user do simple manipulations by modifying a Current Transformation Matrix.

Building/Running:
The program should simply be setup to compile and run in visual studio.
You should also be able to compile it by hand by compiling main.cpp and PLY.cpp, which should require all the headers and libraries they need.


Code Organization:
All the PLY loading methods have been abstracted into a PLY class. (In PLY.cpp)
The other functions (and global variables) are in main.cpp.
header.h has the includes and prototypes.
All of the ply files are in the GLSLExperiment/ply_files folder.
cube.cpp isn't actually used, but contains the code from the example for reference.

Each time the program moves to the next model, the ply file is reloaded.

*/


#include "header.h"

//----------------------------------------------------------------------------
int fileNum = 0; //The active file's number
const int MaxFileNum = 43; //Number of PLY files
PLY activeFile;

int width = 0;
int height = 0;

// handle to program
GLuint program;

float degrees = 0;
float degreesStart = 0;
mat4 rotation = Angel::identity();
mat4 translation = Angel::identity();
mat4 shear = Angel::identity();

int angular_velocity = 0;
int velocity[3] = {0,0,0};

//----------------------------------------------------------------------------

void initGPU() {
	// Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	//Set up a VBO
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	//glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*50000,
	//	  NULL, GL_DYNAMIC_DRAW );

	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

	// set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
	// set up vertex arrays
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
	//glVertexAttribPointer is set up later for vColor.

	// sets the default color to clear screen
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); // black background


	//Use random colors:
	GLint colored = glGetUniformLocation(program, "colored");
	glUniform1i( colored, 1 );

}

//Loads a ply file and sets it up to be rendered
void initPLY(int fileNum = 0) {

	//Load the correct file
	activeFile = PLY(PLY::listFiles()[fileNum].c_str());
	//Load arrays
	glBufferData(GL_ARRAY_BUFFER, activeFile.getVertsSize()*2, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, activeFile.getVertsSize(), activeFile.getVerts());
	glBufferSubData(GL_ARRAY_BUFFER, activeFile.getVertsSize(), activeFile.getVertsSize(), activeFile.getColors());

	//Set up attributes
	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(activeFile.getVertsSize()));
}

//Draws the active ply file
void drawPLY() {
	//Do PLY specific drawing settings and draw
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glEnable( GL_DEPTH_TEST );

	glDrawArrays(GL_LINES, 0, activeFile.getVertsCount());

	glDisable( GL_DEPTH_TEST );
}

//----------------------------------------------------------------------------

// this is where the drawing should happen
void display( void )
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
	
	Angel::mat4 perspectiveMat =
		  Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0)
		* Angel::Translate(0.0, 0.0, -2.0f)/*Starting location*/;

	float viewMatrixf[16];
	viewMatrixf[0] = perspectiveMat[0][0];viewMatrixf[4] = perspectiveMat[0][1];
	viewMatrixf[1] = perspectiveMat[1][0];viewMatrixf[5] = perspectiveMat[1][1];
	viewMatrixf[2] = perspectiveMat[2][0];viewMatrixf[6] = perspectiveMat[2][1];
	viewMatrixf[3] = perspectiveMat[3][0];viewMatrixf[7] = perspectiveMat[3][1];

	viewMatrixf[8] = perspectiveMat[0][2];viewMatrixf[12] = perspectiveMat[0][3];
	viewMatrixf[9] = perspectiveMat[1][2];viewMatrixf[13] = perspectiveMat[1][3];
	viewMatrixf[10] = perspectiveMat[2][2];viewMatrixf[14] = perspectiveMat[2][3];
	viewMatrixf[11] = perspectiveMat[3][2];viewMatrixf[15] = perspectiveMat[3][3];
	

	//Constructing the CTM from its constituents. 
	Angel::mat4 modelMat =
		  translation
		* rotation
		* shear
		* activeFile.rescaleMatrix() //Scales all objects to be visible when they first start out
		;

	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0];modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0];modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0];modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0];modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2];modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2];modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2];modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2];modelMatrixf[15] = modelMat[3][3];
	
	// set up projection matricies
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, modelMatrixf );
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv( viewMatrix, 1, GL_FALSE, viewMatrixf);
	
	drawPLY();

    glFlush(); // force output to graphics hardware

	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	static int colors = 1;
	static GLfloat s = 0; //Shear amount
	GLuint colored;
	static int twist = 0;
	GLuint twisted;

	switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'w':
	case 'W':
		//Draw wireframe at a suitable initial position from the viewer.
		translation = Angel::identity();
		velocity[0] = 0; velocity[1] = 0; velocity[2] = 0;
		angular_velocity = 0;
		degrees = 0;
		degreesStart = 0;
		rotation = Angel::identity();
		break;
	case 'n':
	case 'N':
		fileNum = (fileNum+1) % MaxFileNum;
		cout << PLY::listFiles() [fileNum] << endl;
		initPLY(fileNum);
		break;
	case 'p':
	case 'P':
		fileNum = fileNum == 0 ? MaxFileNum-1 : fileNum-1;
		cout << PLY::listFiles() [fileNum] << endl;
		initPLY(fileNum);
		break;
	case 'x': //Toggle -ve X direction
		velocity[0] = velocity[0] == -1 ? 0 : -1;
		break;
	case 'X': //Toggle +ve X direction
		velocity[0] = velocity[0] == 1 ? 0 : 1;
		break;
	case 'y': //Toggle -ve Y direction
		velocity[1] = velocity[1] == -1 ? 0 : -1;
		break;
	case 'Y': //Toggle +ve Y direction
		velocity[1] = velocity[1] == 1 ? 0 : 1;
		break;
	case 'z': //Toggle -ve Z direction
		velocity[2] = velocity[2] == -1 ? 0 : -1;
		break;
	case 'Z': //Toggle +ve Z direction
		velocity[2] = velocity[2] == 1 ? 0 : 1;
		break;

	case 'r':
	case 'R': //Rotate wireframe around current position & do a slideshow
		angular_velocity = angular_velocity == 0 ? -1 : 0;
		degreesStart = (degrees+ degreesStart);
		degrees = 0;
		fileNum = angular_velocity == 0 ? fileNum : fileNum-1;
		break;
	case 'c':
	case 'C': //Toggle between red color & many colors
		colors = colors == 1 ? 0 : 1;
		colored = glGetUniformLocation(program, "colored");
		glUniform1i( colored, colors );
		break;
	case 'h': //Increase X shearing (read instructions)
		s+= (GLfloat) 0.4f;
	case 'H': //Decrease X shearing
		s = s >= 0.2f ? s -0.2f : 0;
		shear = Angel::mat4(1,s,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1);
		break;
	case 't': //Increase Y twisting (read instructions)
		twist += 2;
	case 'T': //Decrease Y twisting.
		twist = twist > 0 ? twist-1 : twist;
		twisted = glGetUniformLocation(program, "twisted");
		glUniform1f( twisted, (GLfloat) twist*0.02f );
		break;
	default:
		return;
    }
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

// Reshape handler
void reshape(int x, int y) {
	//Placing viewport
	glViewport(0,0,x,y);

	width = x;
	height = y;

	glutPostRedisplay();
	
}
//----------------------------------------------------------------------------
// Idle handler
void idle() {
	if(angular_velocity) {
		degrees += angular_velocity/10.0f;
		cout << degrees << endl;
		if(degrees > 360 || degrees < 0.0f) {
			degrees =		   degrees > 0.0f ? 360.0f : 0.0f;
			angular_velocity = degrees > 0 ? -1  : 1;
			fileNum = (fileNum+1) % MaxFileNum;
			cout << PLY::listFiles() [fileNum] << endl;
			initPLY(fileNum);
		}
	}

	//Figure out rotation:
	const GLfloat degreesToRadians = 3.1415926535897932384626433f/180;
	rotation    = Angel::RotateY(degrees+degreesStart);

	//Figure out translation:
	const GLfloat speedFactor = 0.0005f;
	translation = translation  + 
		Angel::mat4(0,0,0,speedFactor * velocity[0],
					0,0,0,speedFactor * velocity[1],
					0,0,0,speedFactor * velocity[2],
					0,0,0,0);



	glutPostRedisplay();
}
//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{

	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
	width = 512;
	height = 512;

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Ezra Davis HW 2" );

	// init glew
    glewInit();

	initGPU();

	//Create geometry for 1st PLY
	initPLY();

	// assign handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );
	// should add menus
	// add mouse handler

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}
