/*
Homework #3
Graphics - CS 4731
Ezra Davis

Program description:
Draws a few L-systems in random colors.
Keys A-E draw L-systems, F draws several L-systems and a car.
The R key toggles rotation. (Which may be jerky. It takes a while to render an L-system.)

Building/Running:
The program should simply be setup to compile and run in visual studio.
You should also be able to compile it by hand by compiling main.cpp and PLY.cpp, which should require all the headers and libraries they need.


Code Organization:
* PLY handles PLY file loading and drawing. (In PLY.cpp)
* Grammar loads L-system files and does all the necessary string manipulation.
* Turtle draws the L-systems using a hierarchical stack (and Grammar's string)
* main.cpp handles glut and program state
* shaderUtils.cpp handles most of the interaction with the vertex shader.
* header.h ties all the headers and files together.

The ply files are in the GLSLExperiment/ply_files folder, the l-system files are in the lsys_files folder.

cube.cpp and plyHelper.cpp aren't actually used, but contains the code from the example for reference.

Each time the program moves to the next model, the ply file is reloaded.

*/


#include "header.h"
#include "PLY.h"
//----------------------------------------------------------------------------

int width = 0;
int height = 0;

// handle to program
GLuint program;

mat4 rotation = Angel::identity();
mat4 translation = Angel::identity();

int l_to_draw = 0;
bool rotating = false;

//----------------------------------------------------------------------------

void initGPU() {

	// Load shaders
    program = makeProgram();
	// sets the default color to clear screen
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); // black background


	setColor(color4(1,0,0,1), program);
}





//----------------------------------------------------------------------------
int t = 0;
#include <cmath>
// this is where the drawing should happen
void display( void )
{
	static Turtle l_sys[] = {
		Turtle(Grammar::listFiles()[0].c_str()),
		Turtle(Grammar::listFiles()[1].c_str()),
		Turtle(Grammar::listFiles()[2].c_str()),
		Turtle(Grammar::listFiles()[3].c_str()),
		Turtle(".\\lsys_files\\lsys_simple.txt")
		//Turtle(".\\lsys_files\\lsys3Dhilbert.txt") //Doesn't work for some reason
	};

	static vec4 zoom_eye[] = {
		vec4(50,200,0,1),
		vec4(50,50,50,1),
		vec4(15,14,15,1),
		vec4(10,10,10,1),
		vec4(10,10,1,1),
		vec4(100,20,0,1)
	};

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0)
								* LookAt(zoom_eye[l_to_draw],vec4(0,zoom_eye[l_to_draw].y*2.0f/3,0,1),vec4(0,1,0,1));
	setPersp(perspectiveMat, program);

	//Constructing the CTM from its constituents. 
	Angel::mat4 modelMat =
		  translation
		* rotation
		;
	
	static PLY floor(".\\ply_files\\1_square.ply", GL_FILL);

	if(l_to_draw == 5) {//Park!
		for(int i=0; i<5; i++) {
			setColor(randColor(), program);
			l_sys[rand() % 5].draw(modelMat * Translate((rand() %60)/1.0f-30, 0, (rand() % 60)/1.0f-30), program);
			glFlush();
		}

		static PLY car(PLY::listFiles()[6].c_str(), GL_FILL);
		setColor(randColor(), program);
		car.draw(modelMat *Translate(10,0,10) * RotateY(30) * Scale(20.0f) * Translate(0,0.13f,0), program);

		setColor(color4(.3f,.3f,.3f,1), program);
		floor.draw(modelMat*Scale(80),program);

	} else {
		setColor(randColor(), program);
		l_sys[l_to_draw].draw(modelMat, program);

		setColor(color4(.3f,.3f,.3f,1), program);
		floor.draw(modelMat*Scale(80),program);
	}

    glFlush(); // force output to graphics hardware

	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
	case 'a':
		l_to_draw = 0;
		break;
	case 'b':
		l_to_draw = 1;
		break;
	case 'c':
		l_to_draw = 2;
		break;
	case 'd':
		l_to_draw = 3;
		break;
	case 'e':
		l_to_draw = 4;
		break;
	case 'f':
		l_to_draw = 5;
		break;
	case 'r':
		rotating = !rotating;
		return;
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

void idle(void) {
	if(rotating) {
		rotation = RotateY(10) * rotation;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{

	Grammar g(Grammar::listFiles()[0].c_str());
	
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
	width = 512;
	height = 512;

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
    glutCreateWindow( "Ezra Davis HW 3" );

	// init glew
    glewInit();

	initGPU();

	// assign handlers
    glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
